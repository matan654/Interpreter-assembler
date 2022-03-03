


int addLabelToList(lptr *labelPtr,char *label,int ext,int ent,int isInTeach,int address);/*
* 	creates a new labelNode and adds to the end of the list.
*	expects: pointer to label list head pointer
*	pointer to label name string,
*	3 prameter: flag for extrnal prameter
*				flag for entrnal prameter
*				flag for teaching line.
*	and the label value
*	returns 1 on success, 0 on failure.*/
void updateLabelsAdress(lptr *labelPtr,int address);/*
*	goes over the label list and adds for each DATA label the final address of
*	IC counter after the first pass
*	expects pointer to label list */
int setEnteryLabel(lptr *labelPtr,char *label);/*
*	when an entry label is found in the second pass,
*	this function will search for it in the label list
*	and mark it as an antry label
*	expects a pointer to the label list and a pointer to the label name.
*	returns 1 on success, 0 on failure*/
labelData *searchLabel(lptr *labelPtr,char *label);/*
*	search for a label of a given name
*	expects a pointer to the label list and a pointer to the label name
*	returns pointer to labelData or NULL if not found*/
int addLabelToExtList(lptr *extLabelsPtr,char *label,int address);/*
*	creates a new labelNode and adds to the end of the list.
*	use only for extrnals lables is found in the second pass,
* 	expects: pointer to label list head pointer
*	pointer to label name string,
*	and the label value
*	returns 1 on success, 0 on failure.*/
int isExtOprnd(lptr *labelPtr,char *label);/*
*	check if label in the label list is extrnal,
*	expects a pointer to the label list and a pointer to the label name
*	returns 1 if the label is external, 0 if not.*/
void freeLabelList(lptr *labelPtr);/*
*	free memory allocation for label list.
*	expects pointer to head node*/
