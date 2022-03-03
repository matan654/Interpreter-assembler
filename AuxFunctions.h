

int getNextLine(FILE *fd, char *line);/*
*	copy new line from file to pointer char array.
*	exspects a pointer to open file and pointer to char array.*/
void removeRedundantSpaces(char *line);/*
*	removes redundant whitespaces from a given line fo assembly code.
*	exspects a pointer to char array*/
int isLegalLabel(char *buf);/*
*	checks if label name is legal according to specifications.
*	expects pointers to label name
*	return the length of the string if true, and -1 if false.*/
int isRegister(char *buf);/*
*	check if a word in the assembly code is a register name,
*	expects pointer to string
*	return register number if yes, -1 otherwise.*/
int isTeachingStatement(char *buf);/*
*	check if a word in the assembly code is an teaching,
*	return index in instructionTypes array if yes, -1 otherwise.
*	expects pointer to string*/
int isGuidanceStatement(char *buf);/*
*	check if a word in the assembly code is a guidance word,
*	return index in guidanceTypes array if yes, -1 otherwise.
*	expects pointer to string*/
int getTypeOpertion(int opcode);/*
*	divide the type of the opretuon to a 4 diffrent type:
*	expects the number of the opcode.
*	return: 0. without oprand.
*			1. with one oprand.
*			2. with two oprand.
*			3. thech with jump.*/
int getAddresMethod(char *buf);/*
*	check the type address method of the oprand.
*	expects pointers to oprand string name.
*	return: 0 - if is number.
*			1 - if is label.
*			3 - if is register.
*			-1 - if an error.*/
int isNumber(char *buf);/*
*	convert pinter to string of number to an integer number.
*	expects pointers to oprand from type number.
*	return the value of the converting number, and ERROR_NUMBER if an error.*/


