EXEC=ucasnet
CC=cc
LDFLAGS=
.PHONY:clear

all:$(EXEC) clear
ucasnet: src/Main.c ProcessResult.o Connect.o
	$(CC) -o $@ $< ProcessResult.o Connect.o $(LDFLAGS)
%.o:src/%.c
	$(CC) -c -o $@ $<
clean:
	rm -f *.o $(EXEC) 
clear:
	rm -f *.o
