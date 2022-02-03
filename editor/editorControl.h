/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#ifndef EDITOR_CONTROL_H
#define EDITOR_CONTROL_H

#include "editor_struct.h"

int editorRowRxToCx(erow *, int );
int editorRowCxToRx(erow *, int );
void editorScroll();
void bufFree(strBuffer *var);
void bufAppend(strBuffer* ab,const char *s,int len);

void editorSetStatusMessage(const char *fmt, ...);
#endif