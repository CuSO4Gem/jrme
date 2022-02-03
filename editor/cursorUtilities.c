/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#include <unistd.h> 
#include <stdio.h>
#include "cursorUtilities.h"
#include "editorUtilities.h"
#include "editor_struct.h"

struct editorConfig E;

int cursorPosition(int* rows,int* cols)
{
	char buf[32];
	unsigned int i=0;
	if(write(STDOUT_FILENO,"\x1b[6n",4)!=4)
		return -1;
	while(i<sizeof(buf) -1){
		if(read(STDIN_FILENO,&buf[i],1)==1) break;
		if(buf[i]=='R') break;
		i++;
	}
	buf[i]='\0';
	if(buf[0]!='\x1b' || buf[1]!='[') return -1;
	if(sscanf(&buf[2], "%d;%d",rows,cols)!=2) return -1;
	return 0;
}

void moveCursor(int key){
	erow *row=(E.cy >= E.numRows) ? NULL : &E.row[E.cy];
	switch (key) {
		case ARROW_LEFT:
			if(E.cx!=0)
				E.cx--;
			else if(E.cy>0){
				E.cy--;
				E.cx=E.row[E.cy].size;
			}
			break;
		case ARROW_RIGHT:
			if(row&& E.cx<row->size)
				E.cx++;
			// else if(row && E.cx==row->size){
			// 	E.cy++;
			// 	E.cx=0;
			// }
			break;
		case ARROW_UP:
			if(E.cy!=0)
				E.cy--;
			break;
		case ARROW_DOWN:
			if(E.cy<E.numRows-1)
				E.cy++;
			break;
		case KILLED:
			E.cy=0;
			E.cx=0;
			break;
	}
	row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if (E.cx > rowlen) {
		//		E.cy++;
		E.cx = rowlen;
	}
}