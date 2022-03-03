#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataType.h"
#include "AuxFunctions.h"
#include "LinkedList.h"
#include "SecondPass.h"

int secondPass(FILE *fd,lptr *labelPtr,lptr *extLabelsPtr){
	int type=0;
	char *buf;
	char line[MAX_LINE];
	char label[MAX_LABEL_NAME+1];
	int numOfOprnds;
	int addresMethod;
	int prameterFlag;
	int currentIC;
	int len;
	char *tok1;
	char *tok2;
	char *tok3;
	int errorFlag=0;
	int regFlag;
	IC=0;
	numLine=0;

	while(!(getNextLine(fd,line))){
		removeRedundantSpaces(line);
		numLine++;
		currentIC=IC;
		if((IC+DC)>MAX_PROGRAM_SIZE){
					fprintf(stderr,"Error from line %d: The program exceeds the maximum memory size of the processor\n",numLine);
					errorFlag=1;
					return FALSE;
				}

		buf=line;
		if(*buf=='\n' || *buf==EOF)
			continue;
		if(line[0]==';')
			continue;
		buf=strtok(line," \n");
		if(buf[strlen(buf)-1]==':'){/*Is there a definition of label? It should be skipped*/
			buf=strtok(NULL," \n");
			if(!buf)
				continue;
		}
		type=(*buf=='.')?isGuidanceStatement(buf):ERROR;
		if(type>=0){
			if(type==ENTRY){
				buf=strtok(NULL,"\n");
				len=isLegalLabel(buf);
				strncpy(label,buf,len);
				label[len]='\0';
				if(!setEnteryLabel(labelPtr,label))
					errorFlag=1;
			}
			continue;
		}
		type=isTeachingStatement(buf);
		IC++;
		if(type>=0){
			numOfOprnds=getTypeOpertion(type);
			switch(numOfOprnds){
			case TEACH_WITHOUT_OPRAND:
				continue;
			case TEACH_WITH_ONE_OPRAND:
				buf=strtok(NULL,"\n");
				addresMethod=getAddresMethod(buf);
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				IC++;
				continue;
			case TEACH_WITH_TWO_OPRAND:
				buf=strtok(NULL,",");
				addresMethod=getAddresMethod(buf);
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				regFlag=(addresMethod==REG_ADDRESS)?1:0;
				buf=strtok(NULL,"\n");
				addresMethod=getAddresMethod(buf);
				if(regFlag!=1 || addresMethod!=REG_ADDRESS)
					IC++;
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				IC++;
				continue;
			case TEACH_WITH_CONDITION:
				tok1=strtok(NULL,"(\n");
				tok2=strtok(NULL,",");
				tok3=strtok(NULL,")");
				buf=strtok(NULL,"\n");
				if(tok1==NULL || tok2==NULL || tok3==NULL){
					if(tok1!=NULL && tok2==NULL && tok3==NULL && buf==NULL)
						prameterFlag=0;
					else{
						fprintf(stderr,"Error in line %d: Missing parameters\n",numLine);
						break;
					}
				}else
					prameterFlag=1;
				buf=tok1;
				addresMethod=getAddresMethod(buf);
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				IC++;
				if(!prameterFlag)
					continue;
				buf=tok2;
				addresMethod=getAddresMethod(buf);
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				regFlag=addresMethod==REG_ADDRESS?1:0;
				buf=tok3;
				addresMethod=getAddresMethod(buf);
				if(regFlag!=1 || addresMethod!=REG_ADDRESS)
					IC++;
				if(addresMethod==DIRECT_ADDRESS){
					if(!searchLabel(labelPtr,buf)){
						fprintf(stderr,"Error in line %d: Label %s dosent exist in the file.\n",numLine,buf);
						break;
					}
					if(isExtOprnd(labelPtr,buf))
						if(!addLabelToExtList(extLabelsPtr,buf,IC+IC_OFFSET))
							break;
					if(!setDirectAddress(buf,labelPtr))
						break;
				}
				IC++;
				continue;
			}
			errorFlag=1;
			IC=currentIC;
			continue;
		}
	}
	rewind(fd);
	if(!errorFlag)
		return TRUE;
	return FALSE;
}
int setDirectAddress(char *buf,lptr *labelPtr){
	char tempLabel[MAX_LABEL_NAME];
	int len;

	len=isLegalLabel(buf);
	strncpy(tempLabel,buf,len);
	tempLabel[len]='\0';
	if(!setLabelAddres(labelPtr,tempLabel))
		return FALSE;
	return TRUE;
}
int setLabelAddres(lptr *labelPtr,char *label){
	labelData *tempLabel;

	tempLabel=searchLabel(labelPtr,label);
	if(!tempLabel){
		fprintf(stderr,"Error in line %d: The label '%s' was not set during the program and can not be used\n",numLine,label);
		return FALSE;
	}
	if(!(tempLabel->isExtern)){
		codeTable[IC].addressEncoding.addres=tempLabel->addres;
		codeTable[IC].addressEncoding.ARE=RELOCATABLE;
	}else
		codeTable[IC].addressEncoding.ARE=EXTERNAL;
	return TRUE;
}
