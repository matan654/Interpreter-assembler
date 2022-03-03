#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataType.h"
#include "AuxFunctions.h"
#include "LinkedList.h"
#include "FirstPass.h"


int FirstPass(FILE *fd,lptr *labelPtr){
	int type=0;
	char *buf;
	char *buf2;
	char *tok1;
	char *tok2;
	char *tok3;
	char line[MAX_LINE];
	char label[MAX_LABEL_NAME+2];
	int len;
	int currentDC;
	int currentIC;
	int numOfOprnds;
	int addresMethod;
	int oprndVal;
	int prameterFlag;
	int i;
	int labelFlag;
	int errorFlag=0;
	int regFlag;
	int tokFlag;

	IC=0;
	DC=0;
	numLine=0;

	while(!(getNextLine(fd,line))){
		if(strlen(line)>MAX_LINE){
			fprintf(stderr,"Error in line %d: The line is too long\n",numLine);
			errorFlag=1;
		}
		if((IC+DC)>MAX_PROGRAM_SIZE){
			fprintf(stderr,"Error from line %d: The program exceeds the maximum memory size of the processor\n",numLine);
			errorFlag=1;
			return FALSE;
		}
		removeRedundantSpaces(line);
		numLine++;
		labelFlag=0;
		regFlag=0;
		currentDC=DC;
		currentIC=IC;
		tokFlag=0;

		buf=line;
		if(*buf=='\n' || *buf==EOF)
			continue;
		if(*buf==';')
			continue;
		buf2=strpbrk(line,"\"(,): \n");

			if(*buf2==':'){
				buf2++;
				buf=strtok(line,":");
				tokFlag=1;
				len=isLegalLabel(buf);
				if(len<=0){
					errorFlag=1;
					continue;
				}
				labelFlag=1;
				strncpy(label,buf,len);
				label[len]='\0';
				if(*buf2=='\n'){
					fprintf(stderr,"Error in line %d: Label declaration isn't defined as instruction or guidance line, abort\n",numLine);
					errorFlag=1;
					continue;
				}
				if(*buf2==' '){
					buf2++;
					buf=buf2;
					buf2=strpbrk(buf2,"\"(,) \n");
				}
			}
			if(*buf2==' ' || *buf2=='\n'){
				buf2++;
				if(!tokFlag){
					buf=strtok(line," \n");
					tokFlag=1;
				}else
					buf=strtok(NULL," \n");
			}
		type=(*buf=='.')?isGuidanceStatement(buf):ERROR;
		if(type>=0){
			switch(type){
			case DATA: case STRING:
				buf=buf2;
				if(type==DATA)
					if(!storeData(buf))
						break;
				if(type==STRING)
					if(!storeString(buf))
						break;
				if(labelFlag)
					if(!addLabelToList(labelPtr,label,0,0,0,currentDC))
						break;
				continue;
			case EXTERN:
				if(labelFlag)
					fprintf(stderr,"Warning in line %d: The program will ignore the label '%s' it in an extern row\n",numLine,label);/*warning*/
				buf=strtok(NULL," \n");
				tok1=strtok(NULL,"\n");
				if(!buf){
					fprintf(stderr,"Error in line %d: Label name in the external operand statement is missing\n",numLine);
					break;
				}
				if(tok1){
					fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,tok1);
					break;
				}
				len=isLegalLabel(buf);
				if(len<=0)
					break;
				strncpy(label,buf,len);
				label[len]='\0';
				if(!addLabelToList(labelPtr,label,1,0,0,0)){
					break;
				}
				continue;
			case ENTRY:
				if(labelFlag)
					fprintf(stderr,"Warning in line %d: The program will ignore the label '%s' it in an entern row\n",numLine,label);/*warning*/
				buf=strtok(NULL," \n");
				tok1=strtok(NULL,"\n");
				if(tok1){
					fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,tok1);
					break;
				}
				if(!buf){
					fprintf(stderr,"Error in line %d: Label name in the entern operand statement is missing\n",numLine);
					break;
				}
				len=isLegalLabel(buf);
				if(len<=0)
					break;
				continue;
			}
			errorFlag=1;
			DC=currentDC;
			continue;
		}
		type=isTeachingStatement(buf);
		if(type>=0){
			if(labelFlag)
				if(!addLabelToList(labelPtr,label,0,0,1,currentIC+IC_OFFSET)){
					errorFlag=1;
					continue;
				}
			IC++;
			numOfOprnds=getTypeOpertion(type);
			switch(numOfOprnds){
			case TEACH_WITHOUT_OPRAND:
				buf=strtok(NULL,"\n");
				if(buf){
					fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,buf);
					break;
				}
				codeTable[currentIC].firstEncoding.opcode=type;
				continue;
			case TEACH_WITH_ONE_OPRAND:
				buf=strtok(NULL," \n");
				tok1=strtok(NULL,"\n");
				if(!buf){
					fprintf(stderr,"Error in line %d: Missing parameters\n", numLine);
					break;
				}
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,type,DESTINATION,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.opcode=type;
				codeTable[currentIC].firstEncoding.methodDest=addresMethod;
				SetCodeTable(DESTINATION,oprndVal,addresMethod);
				IC++;
				if(tok1){
					fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,tok1);
					break;
				}
				continue;
			case TEACH_WITH_TWO_OPRAND:
				buf=strtok(NULL,",\n");
				tok1=strtok(NULL," \n");
				tok2=strtok(NULL,"\n");
				if(!buf){
					fprintf(stderr,"Error in line %d: Missing parameters\n",numLine);
					break;
				}
				if(!tok1){
					fprintf(stderr,"Error in line %d: Missing parameters or comma\n",numLine);
					break;
				}
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,type,SOURCE,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.opcode=type;
				codeTable[currentIC].firstEncoding.methodSource=addresMethod;
				SetCodeTable(SOURCE,oprndVal,addresMethod);
				regFlag=addresMethod==REG_ADDRESS?1:0;
				buf=tok1;
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,type,DESTINATION,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.methodDest=addresMethod;
				if(regFlag!=1 || addresMethod!=REG_ADDRESS)
					IC++;
				SetCodeTable(DESTINATION,oprndVal,addresMethod);
				IC++;
				if(tok2){
					fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,tok2);
					break;
				}
				continue;
			case TEACH_WITH_CONDITION:
				buf=strtok(NULL,"\n");
				buf=buf2;
				prameterFlag=isJumpStatementLegal(buf);
				if(!prameterFlag)
					break;
				tok1=strtok(buf,"(");
				tok2=strtok(NULL,",");
				tok3=strtok(NULL,")");
				if(prameterFlag==3)
					buf=tok1;
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,type,DESTINATION,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.opcode=type;
				codeTable[currentIC].firstEncoding.methodDest=addresMethod;
				SetCodeTable(DESTINATION,oprndVal,addresMethod);
				IC++;
				if(prameterFlag==1)
					continue;
				buf=tok2;
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,ANY_ADDRES_METHOD,SOURCE,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.parm1=addresMethod;
				SetCodeTable(SOURCE,oprndVal,addresMethod);
				regFlag=addresMethod==REG_ADDRESS?1:0;
				buf=tok3;
				addresMethod=getAddresMethod(buf);
				if(!procesTeach(buf,ANY_ADDRES_METHOD,DESTINATION,addresMethod))
					break;
				oprndVal=getOprndVal(buf,addresMethod);
				codeTable[currentIC].firstEncoding.parm2=addresMethod;
				codeTable[currentIC].firstEncoding.methodDest=JUMP_ADDRESS;
				if(regFlag!=1 || addresMethod!=REG_ADDRESS)
					IC++;
				SetCodeTable(DESTINATION,oprndVal,addresMethod);
				IC++;
				continue;
			}
			errorFlag=1;
			IC=currentIC;
			continue;
		}else{
			fprintf(stderr,"Error in line %d: '%s' is not an instruction or guidance\n", numLine, buf);
			IC=currentIC;
			errorFlag=1;
			continue;
		}
	}
	if(!errorFlag){
		updateLabelsAdress(labelPtr,IC+IC_OFFSET);
		for(i=0;i<DC;i++)
			codeTable[IC++].dataEncoding.data=dataTable[i].dataEncoding.data;
		rewind(fd);
		return TRUE;
	}
	return FALSE;
}
int storeData(char *buf){
	int tempNum;
	char *token;

	if(!checkComma(buf))
		return FALSE;
	token=strtok(buf,",");
	while(token){
		tempNum=isNumber(token);
		if(tempNum==ERROR_NUMBER)
			return FALSE;
		dataTable[DC++].dataEncoding.data=tempNum;
		token=strtok(NULL,",");
	}
	return TRUE;
}
int checkComma(char *buf){
	if(*buf== ','){
		fprintf(stderr,"Error in line %d: Unnecessary comma at the beginning of the statement\n",numLine);
		return FALSE;
	}
	buf++;
	while(*buf!='\n'){
		if(*buf==','){
			if(*(buf+1)=='\n'){
				fprintf(stderr,"Error in line %d: Unnecessary comma at the end of the statement\n",numLine);
				return FALSE;
			}
			if(*(buf+1)==','){
				fprintf(stderr,"Error in line %d: More than two consecutive commas in the statement\n",numLine);
				return 0;
			}
		}
		buf++;
	}
	*buf=',';
	buf++;
	*buf='\0';
	return TRUE;
}
int storeString(char *buf){
	if(*buf!='\"'){
		fprintf(stderr,"Error in line %d: Missing quotation marks(\") at beginning of string\n", numLine);
		return FALSE;
	}
	do{
		buf++;
		dataTable[DC++].dataEncoding.data=*buf;
	}while(*(buf+1)!='\n');
	if(*buf!='\"'){
		fprintf(stderr,"Error in line %d: Missing quotation marks(\") at end of string\n", numLine);
		return FALSE;
	}
	dataTable[DC-1].dataEncoding.data='\0';
	return TRUE;
}
int procesTeach(char *buf,int opcode,int place,int addresMethod){
	if(addresMethod<0)
		return FALSE;
	if(!legalTeachRule[opcode][place][addresMethod]){
		fprintf(stderr,"Error in line %d: %s operand is not compatible with instruction\n",numLine,nameOfPlacOprand[place]);
		return FALSE;
	}
	if(getOprndVal(buf,addresMethod)==ERROR_NUMBER)
		return FALSE;
	return TRUE;
}
int getOprndVal(char *buf,int addresMethod){
	if(addresMethod==REG_ADDRESS)
		return isRegister(buf);
	if(addresMethod==DIRECT_ADDRESS)
		return 0;
	if(addresMethod==IMMEDIATE_ADDRESS){
		buf++;
		return isNumber(buf);
	}
	return FALSE;
}
void SetCodeTable(int place,int oprndVal,int addresMethod){
	if(addresMethod==REG_ADDRESS){
		if(place==DESTINATION)
			codeTable[IC].RegEncoding.regDest =oprndVal;
		else
			codeTable[IC].RegEncoding.regSource=oprndVal;
	}else
		codeTable[IC].addressEncoding.addres=oprndVal;
	return;
}
int isJumpStatementLegal(char *buf){
	char *tok1;
	char *tok2;
	char *tok3;
	char *tok4;
	int len;
	if(!(*buf)){
		fprintf(stderr,"Error in line %d: Missing parameters\n", numLine);
		return FALSE;
	}
	len=strlen(buf);
	tok4=buf+len;
	tok1=strpbrk(buf,"(,)");
	if(!tok1){
		if(!strpbrk(buf," "))
			return 1;
		else{
			fprintf(stderr,"Error in line %d: Invalid row: '%s'\n", numLine,buf);
			return FALSE;
		}
	}
	if(*tok1!='('){
		fprintf(stderr,"Error in line %d: Missing char of separation '('\n",numLine);
		return FALSE;
	}
	if(!(tok1-buf)){
		fprintf(stderr,"Error in line %d: Missing destination operand parameters\n",numLine);
		return FALSE;
	}
	tok2=strpbrk(tok1,",");
	if(!tok2){
		fprintf(stderr,"Error in line %d: Missing char of separation ',' between the parameters of the destination operands\n",numLine);
		return FALSE;
	}
	if((tok2-tok1)<=1){
		fprintf(stderr,"Error in line %d: Missing first parameters of the destination operands\n",numLine);
		return FALSE;
	}
	tok3=strpbrk(tok2,")");
	if(!tok3){
		fprintf(stderr,"Error in line %d: Missing char of separation ')'\n",numLine);
		return FALSE;
	}
	if((tok3-tok2)<=1){
		fprintf(stderr,"Error in line %d: Missing second parameters of the destination operands\n",numLine);
		return FALSE;
	}
	if((tok4-tok3)==1)
		return 3;
	else{
		fprintf(stderr,"Error in line %d: Redundant chars: '%s'\n", numLine,tok3+1);
		return FALSE;
	}
}
