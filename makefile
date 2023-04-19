TARGET=computer.exe
all:
	gcc main.h readyQ.c scheduler.c shell.c memory.c cpu.c computer.c load.c -o $(TARGET)

clean:
	rm $(TARGET)
