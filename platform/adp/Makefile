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

MODULE_OBJS          := $(subst .c,.o,$(wildcard src/*.c)) 
MODULE_DEPS          := $(MODULE_OBJS:src%=$(MODULE_OBJ_DIR)%)

INCLUDE_DIRS         := $(PLATFORM_DIR)/include \
						$(PLATFORM_DIR)/sqlite/include \
						$(PLATFORM_DIR)/libev/include \
						$(PLATFORM_DIR)/zlib/include

CFLAGS               += -Wall -Werror -g $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H

$(MODULE_OBJ_DIR)/%.o: adaptor_net_if.c pon_adaptor_errno.c pon_adaptor_common_if.c
	$(CC) -c $(CFLAGS) -o $@ $<

libadp.a: adaptor_net_if.o adaptor_errno.o adaptor_common_if.o
	$(AR) $(ARFLAGS) libadp.a  adaptor_net_if.o adaptor_errno.o adaptor_common_if.o
	$(MKDIR) $(PRJ_LIB_DIR)/adaptor
	$(CP) libadp.a $(PRJ_LIB_DIR)/adaptor/

clean:
	-rm -f *.o libadp.a
