
CROSS_COMPILE=
STRIP	= $(CROSS_COMPILE)strip
CC	= $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
TARGET=arm-linux

IFLAGS = -I../include 
CFLAGS +=-g -O2
LDFLAGS += -L./

BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
LIB_PATH = $(BUILD_PATH)/lib

OBJ=  tcpclient.o communicate.o getAllChannelSignal.o getOutRate.o getMultiMethod.o getMultiProgNum.o getMultiProgNum2.o setMultiProgNum.o getPidMapSumCnt.o curCHNSearched.o

LIB_OBJ=  communicate.o getAllChannelSignal.o getOutRate.o getMultiMethod.o getMultiProgNum.o getMultiProgNum2.o setMultiProgNum.o getPidMapSumCnt.o curCHNSearched.o

.PHONY: all clean distclean

all:build install

build:
	$(CC) $(CFLAGS) $(IFLAGS) -c tcpclient.c
	$(CC) $(CFLAGS) $(IFLAGS) -c communicate.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getAllChannelSignal.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getOutRate.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getMultiMethod.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getMultiProgNum.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getMultiProgNum2.c
	$(CC) $(CFLAGS) $(IFLAGS) -c setMultiProgNum.c
	$(CC) $(CFLAGS) $(IFLAGS) -c getPidMapSumCnt.c
	$(CC) $(CFLAGS) $(IFLAGS) -c curCHNSearched.c
	
	
	$(CC) $(OBJ) -o tcpclient $(LDFLAGS) $(DBS_LIB) $(BOARDAPI_LD_FLAGS)

	$(CC) $(LIB_OBJ) -shared -Wall -fPIC -o libtcpclient.so

	$(STRIP) tcpclient 

install:
	mkdir -p $(BIN_PATH)
	cp tcpclient $(BIN_PATH)/
	mkdir -p $(LIB_PATH)
	mv libtcpclient.so $(LIB_PATH)/
	mkdir -p $(LIB_PATH)/include
	cp -a *.h $(LIB_PATH)/include


clean:
	rm -f *.o tcpclient
	rm -rf $(BUILD_PATH)

distclean:
	rm -f *.o tcpclient
	rm -rf $(BIN_PATH)
	rm -rf $(LIB_PATH)

