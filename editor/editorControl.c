/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#include "editorControl.h"
#include "editor_struct.h"
#include "rowUtilities.h"

int editorRowCxToRx(erow *row, int cx) {//TODO  
	int rx = 0;
	int j;
	for (j = 0; j < cx; j++) {
		if (row->chars[j] == '\t')
			rx += (TAB_STOP - 1) - (rx % TAB_STOP);
		rx++;
	}
	return rx;
}
int editorRowRxToCx(erow *row, int rx) {
	int cur_rx = 0;
	int cx;
	for (cx = 0; cx < row->size; cx++) {
		if (row->chars[cx] == '\t')
			cur_rx += (TAB_STOP - 1) - (cur_rx % TAB_STOP);
		cur_rx++;
		if (cur_rx > rx) return cx;
	}
	return cx;
}
void editorSetStatusMessage(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);//initializes ap
	vsnprintf(E.status, sizeof(E.status), fmt, ap);//prints var arguments
	va_end(ap);//analog of free() for va_list
	E.statusTime = time(NULL);
}
void editorScroll(){
	E.rx=0;
	if(E.cy<E.numRows){
		E.rx=editorRowCxToRx(&E.row[E.cy],E.cx);
	}
	if(E.cy<E.rowOffset){
		E.rowOffset=E.cy;
	}
	if(E.cy>=E.rowOffset+E.screenRows){
		E.rowOffset=E.cy- E.screenRows+1;
	}
	if(E.rx<E.colOffset){
		E.colOffset=E.rx;
	}
	if(E.rx>=E.colOffset+E.screenColumns){
		E.colOffset=E.rx - E.screenColumns+1;
	}
}
void bufFree(strBuffer *var){//crap free
	free(var->buffer);
}
void bufAppend(strBuffer* ab,const char *s,int len){//crap free
	char *newPtr=(char *)realloc(ab->buffer,ab->len+len);
	if(newPtr==NULL)
		return;
	memcpy(&newPtr[ab->len],s,len);
	ab->buffer=newPtr;
	ab->len+=len;
}