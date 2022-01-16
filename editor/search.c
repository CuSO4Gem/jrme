#include <string.h>
#include <stdlib.h>

#include "editorControl.h"
#include "editor_struct.h"
#include "editorUtilities.h"
#include "rowUtilities.h"

#include "search.h"

void search(){
	int a,s,d,f;
	a=E.cx;
	s=E.cy;
	d=E.colOffset;
	f=E.rowOffset;
	char *query=editorPrompt(DISPLAYED_PROMPT,searchCallback);
	if(query)
		free(query);
	else{
		E.cx=a;
		E.cy=s;
		E.colOffset=d;
		E.rowOffset=f;
	}
}
void searchCallback(char* query,int key){
	static int last_match=-1;
	static int direction=1;

	static int saved_hl_line;
	static char* saved_hl=NULL;

	if(saved_hl){
		memcpy(E.row[saved_hl_line].hl,saved_hl,E.row[saved_hl_line].rsize);
		free(saved_hl);
		saved_hl=NULL;
	}
	if(key=='\r'||key=='\x1b'){
		last_match=-1;
		direction=1;
		return;
	}
	else if(key==ARROW_RIGHT || key==ARROW_DOWN){
		direction=1;
	}
	else if(key==ARROW_LEFT || key==ARROW_UP){
		direction=-1;
	}
	else{
		last_match=-1;
		direction=1;
	}

	if(last_match==-1)
		direction=1;
	int current=last_match;
	int i;
	for(i=0;i<E.numRows;i++){
		current+=direction;
		if(current==-1)
			current=E.numRows-1;
		else if(current==E.numRows)
			current=0;
		erow* row=&E.row[current];
		char *match=strstr(row->render,query);
		if(match){
			last_match=current;
			E.cy=current;
			E.cx=editorRowRxToCx(row,match - row->render);
			E.rowOffset=E.numRows;

			saved_hl_line=current;
			saved_hl=(char *)malloc(row->rsize);
			memcpy(saved_hl,row->hl,row->rsize);
			memset(&row->hl[match- row->render],HL_MATCH,strlen(query));
			break;
		}
	}
}