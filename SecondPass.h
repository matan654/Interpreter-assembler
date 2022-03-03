int secondPass(FILE *fd,lptr *labelPtr,lptr *extLabelsPtr);/*
*	this function contains all the logic of the second pass over the assembly program.
*	it uses the following parameters:
*			-pointer to source code file
*			-pointer to label linked list
*			-pointer to eternal label linked list*/
int setDirectAddress(char *buf,lptr *labelPtr);/*
*	get oprnd from type direct addres method and update the label address of this label.
*	expects: pointer to label name string, pointer to label list.
*	return 1 on success, 0 on failure.*/
int setLabelAddres(lptr *labelPtr,char *label);/*
*	update the label address of current label in the code table
*	and define the lable as external or entrnal.
*	return 1 on success, 0 on failure.*/
