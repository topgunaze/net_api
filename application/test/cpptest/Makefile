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

CC := $(CROSS_COMPILE)g++ -O0 -g
 
#  Create some dependency directory 
MODULE_OBJ_DIR := $(PRJ_OBJ_DIR)/cpp_test
MODULE_LIB_DIR := $(PRJ_LIB_DIR)/cpp_test
$(shell $(MKDIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR))

MODULE_OBJS := $(subst .cpp,.o,$(wildcard src/*.cpp)) 
MODULE_DEPS := $(MODULE_OBJS:src%=$(MODULE_OBJ_DIR)%)

#利用warning函数打印消息  
$(warning "the value of MODULE_OBJS is $(MODULE_OBJS)")
$(warning "the value of MODULE_DEPS is $(MODULE_DEPS)")

INCLUDE_DIRS := ./include
				
APP_DEP_LIBS :=

CFLAGS               += -Wall -Werror $(addprefix -I,$(INCLUDE_DIRS)) -DHAVE_CONFIG_H
CC_LIBS              := -lpthread

#  Pattern matching rules: 
$(MODULE_OBJ_DIR)/%.o:     $(MODULE_DIR)/src/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: all lib clean 
all: $(MODULE_OBJ_DIR)/cpp_test

$(MODULE_OBJ_DIR)/cpp_test : $(APP_DEP_LIBS) $(MODULE_DEPS)
	$(CC) $(CFLAGS) -DSTT_UNDERLYING_OS=U_LINUX -D$(VERSION_TYPE) -o $@ $^ -Wl,--start-group $(CC_LIBS) $(APP_DEP_LIBS) -Wl,--end-group 
	$(CROSS_COMPILE)objdump -S $@ > cpp_test_dump.txt
	$(STRIP) $@

clean: 
	$(RM) $(MODULE_OBJ_DIR)/*
	$(RM) $(MODULE_LIB_DIR)/*
	#$(RM) $(BIN_DIR)/cpp_test

test:
	@$(ECHO) $(MODULE_DEPS)
	@$(ECHO) $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/
	@$(ECHO) $(CC) -c $(CFLAGS) -o $(MODULE_OBJ_DIR) $(MODULE_DIR)/src/

