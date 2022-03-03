#include "DataStruct.h"

#define MAX_LINE 82		/*max line size in assembly source file*/
#define MAX_FILE_NAME 32	/*max char count for file name*/
#define MAX_LABEL_NAME 31	/*max char count for labe name*/
#define MAX_PROGRAM_SIZE 256 /*max program size line count to be assembled*/

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define ERROR_NUMBER -16384/*=110000000000000000*/
#define NUMBER_OF_14_BIT 16383/*=0011111111111111*/

#define DATA 0
#define STRING 1
#define	ENTRY 2
#define EXTERN 3

#define TEACH_WITHOUT_OPRAND 0
#define TEACH_WITH_ONE_OPRAND 1
#define TEACH_WITH_TWO_OPRAND 2
#define TEACH_WITH_CONDITION 3

#define IMMEDIATE_ADDRESS 0
#define DIRECT_ADDRESS 1
#define JUMP_ADDRESS 2
#define REG_ADDRESS 3
#define ANY_ADDRES_METHOD 1

#define DESTINATION 0
#define SOURCE 1
#define PARMETER1 2
#define PARMETER2 3

#define RELOCATABLE 2
#define EXTERNAL 1

#define IC_OFFSET 100	/*memory address of the assembled program after it was loaded to memory*/

#define SURFFIX_ASM ".as"	/*assembly source file suffix*/
#define SURFFIX_OBJ ".ob"	/*assembly object file suffix*/
#define SURFFIX_ENT ".ent"	/*assembly entries file suffix*/
#define SURFFIX_EXT ".ext"	/*assembly externals file suffix*/


extern int IC, DC;
extern int numLine;
extern char *guideLine[];
extern char *opcode[];
extern char *reg[];
extern char *nameOfPlacOprand[];
extern char legalTeachRule[16][2][4];

extern wordOf14Bits codeTable[256];
extern wordOf14Bits dataTable[256];
