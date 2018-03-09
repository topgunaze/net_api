#  Copyright (C)
#  文件名:Makefile
#  作者:
#  版本:1.0.0          
#  日期:
#  描述: 


MODULE_DIR     ?= $(shell pwd)
PRJ_ROOT_DIR   ?= $(shell cd ../..; pwd)
MODULE_NAME    := $(notdir $(MODULE_DIR))

-include $(PRJ_ROOT_DIR)/make/Make.config

MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/$(MODULE_NAME)
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/$(MODULE_NAME)
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS          := $(subst .c,.o,$(wildcard *.c)) 
MODULE_DEPS          := $(addprefix $(MODULE_OBJ_DIR)/,$(MODULE_OBJS))

#利用warning函数打印消息  
$(warning "the value of MODULE_OBJS is $(MODULE_OBJS)")
$(warning "the value of MODULE_DEPS is $(MODULE_DEPS)")

INCLUDE_DIRS         := $(PRJ_INC_DIR) \
						$(SQLITE_DIR)/include \
						$(LIBEV_DIR)/include \
						$(ZLIB_DIR)/include

CFLAGS               += -Wall -Werror $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H

$(MODULE_OBJ_DIR)/%.o: $(MODULE_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $< #第一个依赖对象

.PHONY: all clean 
all: $(MODULE_LIB_DIR)/lib$(MODULE_NAME).a

$(MODULE_LIB_DIR)/lib$(MODULE_NAME).a: $(MODULE_DEPS)
	$(AR) $(ARFLAGS) $@ $^ #目标 依赖对象

clean:
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	
