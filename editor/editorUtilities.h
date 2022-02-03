/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#ifndef EDITOR_UTILITIES_H
#define EDITOR_UTILITIES_H

#include "editor_struct.h"

void clearScreen(int options);

void drawTildes(strBuffer* ab);
void drawStatusBar(strBuffer* ab);
void drawMessageBar(strBuffer *ab);
int is_separator(int c);
void editorUpdateSyntax(erow* row);
void moveCursor(int);
int getWindowSize(int *rows, int *cols);


void editorFromFile(char *filename);
void editorFromString(const char *initStr);

int editorSyntaxToColor(int hl);
char* editorPrompt(char* prompt, void(*callback)(char*,int));
void processKeypress();
void editorDelChar();
void editorInsertNewline();
void editorDelChar();
void editorInsertChar(int c);

#endif