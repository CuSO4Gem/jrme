/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#include <ctype.h>
#include "save.h"
#include "search.h"
#include "editor_struct.h"
#include "rawMode.h"
#include "editorControl.h"
#include "rowUtilities.h"
#include "cursorUtilities.h"
#include "editorUtilities.h"


void clearScreen(int options){
	editorScroll();

	strBuffer ab=str_INIT;
	bufAppend(&ab,"\x1b[?25l",6);
	if(options==0)
	{
		E.cx=E.cy=0;
		bufAppend(&ab,"\x1b[2J",4);
	}
	bufAppend(&ab,"\x1b[H",3);
	if(options==1)//default
	{
		drawTildes(&ab);
		drawStatusBar(&ab);
		drawMessageBar(&ab);
	}

	//	bufAppend(&ab,"\x1b[H",3);
	char buf[32];
	//	E.cx=12;
	snprintf(buf,sizeof(buf),"\x1b[%d;%dH",E.cy -E.rowOffset+1,E.rx-E.colOffset+1);
	bufAppend(&ab,buf,strlen(buf));
	bufAppend(&ab,"\x1b[?25h",6);
	write(STDOUT_FILENO,ab.buffer,ab.len);
	bufFree(&ab);
}
void drawStatusBar(strBuffer* ab){
	bufAppend(ab,"\x1b[7m",4);
	char status[80], rstatus[80];
	int len=0;
	//	char *file;
	//	if(E.file==NULL)
	//		file=strdup(&E.file);
	len=snprintf(status,sizeof(status),"%.20s -%d lines %s",E.file ? E.file : "NONE",E.numRows,E.dirty? "(modified)":"");
	//	else{
	////		*E.file='\0';
	//		len=snprintf(status,sizeof(status),"%.20s -%d lines",E.file,E.numRows);//TODO BUG HERE
	//	}
	int rlen = snprintf(rstatus, sizeof(rstatus), "%s|%d/%d",E.syntax? E.syntax->file:"no ft", E.cy + 1, E.numRows);
	if(len> E.screenColumns)
		len=E.screenColumns;
	bufAppend(ab,status,len);
	while(len<E.screenColumns){
		if(E.screenColumns-len==rlen){
			bufAppend(ab,rstatus,rlen);
			break;
		}
		else{
			bufAppend(ab," ",1);
			len++;
		}
	}
	bufAppend(ab,"\x1b[m",3);
	bufAppend(ab, "\r\n",2);
}
void drawMessageBar(strBuffer *ab) {
	bufAppend(ab, "\x1b[K", 3);
	int msglen = strlen(E.status);
	if (msglen > E.screenColumns)
		msglen = E.screenColumns;
	if (msglen && time(NULL) - E.statusTime < 5)
		bufAppend(ab, E.status, msglen);
}
void drawTildes(strBuffer* ab)//draws tildes
{
	int y;
	for(y=0;y<E.screenRows;y++)
	{
		int filerow=y+E.rowOffset;
		if(filerow>=E.numRows){
			if(E.numRows==0 && y==E.screenRows / 3){
				char welcome[80];
				int welcomelen = snprintf(welcome, sizeof(welcome),"Kilo editor by Yashasvi Goel -- version %s. improve by Zornlink", version);
				if(welcomelen > E.screenColumns)
					welcomelen = E.screenColumns;
				int padding =(E.screenColumns-welcomelen)/2;
				if(padding){
					bufAppend(ab,"~",1);
					padding--;
				}
				while(padding--)
					bufAppend(ab," ",1);
				bufAppend(ab, welcome, welcomelen);
			}
			else{
				bufAppend(ab,"~",1);
			}
		}
		else{
			int len = E.row[filerow].rsize - E.colOffset;
			if(len<0)
				len=0;
			if(len > E.screenColumns)
				len = E.screenColumns;
			char *c =&E.row[filerow].render[E.colOffset];
			unsigned char *hl=&E.row[filerow].hl[E.colOffset];
			int current_color=-1;
			int j;
			for(j=0;j<len;j++){
				if(hl[j]==HL_NORMAL){//isdigit(c[j]))
					if(current_color!=-1){
						bufAppend(ab,"\x1b[31m",5);
						current_color=-1;
					}
					bufAppend(ab,&c[j],1);	
				}
				else{
					int color=editorSyntaxToColor(hl[j]);
					if(color!=current_color){
						current_color=color;
						char buf[16];
						int clen=snprintf(buf,sizeof(buf),"\x1b[%dm",color);
						bufAppend(ab,buf,clen);
					}
					bufAppend(ab,&c[j],1);
				}
			}
			bufAppend(ab,"\x1b[39m",5);
			//			bufAppend(ab, &E.row[filerow].render[E.colOffset], len);
		}

		bufAppend(ab,"\x1b[K",3);
		//		write(STDOUT_FILENO,"~",1);
		//		if(y<E.screenRows-1)
		bufAppend(ab,"\r\n",2);
		//			write(STDOUT_FILENO,"\r\n",3);
	}
}
int is_separator(int c) {//crap free
  return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];{}", c) != NULL;
}
void editorUpdateSyntax(erow* row){
	row->hl=(unsigned char *)realloc(row->hl,row->rsize);
	memset(row->hl, HL_NORMAL,row->rsize);

	if(E.syntax==NULL)
		return;

	int prev_sep=1;
	// int in_String=0;

	int i=0;
	while(i<row->rsize){
		char c=row->render[i];
		unsigned char prev_hl=(i>0)? row->hl[i-1]:HL_NORMAL;

		if(E.syntax-> flags& HL_HIGHLIGHT_NMBERS){
			if((isdigit(c)&&(prev_sep||prev_hl == HL_NUMBER))||(c=='.'&& prev_hl==HL_NUMBER)){
				row->hl[i]=HL_NUMBER;
				i++;
				prev_sep=0;
				continue;
			}
		}

		prev_sep=is_separator(c);
		i++;
	}
}
int getWindowSize(int *rows, int *cols){//crap free
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0){
		if(write(STDOUT_FILENO,"\x1b[500C\x1b[600B",12)!= 12)
			return -1;
		return cursorPosition(rows,cols);
	}
	else{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}
void editorFromFile(char *filename){
	free(E.file);
	E.file=strdup(filename);


	FILE *fp=fopen(filename,"r");
	if(!fp)
		die("fopen");
	char *line=NULL;
	size_t linecap=0;
	ssize_t linelen;
	while((linelen = getline(&line, &linecap, fp))!=-1){
		while (linelen > 0 && (line[linelen - 1] == '\n' ||line[linelen - 1] == '\r'))
			linelen--;

		editorInsertRow(E.numRows,line,linelen);
		//		editorAppendRow(line,linelen);
	}
	free(line);
	fclose(fp);
	E.dirty=0;
}

void editorFromString(const char *initStr)
{
	const char *strBegin = initStr;
	size_t initStrLen = strlen(initStr);
	size_t lineLen=0;
	while (1)
	{
		if (strBegin[lineLen]=='\n' || strBegin[lineLen]=='\0')
		{
			size_t appendLen=lineLen;
			while (appendLen>0 && (strBegin[appendLen-1]=='\n' || strBegin[appendLen-1]=='\r'))
				appendLen--;
			editorInsertRow(E.numRows, strBegin, appendLen);
			if (strBegin[lineLen]=='\0')
				break;
			strBegin += lineLen+1;
			lineLen = 0;
		}
		lineLen++;
	}
	E.dirty=0;
}

void processKeypress(){//manages all the editor modes and special characters
	static int quit_times=KILO_QUIT_TIMES;
	int curr=readKey();
	switch(curr){//add all the mode controls below
		case '\r':
			editorInsertNewline();
			//			E.cy++;
			//			E.colOffset=E.cx=0;
			break;
		case ctrl('q'):
			if (E.dirty && quit_times > 0) {
				editorSetStatusMessage("WARNING!!! File has unsaved changes. "
						"Press Ctrl-Q %d more times to quit.", quit_times);
				quit_times--;
				return;
			}
			clearScreen(0);
			exit(0);
			break;
		case ctrl('f'):
			search();
			break;
		case HOME_KEY:
			E.cx=0;
			break;
		case END_KEY:
			if(E.cy<E.numRows)
				E.cx=E.row[E.cy].size;
			break;
		case PAGE_UP:
		case PAGE_DOWN:
			{
				if (curr == PAGE_UP) {
					E.cy = E.rowOffset;
				}
				else if(curr == PAGE_DOWN){
					E.cy = E.rowOffset + E.screenRows - 1;
					if (E.cy > E.numRows)
						E.cy = E.numRows;
				}
				int times = E.screenRows;
				while (times--)
					moveCursor(curr == PAGE_UP ? ARROW_UP : ARROW_DOWN);
			}
			break;
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			moveCursor(curr);
			break;
		case BACKSPACE:
		case ctrl('h'):
		case DEL_KEY:
			if(curr== DEL_KEY)
				moveCursor(ARROW_RIGHT);
			editorDelChar();
			break;

		case ctrl('l'):
		case '\x1b':
			break;
		case ctrl('s'):
			saveToFile();
			break;
		default:
			editorInsertChar(curr);
			break;
	}
	quit_times=KILO_QUIT_TIMES;
}

void editorInsertNewline(){
	if (E.cx == 0){
		editorInsertRow(E.cy, "", 0);
	}
	else{
		erow *row = &E.row[E.cy];
		editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
		row = &E.row[E.cy];
		row->size = E.cx;
		row->chars[row->size] = '\0';
		editorUpdateRow(row);
	}
	E.cy++;
	E.cx = 0;
}
char* editorPrompt(char* prompt, void(*callback)(char*,int)){
	size_t bufsize = 128;
	char *buf = (char *)malloc(bufsize);
	size_t buflen = 0;
	buf[0]='\0';

	while(1){
		editorSetStatusMessage(prompt,buf);
		clearScreen(1);
		int c =readKey();
		if(c==DEL_KEY|| c==ctrl('h')||c== BACKSPACE){
			if(buflen!=0)
				buf[--buflen]='\0';
		}
		else if(c=='\x1b'){
			editorSetStatusMessage("");
			if(callback)
				callback(buf,c);
			free(buf);
			return NULL;
		}
		else if(c=='\r'){
			if(buflen!=0){
				editorSetStatusMessage("");
				if(callback)
					callback(buf,c);
				return buf;
			}
		}
		else if(!iscntrl(c)&& c<128){//<128 ensures it is not a control char
			if(buflen==bufsize-1){
				bufsize*=2;
				buf=(char *)realloc(buf,bufsize);
			}
			buf[buflen++]=c;
			buf[buflen]= '\0';
		}

		if(callback)
			callback(buf,c);
	}
}
void editorDelChar() {
	if(E.cy == E.numRows) return;
	if(E.cx==0 && E.cy==0) return;
	erow *row = &E.row[E.cy];
	if (E.cx > 0) {
		editorRowDelChar(row, E.cx - 1);
		E.cx--;
	}
	else{
		E.cx=E.row[E.cy-1].size;
		editorRowAppendString(&E.row[E.cy-1],row->chars,row->size);
		delRow(E.cy);
		E.cy--;
	}
}
void editorInsertChar(int c){
	if(E.cy==E.numRows){
		editorInsertRow(E.numRows,"",0);
	}
	editorRowInsertChar(&E.row[E.cy],E.cx,c);
	E.cx++;
}

int editorSyntaxToColor(int hl) {
	switch (hl) {
		case HL_STRING: return 35;
		case HL_NUMBER: return 31;
		case HL_MATCH: return 34;
		default: return 37;
	}
}