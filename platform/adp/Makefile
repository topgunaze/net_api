#  Copyright (C)
#  文件名:Makefile
#  作者:
#  版本:1.0.0          
#  日期:
#  描述: 

#持续集成

MODULE_DIR     ?= $(shell pwd)
PRJ_ROOT_DIR   ?= $(shell cd ../..; pwd)

-include $(PRJ_ROOT_DIR)/make/Make.config

MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/adp
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/adp
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS          := $(subst .c,.o,$(wildcard *.c)) 
MODULE_DEPS          := $(MODULE_OBJS: %=$(MODULE_OBJ_DIR)%)

INCLUDE_DIRS         := $(PRJ_INC_DIR) \
						$(PLATFORM_DIR)/sqlite/include \
						$(PLATFORM_DIR)/libev/include \
						$(PLATFORM_DIR)/zlib/include

CFLAGS               += -Wall -Werror -g $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H

$(MODULE_OBJ_DIR)/%.o: $(MODULE_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $< #第一个依赖对象

.PHONY: all clean 
all: $(MODULE_LIB_DIR)/libadp.a

$(MODULE_LIB_DIR)/libadp.a: $(MODULE_DEPS)
	$(AR) $(ARFLAGS) $@ $^ #目标 依赖对象

clean:
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	
