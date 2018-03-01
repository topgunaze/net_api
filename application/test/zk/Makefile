#  Name         : Makefile
#  Version      :
#  Author       :
#  Time         :
#  Description  :
#                
#                
#  Modification :

MODULE_DIR     ?= $(shell pwd)
PRJ_ROOT_DIR   ?= $(shell cd ../../..; pwd)

-include $(PRJ_ROOT_DIR)/make/Make.config

#  Create some dependency directory 
MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/zk
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/zk
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS := $(subst .c,.o,$(wildcard src/*.c)) 
MODULE_DEPS := $(MODULE_OBJS:src%=$(MODULE_OBJ_DIR)%)

INCLUDE_DIRS := $(PRJ_INC_DIR) $(APP_INC_DIR) $(COM_DIR)/include \
				./include \
				$(LIBEV_DIR)/include $(ZLIB_DIR)/include
				
APP_DEP_LIBS :=	$(ADP_DIR)/libadp.a $(PRJ_LIB_DIR)/common/libcommon.a

CFLAGS               += -Wall -Werror -g $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H
CC_LIBS              := -lm -lrt -lpthread -L$(ZLIB_DIR)/install/lib -lz -L$(LIBEV_DIR)/install/lib -lev #-L$(PRJ_LIB_DIR)/vtysh -lvtysh

#  Pattern matching rules: 
$(MODULE_OBJ_DIR)/%.o:     $(MODULE_DIR)/src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: all lib clean 
all: $(MODULE_OBJ_DIR)/zk

$(MODULE_OBJ_DIR)/zk : $(APP_DEP_LIBS) $(MODULE_DEPS)
	$(CC) $(CFLAGS) -DSTT_UNDERLYING_OS=U_LINUX -o $@ $^ -Wl,--start-group $(CC_LIBS) $(APP_DEP_LIBS) -Wl,--end-group 
	$(CROSS_COMPILE)objdump -S $@ > zk_dump.txt
	$(STRIP) $@

clean: 
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	$(RM) $(BIN_DIR)/zk 

test:
	@$(ECHO) $(MODULE_DEPS)
	@$(ECHO) $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/
	@$(ECHO) $(CC) -c $(CFLAGS) -o $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/

