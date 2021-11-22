p3: Hist_list Mem_list File Prints Auxiliars Memory
	gcc -Wall hist_list.o file.o prints.o memory.o mem_list.o auxiliars.o p3.c -o p3

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

Auxiliars:
	gcc -Wall auxiliars.c -c

clean:
	rm Hist_List.o file.o mem_list.o prints.o memory.o auxiliars.o p3
