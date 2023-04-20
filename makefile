TARGET=computer.exe
PRINT_TARGET=printer.exe
all:
	gcc spool_ds.h spool_queue.c printer.h main.h pcb_linked_list.c printer.c print.c scheduler.c shell.c memory.c cpu.c computer.c load.c -o $(TARGET)

printer_test:
	gcc -g spool_queue.c printer.c print.c main.c -o $(PRINT_TARGET)
run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(TARGET)

