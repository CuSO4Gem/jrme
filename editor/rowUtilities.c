
#include <stdlib.h>
#include <string.h>

#include "editor_struct.h"
#include "editorUtilities.h"
#include "rowUtilities.h"
 
void editorRowAppendString(erow* row,char*s,size_t len){//append 's' to row
	row->chars=(char *)realloc(row->chars,row->size +len +1);
	memcpy(&row->chars[row->size],s,len);
	row->size +=len;
	row->chars[row->size]='\0';
	editorUpdateRow(row);
	E.dirty++;
}


void editorInsertRow(int at,const char * line,size_t linelen){
	if(at<0|| at>E.numRows)
		return;
	//void editorAppendRow(char *line,size_t linelen){

	E.row=(erow *)realloc(E.row,sizeof(erow)*(E.numRows+1));
	memmove(&E.row[at+1],&E.row[at],sizeof(erow)*(E.numRows-at));

	//	int at=E.numRows;
	E.row[at].size=linelen;
	E.row[at].chars=(char *)malloc(linelen+1);
	memcpy(E.row[at].chars, line ,linelen);
	E.row[at].chars[linelen]='\0';

	E.row[at].rsize=0;
	E.row[at].render=NULL;
	E.row[at].hl=NULL;
	editorUpdateRow(&E.row[at]);

	E.numRows++;
	E.dirty++;
}
void editorFreeRow(erow* row){
	free(row->render);
	free(row->chars);
	free(row->hl);
}
void delRow(int at){
	if(at<0 || at>= E.numRows)
		return;
	editorFreeRow(&E.row[at]);
	memmove(&E.row[at],&E.row[at+1],sizeof(erow)*(E.numRows-at -1));
	E.numRows--;
	E.dirty++;
}
void editorRowInsertChar(erow* row,int at,int c){//insert a single char 'c' at 'at'th posn in row
	if(at<0||at> row->size)
		at=row->size;
	row->chars=(char *)realloc(row->chars,row->size+2);//for the new char and the null byte
	memmove(&row->chars[at+1],&row->chars[at],row->size - at+ 1);
	row->size++;
	row->chars[at]=c;
	editorUpdateRow(row);
	E.dirty++;
}
char *editorRowsToString(int *buflen){

	int total=0,j;
	for(j=0;j<E.numRows;j++)
		total+=E.row[j].size+1;

	*buflen=total;

	char *buf=(char *)malloc(total*sizeof(char));
	char *str=buf;

	for(j=0;j<E.numRows;j++)
	{
		memcpy(str,E.row[j].chars,E.row[j].size);
		str+=E.row[j].size;
		*str='\n';
		str++;
	}
	return buf;
}
void editorRowDelChar(erow *row, int at) {
	if (at < 0 || at >= row->size)
		return;
	memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
	row->size--;
	editorUpdateRow(row);
	E.dirty++;
}
void editorUpdateRow(erow *row){
	int tabs=0;
	int j;
	for(j=0;j<row->size;j++)
		if(row->chars[j]=='\t')
			tabs++;
	free(row->render);
	row->render=(char *)malloc(row->size+tabs*(TAB_STOP-1) +1);

	int idx=0;
	for(j=0;j<row->size;j++){
		if(row->chars[j]=='\t'){
			row->render[idx++]=' ';
			while(idx% TAB_STOP !=0)
				row->render[idx++]=' ';
		}
		else{
			row->render[idx++]=row->chars[j];
		}
	}
	row->render[idx]='\0';
	row->rsize=idx;

	editorUpdateSyntax(row);
}