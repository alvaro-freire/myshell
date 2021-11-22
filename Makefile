p3: Hist_list Mem_list File Prints Aux_File Aux_Mem Memory
	gcc -Wall hist_list.o file.o prints.o memory.o mem_list.o aux_file.o aux_mem.o p3.c -o p3

Hist_list:
	gcc -Wall hist_list.c -c

Mem_list:
	gcc -Wall mem_list.c -c

File:
	gcc -Wall file.c -c

Memory:
	gcc -Wall memory.c -c

Prints:
	gcc -Wall prints.c -c

Aux_File:
	gcc -Wall aux_file.c -c

Aux_Mem:
	gcc -Wall aux_mem.c -c

clean:
	rm Hist_List.o file.o mem_list.o prints.o memory.o aux_file.o aux_mem.o p3