#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataType.h"
#include "LinkedList.h"
#include "FirstPass.h"
#include "SecondPass.h"

void printBin(FILE *fd,wordOf14Bits code,int adress);/*DELETE*/


int FileOpening(char * ,FILE **);/*
*	open new file and check it is dosen't empty.
*	expects pointer to a file nmae string, and pointer to file pointer.
*	return 1 if success, otherwise 0.*/
int createExternalFile(char *fileName, lptr *extLabelsPtr);/*
*	Creates a new file of all the operands that are in the labels list.
*	expects pointer to file name and pointer to label list.
*	return 1 if success, otherwise 0.*/
int createInternalFile(char *fileName, lptr *labelPtr);/*
 *	Creates a new file of all the operands that are in the labels list and are enternal oprands.
 *	expects pointer to file name and pointer to label list.
 *	return 1 if success, otherwise 0.*/
int createObjectFile(char *fileName);/*
*	creates object file with all the data.
*	expects pointer to filename.
*	return 1 if success, otherwise 0.*/
void FreeAllMemory(lptr *labelPtr,lptr *extLabelsPtr,FILE **fd);/*
*	free memory allocation for all labels list and close the open file.
*	expects 2 pointers to label list and pointer to file pointer.
*	return 1 if success, otherwise 0.*/
void printBinBizarre(FILE *fd,wordOf14Bits code,int adress);/*
*	print data of encoder base 2 bizarre 1='\'	0='.'
*	expects pointer to file word of 14 bits the address of the code.*/

int main(int argc, char **argv){/*
*	The main function is used to handle the assembly source files.
*	it receives the source files names as command prompt parameters via argv (files names parameters should not contain a suffix).
*	it will iterate over the files and for each file it will run the first and second passes.
*	if no errors were found it will then create the object file and entries/externals files if needed.*/
	FILE *fd=NULL;
	lptr labelPtr;
	lptr extLabelsPtr;
	labelPtr=NULL;
	extLabelsPtr=NULL;
	argc--;
	if(!argc){
		printf("Error: You must enter a filename after the program name\n");
		return FALSE;
	}
		

	while(argc--){/*there is a file*/

		argv++;

		printf("Starting assembly for source file %s.as , Output file will be writen in the source file folder.\n", *argv);
		if(!FileOpening(*argv,&fd))	/*the file can't be opening*/
			continue;
		if(!FirstPass(fd,&labelPtr)){
			printf("First pass failed, aborting assembly for current file\n\n");
			FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
			continue;
		}

		{
			int i;
			fprintf(stderr,"\n");
			for(i=0;i<IC;i++)
				printBin(fd,codeTable[i],i+IC_OFFSET);
		}

		if(!secondPass(fd,&labelPtr,&extLabelsPtr)){
			printf("Second pass failed, aborting assembly for current file\n\n");
			FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
			continue;
		}
		if(!createExternalFile(*argv,&extLabelsPtr)){
			printf("create externals file failed, aborting assembly for current file\n\n");
			FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
			continue;
		}
		if(!createInternalFile(*argv,&labelPtr)){
			printf("create internal file failed, aborting assembly for current file\n\n");
			FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
			continue;
		}
		if(!createObjectFile(*argv)){
			printf("create object file failed, aborting assembly for current file\n\n");
			FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
			continue;
		}
		FreeAllMemory(&labelPtr,&extLabelsPtr,&fd);
		printf("Assembly for source file %s.as completed successfully\n\n", *argv);
	}
	return TRUE;
}
int FileOpening(char *fileName,FILE **fd){
	char tempFileName[MAX_FILE_NAME];
	strcpy(tempFileName,fileName);
	strcat(tempFileName,SURFFIX_ASM);
	if(!(*fd=fopen(tempFileName,"r"))){		/*Check if the file can be open*/
		fprintf(stderr,"Cannot open file %s!\n", tempFileName);
		return FALSE;
	}
	fseek(*fd,0,SEEK_END);
	if(0==ftell(*fd)){
		fprintf(stderr,"The file %s is empty!\n", tempFileName);
		return FALSE;
	}
	rewind(*fd);
	return TRUE;
}
int createExternalFile(char *fileName, lptr *extLabelsPtr){
	FILE *fd;
	lptr p;
	char tempFileName[MAX_FILE_NAME];

	p = *extLabelsPtr;
	if(!p)
		return TRUE;
	strcpy(tempFileName,fileName);
	strcat(tempFileName,SURFFIX_EXT);
	fd=fopen(tempFileName, "w");
	if(!fd){
		fprintf(stderr,"File %s creation failed\n",tempFileName);
		return FALSE;
	}
	while(p){
		fprintf(fd,"%s	%d\n",p->data.labelName,p->data.addres);
		p = p->next;
	}
	return TRUE;
}
int createInternalFile(char *fileName, lptr *labelPtr){
	FILE *fd;
	lptr p;
	char tempFileName[MAX_FILE_NAME];

	strcpy(tempFileName,fileName);
	strcat(tempFileName,SURFFIX_ENT);
	p = *labelPtr;
	while(p && !p->data.isEntern)
		p=p->next;
	if(!p)
		return TRUE;
	fd=fopen(tempFileName, "w");
	if(!fd){
		fprintf(stderr,"File %s creation failed\n",tempFileName);
		return FALSE;
	}
	while(p){
		if(p->data.isEntern)
			fprintf(fd,"%s	%d\n",p->data.labelName,p->data.addres);
		p=p->next;
	}
	return TRUE;
}
int createObjectFile(char *fileName){
	FILE *fd;
	int i;
	char tempFileName[MAX_FILE_NAME];
	strcpy(tempFileName,fileName);
	strcat(tempFileName,SURFFIX_OBJ);
	fd=fopen(tempFileName, "w");
	if(!fd){
		fprintf(stderr,"File %s creation failed\n",tempFileName);
		return FALSE;
	}
	fprintf(fd,"%d	%d\n",IC,DC);
	for(i=0;i<IC+DC;i++)
		printBinBizarre(fd,codeTable[i],i+IC_OFFSET);
	return TRUE;
}
void FreeAllMemory(lptr *labelPtr,lptr *extLabelsPtr,FILE **fd){
	freeLabelList(labelPtr);
	freeLabelList(extLabelsPtr);
	fclose(*fd);
	return;
}
void printBinBizarre(FILE *fd,wordOf14Bits code,int adress){
	wordOf14Bits mask;

	mask.dataEncoding.data =1<<13;
	fprintf(fd,"%d	",adress);
	while(mask.dataEncoding.data){
		if((code.dataEncoding.data&mask.dataEncoding.data)==0)
			fprintf(fd,".");
		else
			fprintf(fd,"/");
		mask.dataEncoding.data>>=1;
	}
	fprintf(fd,"\n");
	return;
}

void printBin(FILE *fd,wordOf14Bits code,int adress){
	wordOf14Bits mask;

	mask.dataEncoding.data =1<<13;
	fprintf(stderr,"%d	",adress);
	while(mask.dataEncoding.data){
		if((code.dataEncoding.data&mask.dataEncoding.data)==0)
			fprintf(stderr,"0");
		else
			fprintf(stderr,"1");
		mask.dataEncoding.data>>=1;
	}
	fprintf(stderr,"\n");
	return;
}
