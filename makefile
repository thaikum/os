TARGET=computer.exe
PRINT_TARGET=printer.exe
READY_QUEUE=readyQ.exe
all:
	gcc spool_ds.h spool_queue.c printer.h main.h pcb_linked_list.c printer.c print.c scheduler.c shell.c memory.c cpu.c computer.c load.c -o $(TARGET)

printer_test:
	gcc -g spool_queue.c printer.c print.c main.c -o $(PRINT_TARGET)

ready_queue_test:
	gcc -g pcb_linked_list.c main.c -o $(READY_QUEUE)

readyQrun:
	./$(READY_QUEUE)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(PRINT_TARGET)
	rm -f $(READY_QUEUE)

