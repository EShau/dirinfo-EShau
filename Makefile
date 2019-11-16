ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: dirinfo_main.o
	$(CC) -o dirinfo dirinfo_main.o

dr_main.o: dirinfo_main.c
	$(CC) -c dirinfo_main.c

run:
	./dirinfo

memcheck:
	valgrind --leak-check=yes ./dirinfo

clean:
	rm *.o
	rm *~
