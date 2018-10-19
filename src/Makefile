###########################################
#Makefile for simple programs
###########################################
CC=gcc
CXX=g++
LINK=g++

#-fpermissive：表示兼容老语法
COM_FLAGS= -g -Wall -DTEST
SO_FLAGS=-fPIC
SO_LINK=-shared
S_FLAGS= -fpermissive
C_FLAGS=  -std=c11
CXX_FLAGS= -std=c++11
INC_PATH= -I ../include
LIB_PATH=
EXT_LIB= -lpthread

PRG=test

SRCS=$(wildcard *.c)
SRCS+=$(wildcard *.cpp)

C_OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CXX_OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))


all:$(PRG)

#链接的时候需要库文件
$(PRG):$(CXX_OBJS) $(C_OBJS)
	$(CXX) $^ $(LIB_PATH) $(EXT_LIB)  -o $@

#编译的时候需要头文件
%.o:%.cpp
	$(CXX) $(COM_FLAGS) $(CXX_FLAGS) -c $< $(INC_PATH) -o $@

%.o:%.c
	$(CC) $(COM_FLAGS) -c $< $(INC_PATH) -o $@

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -rf $(PRG) $(CXX_OBJS) $(C_OBJS)
