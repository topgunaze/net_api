/**************************************************************
* 文件名称: 
* 文件描述:
* 版           本: 
* 修改历史:
*     <修改者>     <时间>      <版本 >     <描述>

**************************************************************/

#ifndef __ADAPTOR_ERRNO__
#define __ADAPTOR_ERRNO__

#include "adaptor_common_if.h"


#if DEFUNC("全局错误码枚举")

typedef enum
{
    NODE_SDK_RC_OK                                 = 0,   /**< OK */
    NODE_SDK_RC_IN_PROGRESS                        = 1,   /**< Operation is in progress */
    NODE_SDK_RC_PARM                               = 2,   /**< Error in parameters */
    NODE_SDK_RC_NOMEM                              = 3,   /**< No memory */
    NODE_SDK_RC_NORES                              = 4,   /**< No resources */
    NODE_SDK_RC_INTERNAL                           = 5,   /**< Internal error */
    NODE_SDK_RC_NOENT                              = 6,   /**< Entry doesn't exist */
    NODE_SDK_RC_NODEV                              = 7,   /**< Device doesn't exist */
    NODE_SDK_RC_ALREADY                            = 8,   /**< Entry already exists */
    NODE_SDK_RC_RANGE                              = 9,   /**< Out of range */
    NODE_SDK_RC_PERM                               = 10,  /**< No permission to perform an operation */
    NODE_SDK_RC_NOT_SUPPORTED                      = 11,  /**< Operation is not supported */
    NODE_SDK_RC_PARSE                              = 12,  /**< Parsing error */
    NODE_SDK_RC_INVALID_OP                         = 13,  /**< Invalid operation */
    NODE_SDK_RC_IO                                 = 14,  /**< I/O error */
    NODE_SDK_RC_STATE                              = 15,  /**< Object is in bad state */
    NODE_SDK_RC_DELETED                            = 16,  /**< Object is deleted */
    NODE_SDK_RC_TOO_MANY                           = 17,  /**< Too many objects */
    NODE_SDK_RC_NO_MORE                            = 18,  /**< No more entries */
    NODE_SDK_RC_OVERFLOW                           = 19,  /**< Buffer overflow */
    NODE_SDK_RC_COMM_FAIL                          = 20,  /**< Communication failure */
    NODE_SDK_RC_NOT_CONNECTED                      = 21,  /**< No connection with the target system */
    NODE_SDK_RC_SYSCALL_ERR                        = 22,  /**< System call returned error */
    NODE_SDK_RC_MSG_ERROR                          = 23,  /**< Received message is insane */
    NODE_SDK_RC_TOO_MANY_REQS                      = 24,  /**< Too many outstanding requests */
    NODE_SDK_RC_TIMEOUT                            = 25,  /**< Operation timed out */
    NODE_SDK_RC_TOO_MANY_FRAGS                     = 26,  /**< Too many fragments */
    NODE_SDK_RC_NULL                               = 27,  /**< Got NULL pointer */
    NODE_SDK_RC_READ_ONLY                          = 28,  /**< Attempt to set read-only parameter */
    NODE_SDK_RC_ONU_ERR_RESP                       = 29,  /**< ONU returned an error restfse */
    NODE_SDK_RC_MANDATORY_PARM_IS_MISSING          = 30,  /**< Mandatory parameter is missing */
    NODE_SDK_RC_KEY_RANGE                          = 31,  /**< Key field was out of range */
    NODE_SDK_RC_QUEUE_EMPTY                        = 32,  /**< Rx PCIe queue empty */
    NODE_SDK_RC_QUEUE_FULL                         = 33,  /**< Tx PCIe queue full */
    NODE_SDK_RC_TOO_LONG                           = 34,  /**< Processing is taking too long, but will finish eventually */
    NODE_SDK_RC_INSUFFICIENT_LIST_MEM              = 35,  /**< Not enough memory was provided for variable-length lists */
    NODE_SDK_RC_OUT_OF_SYNC                        = 36,  /**< Sequence number or operation step was out of sync. */
    NODE_SDK_RC_CHECKSUM                           = 37,  /**< Checksum error */
    NODE_SDK_RC_IMAGE_TYPE                         = 38,  /**< Unsupported file/image type */
    NODE_SDK_RC_INCOMPLETE_TERMINATION             = 39,  /**< Incomplete premature termination */
} node_sdk_errno;

typedef enum
{
    NODE_CTRL_RC_OK = 0,

    NODE_CTRL_RC_NET_INIT,
    NODE_CTRL_RC_NET_NOT_CONNECT,
    NODE_CTRL_RC_NET_NOT_WORK,
    NODE_CTRL_RC_TASK_CREATE,
    NODE_CTRL_RC_MODULE_INIT,
    NODE_CTRL_RC_OMCI_FUNC_REG,

    NODE_CTRL_RC_MSG_SYN_VARIFY,
    NODE_CTRL_RC_PARAM_GET,
    NODE_CTRL_RC_MSG_SYN_WAITE,

    NODE_CTRL_RC_PORT_CFG_AND_ACTIVE,//10
    NODE_CTRL_RC_AUTO_INDICATION_OUT_OF_RANGE,
    NODE_CTRL_RC_PROXY_INDICATION_OUT_OF_RANGE,

    NODE_CTRL_RC_MEM_ALLOCATION,
    NODE_CTRL_RC_PARAM_NULL,
    NODE_CTRL_RC_PARAM_OUT_OF_RANGE,
    
    NODE_CTRL_RC_MSG_QUEUE_CREATE,
    NODE_CTRL_RC_MSG_QUEQUE_PUT,
    NODE_CTRL_RC_MSG_PACK,
    NODE_CTRL_RC_MSG_CMD_UNKNOWN,

    NODE_CTRL_RC_NET_BIND,
    NODE_CTRL_RC_NET_CONNCET,

    NODE_CTRL_RC_OPERATION_BUSY,
    NODE_CTRL_RC_OPERATION_TIMEOUT,
    NODE_CTRL_RC_OPERATION_UNREASONABLE,

    NODE_CTRL_RC_SQL_DB_INIT,
    NODE_CTRL_RC_SQL_DB_NOT_FOUND,
    
}node_zk_errno;

typedef enum
{
    NODE_NODE_RC_OK = 0, 

    NODE_NODE_RC_PARAM_NULL,
    NODE_NODE_RC_PARAM_OUT_OF_RANGE,

    NODE_NODE_RC_TASK_CREATE,

    NODE_NODE_RC_OPERATION_BUSY,
    NODE_NODE_RC_OPERATION_TIMEOUT,
    NODE_NODE_RC_OPERATION_UNREASONABLE,
    
}node_node_errno;

typedef enum
{
    MDW_NET_RC_OK = 0,
    
    MDW_NET_RC_ERROR,
    MDW_NET_RC_PARAM_NULL,
    MDW_NET_RC_PARAM_OUT_OF_RANGE,
    MDW_NET_RC_MEM_ALLOCATE,

    MDW_NET_RC_REC_PACKET,
    MDW_NET_RC_INTR,
    MDW_NET_RC_RBUFFER_EMPTY,
    MDW_NET_RC_REMOTE_DISCONNECT,
    MDW_NET_RC_LOCAL_DISCONNECT,
    MDW_NET_RC_SETSOCKOPT,
    MDW_NET_RC_GETSOCKOPT,
    MDW_NET_RC_FCNTL,
    MDW_NET_RC_INIT,
    MDW_NET_RC_MSG_HEADER_VERIFY,

    MDW_NET_RC_MSG_SEM_WAIT,
    MDW_NET_RC_MSG_CMD_ID_OUT_OF_RANGE,

    MDW_NET_RC_MUTEX_CREATE,
    MDW_NET_RC_RWLOCK_CREATE,
    
    MDW_NET_RC_COMPRESS_NO_NEED,
    MDW_NET_RC_UNCOMPRESS_NO_NEED,
    MDW_NET_RC_COMPRESS_FAIL,
    MDW_NET_RC_UNCOMPRESS_FAIL,
    
}mdw_net_errno;

typedef enum
{
    MDW_LIBEV_RC_OK = 0,
    
}mdw_libev_errno;

typedef enum
{
    MDW_SQL_RC_OK = 0,           /* Successful result */
    MDW_SQL_RC_ERROR,         /* SQL error or missing database */
    MDW_SQL_RC_INTERNAL,      /* Internal logic error in SQLite */
    MDW_SQL_RC_PERM,          /* Access permission denied */
    MDW_SQL_RC_ABORT,         /* Callback routine requested an abort */
    MDW_SQL_RC_BUSY,          /* The database file is locked */
    MDW_SQL_RC_LOCKED,        /* A table in the database is locked */
    MDW_SQL_RC_NOMEM,         /* A malloc() failed */
    MDW_SQL_RC_READONLY,      /* Attempt to write a readonly database */
    MDW_SQL_RC_INTERRUPT,     /* Operation terminated by sqlite3_interrupt()*/
    MDW_SQL_RC_IOERR,         /* Some kind of disk I/O error occurred */
    MDW_SQL_RC_CORRUPT,       /* The database disk image is malformed */
    MDW_SQL_RC_NOTFOUND,      /* Unknown opcode in sqlite3_file_control() */
    MDW_SQL_RC_FULL,          /* Insertion failed because database is full */
    MDW_SQL_RC_CANTOPEN,      /* Unable to open the database file */
    MDW_SQL_RC_PROTOCOL,      /* Database lock protocol error */
    MDW_SQL_RC_EMPTY,         /* Database is empty */
    MDW_SQL_RC_SCHEMA,        /* The database schema changed */
    MDW_SQL_RC_TOOBIG,        /* String or BLOB exceeds size limit */
    MDW_SQL_RC_CONSTRAINT,    /* Abort due to constraint violation */
    MDW_SQL_RC_MISMATCH,      /* Data type mismatch */
    MDW_SQL_RC_MISUSE,        /* Library used incorrectly */
    MDW_SQL_RC_NOLFS,         /* Uses OS features not supported on host */
    MDW_SQL_RC_AUTH,          /* Authorization denied */
    MDW_SQL_RC_FORMAT,        /* Auxiliary database format error */
    MDW_SQL_RC_RANGE,         /* 2nd parameter to sqlite3_bind out of range */
    MDW_SQL_RC_NOTADB,        /* File opened that is not a database file */
    MDW_SQL_RC_NOTICE,        /* Notifications from sqlite3_log() */
    MDW_SQL_RC_WARNING,       /* Warnings from sqlite3_log() */
    MDW_SQL_RC_ROW,           /* sqlite3_step() has another row ready */
    MDW_SQL_RC_DONE,          /* sqlite3_step() has finished executing */
}mdw_sql_errno;

typedef enum
{
    APP_CTRL_RC_OK = 0,

    APP_CTRL_RC_ERROR,

    APP_CTRL_RC_NET_INIT,
    APP_CTRL_RC_NET_NOT_CONNECT,
    APP_CTRL_RC_NET_MAP_ALREADY_EXIST,
    APP_CTRL_RC_NET_MAP_IS_FULL,

    APP_CTRL_RC_PARAM_NULL,
    APP_CTRL_RC_PARAM_OUT_OF_RANGE,
    APP_CTRL_RC_PARAM_INVALID,
    APP_CTRL_RC_PARAM_GET,
    APP_CTRL_RC_PARAM_SET,

    APP_CTRL_RC_MSG_QUEUE_CREATE,
    APP_CTRL_RC_MSG_QUEUE_PUT,
    
    APP_CTRL_RC_TASK_CREATE,
    
    APP_CTRL_RC_MSG_SYN_VARIFY,
    APP_CTRL_RC_MSG_SYN_WAITE,

    APP_CTRL_RC_MEM_ALLOCATION,

    APP_CTRL_RC_PORT_OPERATION,
    
    APP_CTRL_RC_IPC_OPERATION,
    APP_CTRL_RC_IPC_PORT_ABSENSE,
    
    APP_CTRL_RC_IMPOSSIBLE_ERROR,

}app_zk_errno;

typedef enum
{
    APP_ALARM_RC_OK,

    APP_ALARM_RC_INIT,
    APP_ALARM_RC_PARAM_ONT_OF_RANGE,

}app_alarm_errno;

#endif

#if DEFUNC("错误码转换接口")

typedef enum
{
    OBJ_DRV_SDK = 100,//drv obj
    OBJ_DRV_ZK,
    OBJ_DRV_FK,
    
    OBJ_MDW_LIBEV = 300, //middleware net
    OBJ_MDW_SQL,
    OBJ_MDW_NET,

    OBJ_APP_CTRL = 400,//app obj
    OBJ_APP_ALARM,
    OBJ_APP_STAT,

    OBJ_ID_NUM_OF
    
}OBJ_ID_E;

#define RETURNCODE_BASE                 1000
#define RETURNCODE_MAX_VALUE            (RETURNCODE_BASE*RETURNCODE_BASE*RETURNCODE_BASE)

#define OBJ_DRV_SDK_RC_OFFSET       100000000
#define OBJ_DRV_OLT_RC_OFFSET       101000000
#define OBJ_DRV_NI_RC_OFFSET        102000000
#define OBJ_DRV_ONU_RC_OFFSET       103000000
#define OBJ_DRV_NW_RC_OFFSET        104000000

#define OBJ_MDW_LIBEV_RC_OFFSET         300000000
#define OBJ_MDW_SQLITE_RC_OFFSET        301000000
#define OBJ_MDW_NET_RC_OFFSET           302000000

#define OBJ_APP_CTRL_RC_OFFSET    400000000
#define OBJ_APP_TFCFG_RC_OFFSET    401000000
#define OBJ_APP_ONUCFG_RC_OFFSET    402000000
#define OBJ_APP_ONURA_RC_OFFSET     403000000
#define OBJ_APP_PROFILE_RC_OFFSET   404000000
#define OBJ_APP_STAT_RC_OFFSET      405000000
#define OBJ_APP_ALARM_RC_OFFSET     406000000

typedef struct
{
    unsigned int obj_id;
    union 
    {
        unsigned int olt_id;
        unsigned int port_id;
        unsigned int onu_id;
        unsigned int profile_id;
    }sub_obj_id;

    unsigned int err_id;
    
}rc_info;

#define ERR_INFO2RC(rc_info,obj,sub_obj,err_no,rc) do{\
        rc_info.obj_id = obj;\
        *(unsigned int*)&rc_info.sub_obj_id = sub_obj;\
        rc_info.err_id = err_no;\
        info2rc_convert(&rc, &rc_info);\
    }while(0)

#define ERRNO_INFO2RC(rc_info,rc,err_no) do{\
        rc_info.err_id = err_no;info2rc_convert(&rc, &rc_info);\
    }while(0)

unsigned int 
rc2info_convert(unsigned int rc_val, rc_info *p_rc_info);

unsigned int 
info2rc_convert(unsigned int *p_rc_val, rc_info *p_rc_info);


#endif


#endif
