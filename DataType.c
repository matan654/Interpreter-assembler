#include<stdlib.h>
#include "DataStruct.h"

int IC, DC;
int numLine;	/*count the number of the current line of the file*/
char *guideLine[]={"data","string","entry","extern",NULL};	/*Char array to hold the guidance types */
char *opcode[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop",NULL};/*
*	Char array to hold the opcode types */
char *reg[]={"r0","r1","r2","r3","r4","r5","r6","r7",NULL};/*
*	Char array to hold the register types */
char *nameOfPlacOprand[]={"Destination","Source"};

wordOf14Bits codeTable[MAX_PROGRAM_SIZE];
wordOf14Bits dataTable[MAX_PROGRAM_SIZE];


/*	 legalTeachRule[opcode][oprandPlace][adressMethod]*/
char legalTeachRule[16][2][4]= {{{0,1,0,1},{1,1,0,1}},
								{{1,1,0,1},{1,1,0,1}},
								{{0,1,0,1},{1,1,0,1}},
								{{0,1,0,1},{1,1,0,1}},
								{{0,1,0,1},{0,0,0,0}},
								{{0,1,0,1},{0,0,0,0}},
								{{0,1,0,1},{0,1,0,0}},
								{{0,1,0,1},{0,0,0,0}},
								{{0,1,0,1},{0,0,0,0}},
								{{0,1,1,1},{0,0,0,0}},
								{{0,1,1,1},{0,0,0,0}},
								{{0,1,0,1},{0,0,0,0}},
								{{1,1,0,1},{0,0,0,0}},
								{{0,1,1,1},{0,0,0,0}},
								{{0,0,0,0},{0,0,0,0}},
								{{0,0,0,0},{0,0,0,0}}};
