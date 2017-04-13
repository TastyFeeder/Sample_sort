CC :=gcc
CFLAG = -pthread
OBJECT = sample_sort.o 
TARGET = sample_sort



all: samplesort 

samplesort: main.o  $(OBJECT)
	$(CC) main.o $(OBJECT) -o $@ $(CFLAG)

main.o :main.c

$(OBJECT): $(OBJECT:.o=.c) $(OBJECT:.o=.h)
	$(CC) -c $(OBJECT:.o=.c) $(CFLAGS)

gen: gendata.c
	$(CC) gendata.c -o $@ 
clean:
	rm *.o
