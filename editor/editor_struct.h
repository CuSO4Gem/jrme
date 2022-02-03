#ifndef EDITOR_STRUCH_H
#define EDITOR_STRUCH_H

#include <sys/ioctl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define HL_HIGHLIGHT_NMBERS (1<<0)
#define KILO_QUIT_TIMES 3
#define HL_HIGHLIGHT_STRINGS (1<<1)
#define version "0.0.1"
#define str_INIT {NULL,0}
#define ctrl(k) ((k) & 0x1f)

enum editorKey {
	BACKSPACE =127,
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN,
	KILLED=0
};

enum editorHighlight{
	HL_NORMAL=0,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH
};

struct editorSyntax{
	char *file;
	char **filematch;
	int flags;
};
typedef struct erow {
	int size;
	int rsize;
	char *chars;
	char *render;
	unsigned char *hl;
} erow;

typedef struct strBuffer{
	char *buffer;
	int len;
}strBuffer;
typedef struct editorConfig{
	int cx,cy;//cx is posn in the line,without indentation
	int rx;//actual posn in line
	int rowOffset,colOffset;
	int screenRows;
	int screenColumns;
	char *file;
	int numRows;
	int dirty;
	erow *row;
	char status[80];
	time_t statusTime;
	struct editorSyntax *syntax;
	struct termios orig_termios;
}editorConfig;
extern struct editorConfig E;

#endif