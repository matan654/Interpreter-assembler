#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "DataType.h"
#include "AuxFunctions.h"

int getNextLine(FILE *fd, char *line){
	fgets(line,MAX_LINE,fd);
	return feof(fd);
}
void removeRedundantSpaces(char *line){
	char *temp = line;
	char *ptr;
	int numOfChar=0;
	int comma = 0;
	int space = 0;

	while(*line != EOF && *line != '\n'){
		if(*line == ' ' || *line == '\t'){
			space = 1;
			if(numOfChar==0)
				space = 0;
		}else
			if(*line == ','){
				if(comma == 0){
					comma = 1;
				}else{
					*temp = ',';
					temp++;
					space = 0;
				}
			}else{
				if(comma == 1){
					*temp = ',';
					temp++;
					comma = 0;
					space = 0;
				}else
					if(space == 1){/*---*/
						*temp = ' ';
						if(*line!='(' && *line!=')')
							temp++;
						else
							numOfChar=-1;
						space = 0;

					}
				if(*line == '\"'){
					*temp = *line;
					ptr=line;
					line++;
					temp++;
					while(*line!=EOF && *line!='\n'){
						if(*line=='\"')
							ptr=line;
						*temp = *line;
						line++;
						temp++;
					}
					line--;
					temp--;
					while(ptr!=line && (*line == ' ' || *line == '\t' )){
						line--;
						temp--;
					}

					line++;
					temp++;
					*temp = '\n';
					*line='\n';
					continue;
				}
				*temp = *line;
				temp++;
				numOfChar++;
			}
		line++;
	}
	if(comma == 1){
		*temp = ',';
		temp++;
	}
	*temp ='\n';
	temp++;
	*temp='\0';
	return;
}
int isLegalLabel(char *buf){
	int i=0;
	char temp[MAX_LABEL_NAME+1];

	if(!isalpha(buf[i])){
		fprintf(stderr,"Error in line %d: Label name %s are not starts with a letter\n", numLine,buf);
		return FALSE;
	}
	while(buf[i]!='(' && buf[i]!=',' && buf[i]!=')' && buf[i]!='\0' && buf[i]!=EOF){
		if(i>MAX_LABEL_NAME){
			fprintf(stderr,"Error in line %d: Label name '%s' are too long\n", numLine,buf);
			return FALSE;
		}
		if(!isalnum(buf[i])){
			fprintf(stderr,"Error in line %d: Label name '%s' contains a char which is not a digit nor a letter\n", numLine,buf);
			return FALSE;
		}
		temp[i]=buf[i];
		i++;
	}
	temp[i]='\0';
	if(isRegister(temp)>=0 || isTeachingStatement(temp)>=0 || isGuidanceStatement(temp)>=0){
		fprintf(stderr,"Error in line %d: Label name '%s' cannot be register,teaching or guidance name\n", numLine,temp);
		return FALSE;
	}
	return i;
}
int isRegister(char *buf){
	int i=0;
	while(reg[i]){
		if(strncmp(buf,reg[i],strlen(reg[i]))==0)
			if(!isalnum(buf[strlen(reg[i])]))
				return i;
		i++;
	}
	return ERROR;
}
int isTeachingStatement(char *buf){
	int i=0;
	while(opcode[i]){
		if(strncmp(buf,opcode[i],strlen(opcode[i]))==0)
			if(!isalnum(buf[strlen(opcode[i])]))
				return i;
		i++;
	}
	return ERROR;
}
int isGuidanceStatement(char *buf){
	int i=0;
	if(*buf=='.')
		buf++;
	while(guideLine[i]){
		if(strncmp(buf,guideLine[i],strlen(guideLine[i]))==0)
			if(!isalnum(buf[strlen(guideLine[i])]))
				return i;
		i++;
	}
	return ERROR;
}
int getTypeOpertion(int opcode){
	if(opcode<=3 || opcode==6)/*Instruction with two operands*/
		return 2;
	else
		if(opcode<=8 || opcode==11 || opcode==12)/*Instruction with one operand*/
			return 1;
		else
			if(opcode==9 || opcode==10 || opcode==13)/*Jumping instruction with or without parameters*/
				return 3;
	return 0;/*Instruction without operands*/
}
int getAddresMethod(char *buf){
	if(*buf=='#'){
		buf++;
		if(isNumber(buf)!=ERROR_NUMBER)
			return 0;
		return ERROR;
	}
	if(isRegister(buf)>=0)
		return 3;
	if(isLegalLabel(buf)>0)
		return 1;
	return ERROR;
}
int isNumber(char *buf){
	int i=0;
	int j=0;
	char tempNum[32];
	int num;
	if(!isdigit(buf[j]) && buf[j]!='+' && buf[j]!='-'){
		fprintf(stderr,"Error in line %d: Could not parse natural number in parameter '%s'\n", numLine,buf);
		return ERROR_NUMBER;
	}
	tempNum[i++]=buf[j];
	j++;
	while(isdigit(buf[j]))
		tempNum[i++]=buf[j++];
	if(buf[j]!=',' && buf[j]!=')' && buf[j]!='\0' && buf[j]!=EOF){
		fprintf(stderr,"Error in line %d: Could not parse natural number in parameter '%s'\n", numLine,buf);
		return ERROR_NUMBER;
	}
	tempNum[i]='\0';
	num=atoi(tempNum);
	if(num==ERROR_NUMBER)
		return num&NUMBER_OF_14_BIT;
	return num;
}
