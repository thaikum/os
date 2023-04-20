TARGET=computer.exe
all:
	gcc spool_ds.h spool_queue.c printer.h main.h pcb_linked_list.c printer.c print.c scheduler.c shell.c memory.c cpu.c computer.c load.c -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

