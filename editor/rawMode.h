/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#ifndef RAWMODE_H
#define RAWMODE_H

void die(const char *s);
int readKey();

void disableRawMode();
void enableRawMode();
void initEditor();
void clear();

// typedef struct editorConfig{
// 	int cx,cy;//cx is posn in the line,without indentation
// 	int rx;//actual posn in line
// 	int rowOffset,colOffset;
// 	int screenRows;
// 	int screenColumns;
// 	char *file;
// 	int numRows;
// 	int dirty;
// 	erow *row;
// 	char status[80];
// 	time_t statusTime;
// 	struct editorSyntax *syntax;
// 	struct termios orig_termios;
// }editorConfig;

#endif