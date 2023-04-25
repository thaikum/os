TARGET=computer.exe
PRINT_TARGET=printer.exe
READY_QUEUE=readyQ.exe
all:
	gcc -c *.c
	gcc *.o -o $(TARGET)
	rm *.o

run:
	./$(TARGET)

clean:
	rm -f *.exe *.spool *.out
