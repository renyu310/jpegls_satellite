#zhangrenyu jpeg-ls encoder and decoder

CC := /usr/bin/g++

TARGETS := JPEGLS

CURRECT_DIR :=$(shell cd ./;pwd)
OBJ_DIR := $(CURRECT_DIR)/objs

INC_DIR := $(CURRECT_DIR)
LD_LIB_DIR := $(CURRECT_DIR) 

#CFLAG := -g -Wno-strict-aliasing -I$(INC_DIR)
CFLAG := -Wall -g -I$(INC_DIR)
CLDFLAG := -L$(LD_LIB_DIR)

src := $(wildcard $(CURRECT_DIR)/*.cpp)
dir := $(notdir $(src))
ALL_OBJS :=$(patsubst %.cpp,%.o,$(dir))
ALL_OBJS := $(addprefix $(OBJ_DIR)/,$(ALL_OBJS))


$TARGETS:$(ALL_OBJS)
	$(CC) $(CFLAG) $(CLDFLAG) $(ALL_OBJS) -o $(TARGETS)

$(OBJ_DIR)/%.o:$(CURRECT_DIR)/%.cpp
	$(CC) $(CFLAG) $(CLDFLAG) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(TARGETS)


