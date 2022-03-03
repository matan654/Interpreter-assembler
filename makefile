assembler: FirstPass.o SecondPass.o AuxFunctions.o LinkedList.o DataType.o main.o 
	gcc -g -ansi -Wall -pedantic FirstPass.o SecondPass.o AuxFunctions.o LinkedList.o DataType.o main.o   -o assembler
main.o: main.c FirstPass.h SecondPass.h LinkedList.h DataType.h DataStruct.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
FirstPass.o: FirstPass.c FirstPass.h DataType.h LinkedList.h AuxFunctions.h DataStruct.h
	gcc -c -ansi -Wall -pedantic FirstPass.c -o FirstPass.o
SecondPass.o: SecondPass.c SecondPass.h AuxFunctions.h LinkedList.h DataType.h DataStruct.h
	gcc -c -ansi -Wall -pedantic SecondPass.c -o SecondPass.o
AuxFunctions.o: AuxFunctions.c AuxFunctions.h DataType.h DataStruct.h
	gcc -c -ansi -Wall -pedantic AuxFunctions.c -o AuxFunctions.o
LinkedList.o: LinkedList.c LinkedList.h DataType.h DataStruct.h
	gcc -c -ansi -Wall -pedantic LinkedList.c -o LinkedList.o
DataType.o: DataType.c DataType.c DataStruct.h
	gcc -c -ansi -Wall -pedantic DataType.c -o DataType.o
