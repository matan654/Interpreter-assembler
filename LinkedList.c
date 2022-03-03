#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataType.h"
#include "LinkedList.h"


int addLabelToList(lptr *labelPtr,char *label,int ext,int ent,int isInTeach,int address){
	lptr p1,p2;
	lptr t;

	t = (lptr) malloc(sizeof(labelNode));
	if(!t){
		fprintf(stderr,"Error in line %d: Space allocation error",numLine);
		return FALSE;
	}
	p1 = *labelPtr;
	strcpy(t->data.labelName,label);
	t->data.addres=address;
	t->data.isExtern=ext;
	t->data.isEntern=ent;
	t->data.isInTeachLine=isInTeach;
	t->next=NULL;
	if(!p1)
		*labelPtr = t;
	else{
		while (p1){
			if(!strcmp(p1->data.labelName,t->data.labelName)){
				fprintf(stderr,"Error in line %d: Label %s already exist in labels table, action aborted\n",numLine, t->data.labelName);
				free(t);
				return FALSE;
			}
			p2 = p1;
			p1 = p1->next;
		}
		p2->next = t;
	}
	return TRUE;
}
void updateLabelsAdress(lptr *labelPtr,int address){
	lptr p;
	p = *labelPtr;

	while(p){
		if(p->data.isExtern==0 && p->data.isInTeachLine==0)
			p->data.addres+=address;
		p = p->next;
	}
	return;
}
int setEnteryLabel(lptr *labelPtr,char *label){
	lptr p;
	p = *labelPtr;

	while(p){
		if(strcmp(p->data.labelName,label)==0){
			if(p->data.isExtern==0){
				p->data.isEntern=1;
				return TRUE;
			}
			fprintf(stderr,"Error in line %d: Entry label %s was defined as external,it cannot be defined as internal too.\n", numLine, label);
			return FALSE;
		}
		p=p->next;
	}
	fprintf(stderr,"Error in line %d: Entry label %s was not use, it can not be defined as internal.\n", numLine, label);
	return FALSE;
}
labelData *searchLabel(lptr *labelPtr,char *label){
	lptr p;
	p = *labelPtr;

	while(p!=NULL){
		if(strcmp(p->data.labelName,label)==0)
			return &(p->data);
		p=p->next;
	}
	return NULL;
}
int addLabelToExtList(lptr *extLabelsPtr,char *label,int address){
	lptr p1,p2;
	lptr t;

	t = (lptr) malloc(sizeof(labelNode));
	if(!t){
		fprintf(stderr,"Error in line %d: Space allocation error",numLine);
		return FALSE;
	}
	p1 = *extLabelsPtr;
	strcpy(t->data.labelName,label);
	t->data.addres=address;
	t->data.isExtern=0;
	t->data.isEntern=0;
	t->data.isInTeachLine=0;
	t->next=NULL;
	if(!p1)
		*extLabelsPtr = t;
	else{
		while(p1){
			p2 = p1;
			p1 = p1->next;
		}
		p2->next = t;
	}
	return TRUE;
}
int isExtOprnd(lptr *labelPtr,char *label){
	if(searchLabel(labelPtr,label)->isExtern)
		return TRUE;
	return FALSE;
}
void freeLabelList(lptr *labelPtr){
	lptr p;
	while(*labelPtr){
		p=*labelPtr;
		*labelPtr=p->next;
		free(p);
	}
	return;
}
