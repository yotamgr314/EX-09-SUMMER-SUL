CC = gcc
OBJS = main.o Node.o Queue.o
EXEC = AsyncPrinter
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -c
$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

main.o: main.c Queue.h
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $*.c

Node.o: Node.c Node.h
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $*.c

Queue.o: Queue.c Queue.h Node.h
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)
