CC :=gcc
CFLAG = -pthread  -fopenmp -lrt
OBJECT = sample_sort.o 
TARGET = sample_sort



all: samplesort.out gen.out OOO.out

samplesort.out: main.o  $(OBJECT)
	$(CC) main.o $(OBJECT) -o $@ $(CFLAG)

OOO.out: main.o  $(OBJECT)
	$(CC) -O3 main.o  $(OBJECT) -o $@ $(CFLAG) 	
main.o :main.c

$(OBJECT): $(OBJECT:.o=.c) $(OBJECT:.o=.h)
	$(CC) -c $(OBJECT:.o=.c) $(CFLAGS)

gen.out: gendata.c
	$(CC) gendata.c -o $@ 
clean:
	rm *.o
