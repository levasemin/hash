# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=g++
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
BuildF = obj/
all: clean main


main :  $(BuildF)main.o $(BuildF)hash_table.o $(BuildF)hash_functions.o $(BuildF)list.o $(BuildF)read_file.o $(BuildF)plot.o 
	$(CC) -g $(BuildF)main.o $(BuildF)hash_table.o $(BuildF)hash_functions.o $(BuildF)list.o $(BuildF)read_file.o $(BuildF)plot.o -o hash_table.out


$(BuildF)main.o:
	$(CC) -c $(CFLAGS) main.cpp -o $(BuildF)main.o

$(BuildF)hash_functions.o:
	$(CC) -c $(CFLAGS) hash_functions.cpp -o $(BuildF)hash_functions.o

$(BuildF)hash_table.o:
	$(CC) -c $(CFLAGS) hash_table.cpp -o $(BuildF)hash_table.o

$(BuildF)list.o:
	$(CC) -c $(CFLAGS) list.cpp -o $(BuildF)list.o

$(BuildF)read_file.o:
	$(CC) -c $(CFLAGS) read_file.cpp -o $(BuildF)read_file.o

$(BuildF)plot.o:
	$(CC) -c $(CFLAGS) plot.cpp -o $(BuildF)plot.o

clean:
	rm -rf obj/*.o