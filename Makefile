all: st_pipeline

st_pipeline: st_pipeline.h st_pipeline.o aofuncs.o queue.o
	gcc -o st_pipeline st_pipeline.o aofuncs.o queue.o -pthread -lm

st_pipeline.o: st_pipeline.c
	gcc -c st_pipeline.c -pthread -lm

aofuncs.o: aofuncs.c
	gcc -c aofuncs.c -pthread

queue.o: queue.c
	gcc -c queue.c -pthread

myqueue.o: myqueue.c
	gcc -c myqueue.c -pthread

.PHONY: all clean

clean:
	-rm *.o *.so client st_pipeline