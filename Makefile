EXEC=online offline
CC=cc
LDFLAGS=-lcurl
.PHONY:clear

all:$(EXEC) clear
online: src/Login.c include/common.h process.o
	$(CC) -o $@ $< process.o $(LDFLAGS)
offline: src/Logout.c include/common.h process.o
	$(CC) -o $@ $< process.o $(LDFLAGS)
process.o: src/ProcessResult.c
	$(CC) -c -o $@ $<
clean:
	rm -f *.o $(EXEC)
clear:
	rm -f *.o
