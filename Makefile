EXEC=online offline
CC=cc
LDFLAGS=
.PHONY:clear

all:$(EXEC) clear
online: src/Login.c include/common.h include/connect.h process.o connect.o
	$(CC) -o $@ $< process.o connect.o $(LDFLAGS)
offline: src/Logout.c include/common.h include/connect.h process.o connect.o
	$(CC) -o $@ $< process.o connect.o $(LDFLAGS)
process.o: src/ProcessResult.c
	$(CC) -c -o $@ $<
connect.o: src/Connect.c include/connect.h
	$(CC) -c -o $@ $<
clean:
	rm -f *.o $(EXEC) 
clear:
	rm -f *.o
