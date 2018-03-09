
#  Copyright (C), 2014-2014, C-Data Tech. Co., Ltd.
#  文件名:Makefile
#  作者:jh.liang
#  版本:1.0.0          
#  日期:2014/12
#  描述: 此文件定义IPC模块主程序


MODULE_DIR     ?= $(shell pwd)
PRJ_ROOT_DIR   ?= $(shell cd ../..; pwd)
MODULE_NAME    := $(notdir $(MODULE_DIR))

include $(PRJ_ROOT_DIR)/make/Make.config

MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/$(MODULE_NAME)
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/$(MODULE_NAME)
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS          := $(subst .c,.o,$(wildcard *.c)) 
MODULE_DEPS          := $(addprefix $(MODULE_OBJ_DIR)/,$(MODULE_OBJS))

INCLUDE_DIRS         := $(PRJ_INC_DIR) \
						$(COM_DIR)/include
						
CFLAGS += -Wall -Werror $(addprefix -I,$(INCLUDE_DIRS)) -D_GNU_SOURCE -DHAVE_CONFIG_H

$(MODULE_OBJ_DIR)/%.o: $(MODULE_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $< #第一个依赖对象
	
.PHONY: all clean 
all:$(MODULE_OBJ_DIR)/ipc_ker $(MODULE_LIB_DIR)/libipc_if.a

$(MODULE_OBJ_DIR)/ipc_ker: $(MODULE_OBJ_DIR)/ipc_ker.o
	$(CC) $(CFLAGS) -o $@ $<
	#$(CC) $(CFLAGS) -DSTT_UNDERLYING_OS=U_LINUX -o $@ $^ -Wl,--start-group $(APP_DEP_LIBS) -Wl,--end-group 
	#$(MV) ipc_ker.o $(MODULE_OBJ_DIR)
	#$(MV) ipc_ker $(MODULE_OBJ_DIR)
	
$(MODULE_LIB_DIR)/libipc_if.a: $(MODULE_OBJ_DIR)/ipc_if.o
	$(AR) $(ARFLAGS) $@ $<
	#$(MV) ipc_if.o $(MODULE_OBJ_DIR)
	#$(MV) libipc_if.a $(MODULE_LIB_DIR)

clean:
	#-rm -f *.o ipc_ker libipc_if.a
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	

