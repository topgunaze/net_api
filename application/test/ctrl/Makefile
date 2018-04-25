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
MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/ctrl
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/ctrl
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS := $(subst .c,.o,$(wildcard src/*.c)) 
MODULE_DEPS := $(MODULE_OBJS:src%=$(MODULE_OBJ_DIR)%)

#利用warning函数打印消息  
$(warning "the value of MODULE_OBJS is $(MODULE_OBJS)")
$(warning "the value of VERSION_TYPE is $(VERSION_TYPE) -D$(VERSION_TYPE)")

INCLUDE_DIRS := $(PRJ_INC_DIR) $(APP_INC_DIR) $(COM_DIR)/include \
				./include \
				$(LIBEV_DIR)/include $(ZLIB_DIR)/include
				
APP_DEP_LIBS :=	$(ADP_LIB_DIR)/libadp.a $(PRJ_LIB_DIR)/common/libcommon.a $(IPC_LIB_DIR)/libipc_if.a

CFLAGS               += -Wall -Werror $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H
CC_LIBS              := -lm -lrt -lpthread -L$(ZLIB_DIR)/install/lib -lz -L$(LIBEV_DIR)/install/lib -lev #-L$(PRJ_LIB_DIR)/vtysh -lvtysh

#  Pattern matching rules: 
$(MODULE_OBJ_DIR)/%.o:     $(MODULE_DIR)/src/%.c
	$(CC) -c $(CFLAGS)  -o $@ $<

.PHONY: all lib clean 
all: $(MODULE_OBJ_DIR)/ctrl

$(MODULE_OBJ_DIR)/ctrl : $(APP_DEP_LIBS) $(MODULE_DEPS)
	$(CC) $(CFLAGS) -D$(VERSION_TYPE) -DSTT_UNDERLYING_OS=U_LINUX -o $@ $^ -Wl,--start-group $(CC_LIBS) $(APP_DEP_LIBS) -Wl,--end-group 
	$(CROSS_COMPILE)objdump -S $@ > ctrl_dump.txt
	$(STRIP) $@

clean: 
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	$(RM) $(BIN_DIR)/ctrl

test:
	@$(ECHO) $(MODULE_DEPS)
	@$(ECHO) $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/
	@$(ECHO) $(CC) -c $(CFLAGS) -o $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/

