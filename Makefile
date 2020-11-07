#WINDOWS ?= 0
#ifeq ($(WINDOWS),1)
#CC=i686-w64-mingw32-gcc
#endif

all:
	$(CC) -o server.exe server.c
	$(CC) -o client.exe client.c


clean:
	rm *.exe


	
