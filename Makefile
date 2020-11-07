WINDOWS ?= 0
CFLAGS ?=

ifeq ($(WINDOWS),1)
#CC=i686-w64-mingw32-gcc
CC=x86_64-w64-mingw32-gcc-win32
#Winsock
CFLAGS += -lws2_32
endif

all:
	$(CC) -o server.exe server.c $(CFLAGS)
	$(CC) -o client.exe client.c $(CFLAGS)


clean:
	rm *.exe


	
