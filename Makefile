OBJS=kmeans.o
CFLAGS=-g -O0

all: example2 example1

clean:
	@rm -f *.o example1 example2

example1: $(OBJS) example1.o
	$(CC) $(CFLAGS) $^ -o $@

example2: $(OBJS) example2.o
	$(CC) $(CFLAGS) $^ -o $@

