#ifndef ROW_UTILITIES_H
#define ROW_UTILITIES_H

#define TAB_STOP 8

void editorRowAppendString(erow*,char*,size_t);
void editorUpdateSyntax(erow* );
void editorInsertRow(int ,const char * ,size_t );
void editorFreeRow(erow* );
void delRow(int );
void editorRowInsertChar(erow* row,int at,int c);
char *editorRowsToString(int *buflen);
void editorRowDelChar(erow *row, int at);
void editorUpdateRow(erow *row);

#endif