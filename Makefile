p3: Hist_list Mem_list Bg_list Prints Aux_File Aux_Mem Aux_Proc File Memory Process
	gcc -Wall hist_list.o mem_list.o bg_list.o file.o memory.o process.o prints.o aux_file.o aux_mem.o aux_proc.o p3.c -o p3

Hist_list:
	gcc -Wall hist_list.c -c

Mem_list:
	gcc -Wall mem_list.c -c

Bg_list:
	gcc -Wall bg_list.c -c

File:
	gcc -Wall file.c -c

Memory:
	gcc -Wall memory.c -c

Process:
	gcc -Wall process.c -c

Prints:
	gcc -Wall prints.c -c

Aux_File:
	gcc -Wall aux_file.c -c

Aux_Mem:
	gcc -Wall aux_mem.c -c

Aux_Proc:
	gcc -Wall aux_proc.c -c

clean:
	rm Hist_List.o file.o mem_list.o prints.o memory.o aux_file.o aux_mem.o aux_proc.o p3