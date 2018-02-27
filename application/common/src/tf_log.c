/**************************************************************
 * �ļ�����:  tf_log.c
 * ��           ��:  steven.tian
 * ��           ��:  2015.09.22
 * �ļ�����:  system log interface
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

/*************************************************************
˵��:
��ǰϵͳ��־��Ϊ����:�û���Ϣ�͵�����Ϣ
�û���Ϣ��Ҫ�����CLI�ͼ�¼�������ļ�
������ϢĬ��֮��¼�������ļ�
������Ϣ�ṩÿ��ģ������Ƿ�����ӿ�

��ǰʹ�ýӿ�ʱ:
��Ҫ�����CLI����ʹ��: tflog_crit, tflog_err, tflog_warn,
                                                          tflog_info, tflog_notice
tflog_crit(LOG_CRIT):�����¼�
tflog_err(LOG_ERR):��Ҫ�¼�
tflog_warn(LOG_WARNING):��Ҫ�¼�
tflog_notice(LOG_NOTICE):������Ϣ
tflog_info(LOG_INFO):��֪ͨͨ��Ϣ

�����¼�:�����ѷ�����ҵ����������Ҫ�����޸���
1����������·״̬�Ͽ�

��Ҫ�¼�:�����ѷ�����ҵ������̶��𽥷Ŵ���Ҫ�����޸���
1�����ֻ�·
2��ONT������

��Ҫ�¼�:�����ѷ�����ҵ����ʱ��û����
                        ��Ҫ�����ȡ�޸���ʩ������������ص����������
                        Ӱ��ҵ��
1��ONT��֤��Ϣ�Ƿ�
2�������쳣
3���¶��쳣
4����ģ�鲻��λ

������Ϣ:����δ������ҵ�����������
                        ��û�ж��豸����Ӱ�졣Ӧ��ȡ��ϴ�ʩ��
                        �ų�����������������ص����������Ӱ��ҵ��
1��������λ
2�������ָ�����
3���¶��쳣
4��CPUʹ���ʹ���
5���ڴ�ʹ���ʹ���
6��ONT������
7����åONT
8��RSTP�˿�״̬�仯

������Ϣ��ʹ��                :  tflog_debug
1������������
2��API ִ�н���쳣����
3����������״̬������Ϣ
4��IPC ͨ�Ŵ���

*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/param.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tf_log.h"

struct tflog *tflog_default = NULL;

#if defined(BUF_USE_HEAP) && !defined(BUF_USE_STACK)
static char *cliMsgBuf = NULL;
static char *orgMsgBuf = NULL;
#endif

static alarm_event_t *alarm_event_head = NULL;

const char *tflog_priority[] =
{
  "emergencies",
  "alerts",
  "critical",
  "errors",
  "warnings",
  "notifications",
  "informational",
  "debugging",
  NULL,
};

static inline void set_close_on_exec(unsigned int fd)
{
    if (fd < 0)
        return;
    
    /* close on exec */
    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

static int tflog_file_lock(int lock_fd, pthread_mutex_t *plock_mutex)
{
    int rc = 0;
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	rc = pthread_mutex_trylock(plock_mutex);
	if (rc == EBUSY) {
		fprintf(stderr, "PID[%d] pthread_mutex_trylock fail, as lock_mutex is locked by other threads", getpid());
		return -1;
	} else if (rc != 0) {
		fprintf(stderr, "pthread_mutex_trylock fail, rc[%d]", rc);
		return -1;
	}

	if (fcntl(lock_fd, F_SETLK, &fl)) {
		if (errno == EAGAIN || errno == EACCES) {
			/* lock by other process, that's right, go on */
			/* EAGAIN on linux */
			/* EACCES on AIX */
			fprintf(stderr, "fcntl lock fail, as file is lock by other process");
		} else {
			fprintf(stderr, "lock fd[%d] fail, errno[%d]", lock_fd, errno);
		}
		if (pthread_mutex_unlock(plock_mutex)) {
			fprintf(stderr, "pthread_mutex_unlock fail, errno[%d]", errno);
		}
		return -1;
	}

	return 0;
}

static int tflog_file_unlock(int lock_fd, pthread_mutex_t *plock_mutex)
{
	int rc = 0;
	struct flock fl;

	fl.l_type = F_UNLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	if (fcntl(lock_fd, F_SETLK, &fl)) {
		rc = -1;
		fprintf(stderr, "unlock fd[%s] fail, errno[%d]", lock_fd, errno);
	}

	if (pthread_mutex_unlock(plock_mutex)) {
		rc = -1;
		fprintf(stderr, "pthread_mutext_unlock fail, errno[%d]", errno);
	}

	return rc;
}

int tflog_file_safe_write(tflog_catogory_t *cat, char *buf, int datalen)
{
    struct stat stb;
	int do_file_reload = 0;
	int redo_inode_stat = 0;
	FILE *fp = NULL;
    int len;
    char *pBuf;

    /* check if the output file was changed by an external tool by comparing the inode to our saved off one */
	if (stat(cat->filename, &stb)) 
	{
		if (errno != ENOENT) 
		{
			fprintf(stderr, "stat fail on [%s], errno[%d]\r\n", cat->filename, errno);
			return -1;
		} 
		else 
		{
			do_file_reload = 1;
			redo_inode_stat = 1; /* we'll have to restat the newly created file to get the inode info */
		}
	} 
	else if (stb.st_size >= cat->fileSizeLimit)
	{
	    if (tflog_file_lock(cat->lock_fd, &cat->lock_mutex) == 0)
        {
    	    /*fprintf(stderr, "PID[%d] type=%d fd=%d\r\n", getpid(), type, cl->catogory[type].fd);*/
		    close(cat->fd);
			if((fp = fopen(cat->filename, "rb")) == NULL)
			{
				fprintf(stderr, "\r\n Open %s error...\r\n", cat->filename);
				remove(cat->filename);
				tflog_file_unlock(cat->lock_fd, &cat->lock_mutex);
				return -1;
			}

			/* save last len byte */
			len = TF_LOG_LINE_LEN * 100;
			if((pBuf = malloc(len)) != NULL)
			{
				fseek(fp, 0-len, SEEK_END);
				/* drop first line */
				fgets(pBuf, TF_LOG_LINE_LEN, fp);
				len = fread(pBuf, 1, len, fp);	
				fclose(fp);

				if((fp = fopen(cat->filename, "wb")) != NULL)
				{
					fseek(fp, 0, SEEK_SET);
					fwrite(pBuf, 1, len, fp); 
				}
				free(pBuf);
			}				
			fclose(fp);
			tflog_file_unlock(cat->lock_fd, &cat->lock_mutex);
			
			do_file_reload = 1;
		}
	}
	else 
	{
		do_file_reload = (stb.st_ino != cat->ino || stb.st_dev != cat->dev);
	}

	if (do_file_reload) 
	{
	    if (tflog_file_lock(cat->lock_fd, &cat->lock_mutex) != 0)
        {
            return -1;
        }
        
		close(cat->fd);
		cat->fd = open(cat->filename, O_WRONLY | O_APPEND | O_CREAT);
		if (cat->fd < 0) 
		{
			fprintf(stderr, "open file[%s] fail, errno[%d]\r\n", cat->filename, errno);
			tflog_file_unlock(cat->lock_fd, &cat->lock_mutex);
			return -1;
		}

		/* save off the new dev/inode info from the stat call we already did */
		if (redo_inode_stat) 
		{
			if (stat(cat->filename, &stb) < 0) 
			{
				fprintf(stderr, "stat fail on new file[%s], errno[%d]\r\n", cat->filename, errno);
				tflog_file_unlock(cat->lock_fd, &cat->lock_mutex);
				return -1;
			}
		}
		cat->dev = stb.st_dev;
		cat->ino = stb.st_ino;

		tflog_file_unlock(cat->lock_fd, &cat->lock_mutex);
	}

	if (write(cat->fd, buf, datalen) < 0)
	{
	    fprintf(stderr, "write fail, errno[%d] msg:%.*s\r\n", errno, len, buf);
		return -1;
	}

	return 0;
}

/* va_list version of tflog. */
void
vtflog (struct tflog *cl, int type, int priority, const char *format, va_list args)
{
    #if defined(BUF_USE_STACK) && !defined(BUF_USE_HEAP)
    char cliMsgBuf[CLI_MSG_LEN] = {0};
    char orgMsgBuf[CLI_MSG_LEN] = {0};
    #endif

    const char *mark[TF_LOG_TYPE_NUM] = {"oper","alarm","debug"};
    va_list ac;
    int msglen = 0;
    char timeStr[32] = {0};
    time_t t;
    struct tm tmInfo;
    char *newline = NULL;
    
    /* If zlog is not specified, use default one. */
    if (cl == NULL)
        cl = tflog_default;

    /* When zlog_default is also NULL, use stderr for logging. */
    if (cl == NULL)
    {
        #if 0
        fprintf (stderr, "%s: ", "unknown");
        vfprintf (stderr, format, args);
        fprintf (stderr, "\n");
        fflush (stderr);
        #endif

        /* In this case we return at here. */
        return;
    }

    if (type >= TF_LOG_TYPE_NUM)
        return;

    if (cl->flags & TFLOG_STDERR)
    {
        va_copy(ac, args);
        vfprintf(stderr, format, ac);
        va_end(ac);
    }

    /* Syslog output */
    if ((cl->flags & TFLOG_SYSLOG) 
        && (TF_LOG_PRIO_MASK(priority) & cl->catogory[type].syslogPrioMask))
    {
        va_copy(ac, args);
        vsyslog (priority|tflog_default->facility, format, ac);
        va_end(ac);
    }

    va_copy(ac, args);
    msglen = vsnprintf(orgMsgBuf, CLI_MSG_LEN, format, args);
    va_end(ac);

    if (msglen <= 0)
        return;

    time(&t);
	tzset();
    localtime_r(&t, &tmInfo);/* include timezone */
    /* format: 2016/01/06 14:04:15 */
    strftime(timeStr, sizeof(timeStr), "%Y/%m/%d %T ", &tmInfo);
    timeStr[sizeof(timeStr) - 1] = '\0';
    
    msglen = (msglen > (CLI_MSG_LEN - 1)) ? CLI_MSG_LEN - 1 : msglen;

    /* add newline character  */
    if (orgMsgBuf[msglen - 1] != '\n')
    {
        if (msglen == (CLI_MSG_LEN - 1))
        {
            orgMsgBuf[msglen - 1] = '\n';
        }
        else
        {
            orgMsgBuf[msglen + 1] = '\0';
            orgMsgBuf[msglen] = '\n';
        }
    }
    
    /* File output. */
    if ((TF_LOG_PRIO_MASK(priority) & cl->catogory[type].recordPrioMask) && cl->catogory[type].fd != -1)
    {
        snprintf(cliMsgBuf, CLI_MSG_LEN, "%s%s", timeStr, orgMsgBuf);

		tflog_file_safe_write(&cl->catogory[type], cliMsgBuf, strlen(cliMsgBuf));
    }

    /* Terminal monitor. */
    if (cl->termOut
        && (cl->flags & TFLOG_CLI)
        && (TF_LOG_PRIO_MASK(priority) & cl->catogory[type].termPrioMask))
    {
        char *nextline = orgMsgBuf;

        /* Split into multiple lines */
        while(*nextline != '\0' && (newline = strchr(nextline, '\n')))
        {
            char org;
            newline++;
            org = *newline;
            *newline = '\0';
            snprintf(cliMsgBuf, CLI_MSG_LEN, "!</%s>%s", mark[type], nextline);
            cl->termOut(cliMsgBuf);
            *newline = org;
            nextline = newline;
        }

        if (*nextline != '\0')
        {
            snprintf(cliMsgBuf, CLI_MSG_LEN, "!</%s>%s\n", mark[type], nextline);
            cl->termOut(cliMsgBuf);
        }
    }
}

void
tflog_oper (const char *format, ...)
{
    va_list args;

    if (tflog_default == NULL)
        return;

    va_start(args, format);
    vtflog (tflog_default, TF_LOG_TYPE_OPER, LOG_NOTICE, format, args);
    va_end (args);

}

#define TF_OPER_FUNC(funcname, level)\
void funcname(const char *format, ...)\
{\
    va_list args;\
    if (tflog_default == NULL)\
        return;\
    va_start(args, format);\
    vtflog (tflog_default, TF_LOG_TYPE_OPER, level, format, args);\
    va_end (args);\
}

TF_OPER_FUNC(tflog_oper_crit, LOG_CRIT)
TF_OPER_FUNC(tflog_oper_err, LOG_ERR)
TF_OPER_FUNC(tflog_oper_warn, LOG_WARNING)
TF_OPER_FUNC(tflog_oper_info, LOG_INFO)
TF_OPER_FUNC(tflog_oper_notice, LOG_NOTICE)

#define TF_ALARM_FUNC(funcname, level)\
void funcname(const char *format, ...)\
{\
    va_list args;\
    if (tflog_default == NULL)\
        return;\
    va_start(args, format);\
    vtflog (tflog_default, TF_LOG_TYPE_ALARM, level, format, args);\
    va_end (args);\
}

TF_ALARM_FUNC(tflog_alarm_crit, LOG_CRIT)
TF_ALARM_FUNC(tflog_alarm_err, LOG_ERR)
TF_ALARM_FUNC(tflog_alarm_warn, LOG_WARNING)
TF_ALARM_FUNC(tflog_alarm_info, LOG_INFO)
TF_ALARM_FUNC(tflog_alarm_notice, LOG_NOTICE)
TF_ALARM_FUNC(tflog_crit, LOG_CRIT)
TF_ALARM_FUNC(tflog_err, LOG_ERR)
TF_ALARM_FUNC(tflog_warn, LOG_WARNING)
TF_ALARM_FUNC(tflog_info, LOG_INFO)
TF_ALARM_FUNC(tflog_notice, LOG_NOTICE)

void 
tflog_debug (int mod, const char *format, ...)
{
    va_list args;

    if (tflog_default == NULL)
        return;

    if (mod > tflog_default->debug.validCnt)
        return;

    if (!tflog_default->debug.ctrl[mod].debugEn)
        return;
    
    va_start(args, format);
    vtflog (tflog_default, TF_LOG_TYPE_DEBUG, LOG_DEBUG, format, args);
    va_end (args);

}

/*************************************************************
 * opentflog - open log stream
 * 
 * progname[in]: ������
 * mods[in]: ֧�ֵ��Ե�ģ������
 * debugEn[in]: Ĭ�ϵ��Կ���
 * flags[in]:�Ƿ���Ҫ��¼syslog;�Ƿ���ҪCLI���
 * syslog_facility[in]:syslog��facilityֵ֧��(LOG_DAEMON,LOG_LOCAL0-7)
 *  LOG_LOCAL0:TF
 *  LOG_LOCAL1:SWITCH
 *  LOG_LOCAL2:USER VIEW(CLI, SNMP)
 *  LOG_LOCAL3:OTHER
 *  LOG_DAEMON: monitorר��
 * Returns: ����ɹ����طǿ�ָ��,���ʧ�ܷ��ؿ�ָ��
 ************************************************************/
struct tflog *
opentflog (const char *progname, int mods, int debugEn,int flags, int syslog_facility, int (*termOut)(char *))
{
    struct tflog *cl = NULL;
    int syslog_flags = LOG_NDELAY|LOG_PID;
    int idx = 0;

    if (tflog_default)
        return tflog_default;
    
    cl = ZMALLOC(MTYPE_TFLOG, sizeof (struct tflog));
    if (cl == NULL)
    {
        return NULL;
    }
    memset (cl, 0, sizeof (struct tflog));

    cl->debug.ctrl = ZMALLOC(MTYPE_TFLOG, sizeof (debugCtrl_t) * mods);
    if (cl->debug.ctrl == NULL)
    {
        ZFREE(MTYPE_TFLOG, cl);
        return NULL;
    }

    #if defined(BUF_USE_HEAP) && !defined(BUF_USE_STACK)
    cliMsgBuf = ZMALLOC(MTYPE_TFLOG, CLI_MSG_LEN);
    if (cliMsgBuf == NULL)
    {
        ZFREE(MTYPE_TFLOG, cl->debug.ctrl);
        ZFREE(MTYPE_TFLOG, cl);
        return NULL;
    }

    orgMsgBuf = ZMALLOC(MTYPE_TFLOG, CLI_MSG_LEN);
    if (orgMsgBuf == NULL)
    {
        ZFREE(MTYPE_TFLOG, cliMsgBuf);
        ZFREE(MTYPE_TFLOG, cl->debug.ctrl);
        ZFREE(MTYPE_TFLOG, cl);
        return NULL;
    }
    #endif

    cl->debug.validCnt = mods;

    cl->ident = progname;
    cl->flags = flags;
    cl->facility = syslog_facility;

    if (termOut)
        cl->termOut = termOut;

    cl->debug.validCnt = mods;
    cl->debug.termOutEn = 1;
    for (idx = 0; idx < mods; idx++)
    {
        cl->debug.ctrl[idx].debugEn = debugEn;
    }

    cl->catogory[TF_LOG_TYPE_OPER].fd = open(TF_OPER_LOG_FILE, O_APPEND|O_CREAT|O_WRONLY);
    set_close_on_exec(cl->catogory[TF_LOG_TYPE_OPER].fd);
    cl->catogory[TF_LOG_TYPE_OPER].fileSizeLimit = TF_LOG_FILE_SIZE;
    snprintf(cl->catogory[TF_LOG_TYPE_OPER].filename,
             sizeof(cl->catogory[TF_LOG_TYPE_OPER].filename), 
             "%s", TF_OPER_LOG_FILE);
    cl->catogory[TF_LOG_TYPE_OPER].syslogPrioMask = TF_LOG_PRIO_MASK_DEF_DEBUG;
    cl->catogory[TF_LOG_TYPE_OPER].termPrioMask = 0;
    cl->catogory[TF_LOG_TYPE_OPER].recordPrioMask = TF_LOG_PRIO_MASK_DEF_ALARM;
    pthread_mutex_init(&cl->catogory[TF_LOG_TYPE_OPER].lock_mutex, NULL);
    cl->catogory[TF_LOG_TYPE_OPER].lock_fd = open(TF_OPER_LOG_FILE".lock", O_RDWR | O_CREAT,
		                                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    set_close_on_exec(cl->catogory[TF_LOG_TYPE_OPER].lock_fd);
    
    cl->catogory[TF_LOG_TYPE_ALARM].fd = open(TF_ALARM_LOG_FILE, O_APPEND|O_CREAT|O_WRONLY);
    set_close_on_exec(cl->catogory[TF_LOG_TYPE_ALARM].fd);
    cl->catogory[TF_LOG_TYPE_ALARM].fileSizeLimit = TF_LOG_FILE_SIZE;
    snprintf(cl->catogory[TF_LOG_TYPE_ALARM].filename,
             sizeof(cl->catogory[TF_LOG_TYPE_ALARM].filename), 
             "%s", TF_ALARM_LOG_FILE);
    cl->catogory[TF_LOG_TYPE_ALARM].syslogPrioMask= TF_LOG_PRIO_MASK_DEF_DEBUG;
    cl->catogory[TF_LOG_TYPE_ALARM].termPrioMask= TF_LOG_PRIO_MASK_DEF_ALARM;
    cl->catogory[TF_LOG_TYPE_ALARM].recordPrioMask = TF_LOG_PRIO_MASK_DEF_ALARM;
    pthread_mutex_init(&cl->catogory[TF_LOG_TYPE_ALARM].lock_mutex, NULL);
    cl->catogory[TF_LOG_TYPE_ALARM].lock_fd = open(TF_ALARM_LOG_FILE".lock", O_RDWR | O_CREAT,
		                                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    set_close_on_exec(cl->catogory[TF_LOG_TYPE_ALARM].lock_fd);

    cl->catogory[TF_LOG_TYPE_DEBUG].fd = -1;
    cl->catogory[TF_LOG_TYPE_DEBUG].fileSizeLimit = TF_LOG_FILE_SIZE;
    cl->catogory[TF_LOG_TYPE_DEBUG].syslogPrioMask = TF_LOG_PRIO_MASK_DEF_DEBUG;
    cl->catogory[TF_LOG_TYPE_DEBUG].termPrioMask = TF_LOG_PRIO_MASK_DEF_DEBUG;
    cl->catogory[TF_LOG_TYPE_DEBUG].recordPrioMask = 0;
    pthread_mutex_init(&cl->catogory[TF_LOG_TYPE_DEBUG].lock_mutex, NULL);

    openlog (progname, syslog_flags, cl->facility);
    
    if (tflog_default == NULL)
    {
        tflog_default = cl;
    }
    return cl;
}

void
closetflog (struct tflog *cl)
{
    int idx = 0;
    
    closelog();

    for (idx = 0; idx < TF_LOG_TYPE_NUM; idx++)
    {
        if (cl->catogory[idx].fd != -1)
            close (cl->catogory[idx].fd);

        pthread_mutex_destroy(&cl->catogory[TF_LOG_TYPE_DEBUG].lock_mutex);
    }
    #if defined(BUF_USE_HEAP) && !defined(BUF_USE_STACK)
    ZFREE(MTYPE_TFLOG, orgMsgBuf);
    ZFREE(MTYPE_TFLOG, cliMsgBuf);
    #endif
    ZFREE(MTYPE_TFLOG, cl->debug.ctrl);
    ZFREE(MTYPE_TFLOG, cl);
}

/* add by steven.tian */
int alarm_event_reg(alarm_event_t *alarm_event)
{
    alarm_event->next = alarm_event_head;
    alarm_event_head = alarm_event;

    return 0;
}

/* add by steven.tian */
alarm_event_t* alarm_event_get_by_id(unsigned long alarmid)
{
    alarm_event_t *pAlarmEvent;

    for (pAlarmEvent = alarm_event_head; pAlarmEvent; pAlarmEvent = pAlarmEvent->next)
    {
        if (alarmid == pAlarmEvent->id)
        {
            return pAlarmEvent;
        }
    }

    return NULL;
}

/*************************************************************
 * tflog_debug_mod_enable - ģ�����ʹ��
 * 
 * mod[in]: ģ��ID
 * enable[in]: 
 *
 * Returns: ��
 ************************************************************/
void tflog_debug_mod_enable(int mod, int enable)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return;
    }

    if (mod >= cl->debug.validCnt)
    {
        return;
    }

    cl->debug.ctrl[mod].debugEn = enable ? 1 : 0;
}

/*************************************************************
 * tflog_debug_mod_term_enable - ģ����������CLIʹ��
 * 
 * mod[in]: ģ��ID
 * enable[in]: 
 *
 * Returns: ��
 ************************************************************/
void tflog_debug_term_enable(int enable)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return;
    }

    cl->debug.termOutEn = enable ? 1 : 0;
}

/*************************************************************
 * tflog_debug_term_state_get - ģ��������״̬
 * 
 *
 * Returns: �������״̬
 ************************************************************/
int tflog_debug_term_state_get(void)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return 0;
    }

    return (cl->debug.termOutEn ? 1 : 0);
}

/*************************************************************
 * tflog_debug_mod_param_get - ģ����Բ�����ȡ
 * 
 * mod[in]: ģ��ID
 * debugEn[out]: ģ��ʹ�����������Ϣ
 * termEn[out]:������Ϣ�Ƿ������CLI
 *
 * Returns: �ɹ�����0,ʧ�ܷ���-1
 ************************************************************/
int tflog_debug_mod_param_get(int mod, int *debugEn)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return -1;
    }

    if (mod >= cl->debug.validCnt)
    {
        return -1;
    }

    if (debugEn)
    {
        *debugEn = cl->debug.ctrl[mod].debugEn;
    }

    return 0;
}

/*************************************************************
 * tflog_syslog_priority_set - syslog���ȼ���������
 * 
 * prio[in]: ���ȼ�
 *
 * Returns: ��
 ************************************************************/
int tflog_syslog_priority_set(int prio)
{
    struct tflog *cl = tflog_default;
    int  idx = 0;

    if (cl == NULL)
    {
        return -1;
    }

    for (idx = 0; idx < TF_LOG_TYPE_NUM; idx++)
    {
        cl->catogory[idx].syslogPrioMask = TF_LOG_PRIO_MASK_HL(prio);
    }

    return 0;
}

/*************************************************************
 * priority_mask_to_int - ת������Ϊ���ȼ�
 * 
 * mask[in]: ���ȼ�����
 *
 * Returns: ���ȼ�
 ************************************************************/
int inline priority_mask_to_int(unsigned int mask)
{
    int prio;

    for (prio = 0; prio < (LOG_DEBUG + 1); prio++)
    {
        if (!(mask & (1 << prio)))
            break;
    }
    prio--;
    
    return prio;
}

/*************************************************************
 * tflog_syslog_priority_get - syslog���ȼ���ȡ
 * 
 *
 * Returns: ���ȼ�
 ************************************************************/
int tflog_syslog_priority_get(void)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return -1;
    }
    
    return priority_mask_to_int(cl->catogory[TF_LOG_TYPE_ALARM].syslogPrioMask);
}

/*************************************************************
 * tflog_alarm_priority_set - �澯���ȼ���������
 * 
 * prio[in]: ���ȼ�
 *
 * Returns: ��
 ************************************************************/
int tflog_alarm_priority_set(int prio)
{
    struct tflog *cl = tflog_default;
    int  idx = 0;

    if (cl == NULL)
    {
        return -1;
    }

    cl->catogory[TF_LOG_TYPE_ALARM].termPrioMask = TF_LOG_PRIO_MASK_HL(prio);

    return 0;
}

/*************************************************************
 * tflog_alarm_priority_get -�澯���ȼ�������ȡ
 * 
 *
 * Returns: ���ȼ�
 ************************************************************/
int tflog_alarm_priority_get(void)
{
    struct tflog *cl = tflog_default;

    if (cl == NULL)
    {
        return -1;
    }

    return priority_mask_to_int(cl->catogory[TF_LOG_TYPE_ALARM].termPrioMask);
}


