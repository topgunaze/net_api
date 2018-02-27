/**************************************************************
 * �ļ�����:  tf_file.h reference to busybox
 * ��           ��:  steven.tian
 * ��           ��:  2017/02/27
 * �ļ�����:  file operate lib
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/
#ifndef _TF_FILE_H
#define _TF_FILE_H

#include <stddef.h>
#include <stdio.h>

#define CONFIG_FEATURE_COPYBUF_KB 4

#define PROC_MTD_FILE_NAME "/proc/mtd"
#define STACK_LINE_BUF_MAX_LEN 256
#define MTD_NAME_MAX_LEN       256

#define tf_file_err(arg...) printf(arg)

#define FD_CLOSE(fd) do { \
                        if (fd >= 0) \
                        { \
                            close(fd); \
                            fd = -1; \
                        } \
                     }while(0)
                     
#define FILE_CLOSE(fp) do { \
                        if (fp) \
                        { \
                            fclose(fp); \
                            fp = NULL; \
                        } \
                     }while(0)

size_t safe_read(int fd, void *buf, size_t count);
size_t safe_write(int fd, const void *buf, size_t count);
size_t full_read(int fd, void *buf, size_t len);
size_t full_write(int fd, const void *buf, size_t len);
size_t full_fd_copy(int src_fd, int dst_fd, size_t size);
size_t full_file_copy(FILE *src, FILE *dst, size_t size);
int find_mtd_dev_by_name(char *name, char *mtd, int len);

#endif

