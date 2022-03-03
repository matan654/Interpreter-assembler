#define MAX_PROGRAM_SIZE 256 /*max program size line count to be assembled*/
typedef struct firstWord{	/*use for array of code table.*/
	unsigned int ARE:2;
	unsigned int methodDest:2;
	unsigned int methodSource:2;
	unsigned int opcode:4;
	unsigned int parm2:2;
	unsigned int parm1:2;
}firstWord;

typedef struct addressWord{	/*use for array of code table.*/
	unsigned int ARE:2;
	unsigned int addres:12;
}addressWord;

typedef struct twoRegWord{	/*use for array of code table.*/
	unsigned int ARE:2;
	unsigned int regDest:6;
	unsigned int regSource:6;
}twoRegWord;

typedef struct dataWord{	/*use for array of data table.*/
	unsigned int data:14;
}dataWord;

typedef union wordOf14Bits{	/*use for array of code table and data table.*/
	firstWord firstEncoding;
	addressWord addressEncoding;
	twoRegWord RegEncoding;
	dataWord dataEncoding;
}wordOf14Bits;

typedef struct labelData{/*
				*	label data type. includes vars for label name, label address, if label is used as extern,
				*	or if label is used as entry or if label is used in teach line.*/
	char labelName[32];
	int addres;
	int isExtern;
	int isEntern;
	int isInTeachLine;
}labelData;

typedef struct labelNode *lptr;	/*pointer to labelNode object*/
typedef struct labelNode{		/*
							*	labelNode data type.
							*	includes label object
							*	and a pointer to the next labelNode object in list*/
	labelData data;
	lptr next;
}labelNode;
