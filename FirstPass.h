int FirstPass(FILE *fd,lptr *labelPtr);/*
*	this function contains all the logic of the first pass over the assembly program.
*	expects to: - pointer to file are open
*				- pointer to label linked list
*	return 1 if the firt pass complete wituot error,else -1.*/
int storeData(char *buf);/*
*	saves data guidance information to dataTable.
*	expects pointer to string representation of data value*/
int checkComma(char *buf);/*
*	check correct comma placement in string, expects pointer to string*/
int storeString(char *buf);/*
*	saves string guidance information to dataImage.
*	expects pointer to string value*/
int procesTeach(char *buf,int opcode,int place,int addresMethod);/*
*	parse instruction, check if the oprand of the instruction is legal
*	expects pointer to string,the type opcode,the place in statment line and thr address method.
*	return 1 if is leagl,otherwise 0.*/
int getOprndVal(char *buf,int addresMethod);/*
*	found the value of the oprand by the type of hes address method.
*	expects pointer to the oprand string,and the address method of the line statement.
*	return the value of the oprand.*/
void SetCodeTable(int place,int oprndVal,int addresMethod);/*
*	set the value of oprand in the code table
*	expects the place of the oprand in the line statemnt, the value to set and the address method.*/
int isJumpStatementLegal(char *buf);/*
*	check if an row of jump statement with or without parameters is legal.
*	expects pointer to string from type of jump statement.
*	return 1 if legal, otherwise return 0.*/
