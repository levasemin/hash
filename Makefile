# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
BuildF = obj/
CFLAGS = -c -g  -mavx2
all: clean main


main : $(BuildF)main.o $(BuildF)hash_table.o $(BuildF)hash_functions.o $(BuildF)list.o $(BuildF)intr_functions.o $(BuildF)read_file.o $(BuildF)plot.o $(BuildF)ascii_sum_asm.o $(BuildF)heap.o $(BuildF)test.o
	$(CC) -g -no-pie  -mavx2 $(BuildF)ascii_sum_asm.o $(BuildF)main.o $(BuildF)hash_table.o $(BuildF)hash_functions.o $(BuildF)list.o $(BuildF)intr_functions.o $(BuildF)read_file.o $(BuildF)plot.o $(BuildF)heap.o $(BuildF)test.o -o hash_table.out


$(BuildF)test.o:
	$(CC) $(CFLAGS) test.cpp -o $(BuildF)test.o

$(BuildF)ascii_sum_asm.o:
	nasm -felf64 ascii_sum_asm.nasm -o $(BuildF)ascii_sum_asm.o

$(BuildF)heap.o:
	$(CC) $(CFLAGS) heap.cpp -o $(BuildF)heap.o

$(BuildF)main.o:
	$(CC) $(CFLAGS) main.cpp -o $(BuildF)main.o

$(BuildF)hash_functions.o:
	$(CC) $(CFLAGS) hash_functions.cpp -o $(BuildF)hash_functions.o 

$(BuildF)hash_table.o:
	$(CC) $(CFLAGS) hash_table.cpp -o $(BuildF)hash_table.o

$(BuildF)list.o:
	$(CC) $(CFLAGS) list.cpp -o $(BuildF)list.o

$(BuildF)intr_functions.o:
	$(CC) $(CFLAGS) intr_functions.cpp -o $(BuildF)intr_functions.o

$(BuildF)read_file.o:
	$(CC) $(CFLAGS) read_file.cpp -o $(BuildF)read_file.o

$(BuildF)plot.o:
	$(CC) $(CFLAGS) plot.cpp -o $(BuildF)plot.o

clean:
	rm -rf obj/*.o