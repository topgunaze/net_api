
#  Copyright (C), 2014-2014, C-Data Tech. Co., Ltd.
#  文件名:Makefile
#  作者:jh.liang
#  版本:1.0.0          
#  日期:2014/12
#  描述: 此文件定义IPC模块主程序

PRJ_ROOT_DIR         ?= $(shell cd ../..; pwd)

-include $(PRJ_ROOT_DIR)/make/Make.config

MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/ipc
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/ipc
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

CFLAGS += -D_GNU_SOURCE -I$(PRJ_INC_DIR) -I$(COM_DIR)/include
all:ipc_ker libipc_if.a

ipc_ker:ipc_ker.o
	$(CC) -o ipc_ker $(CFLAGS) ipc_ker.c
	$(MV) ipc_ker.o $(MODULE_OBJ_DIR)
	$(MV) ipc_ker $(MODULE_OBJ_DIR)
	
libipc_if.a:ipc_if.o
	$(AR) $(ARFLAGS) libipc_if.a ipc_if.o
	$(MV) ipc_if.o $(MODULE_OBJ_DIR)
	$(MV) libipc_if.a $(MODULE_LIB_DIR)

clean:
	#-rm -f *.o ipc_ker libipc_if.a
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
