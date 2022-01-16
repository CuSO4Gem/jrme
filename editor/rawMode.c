/*
	Author:Yashasvi Goel
	Code Refactored on 21/3/20
*/
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "editorControl.h"
#include "editorUtilities.h"
#include "editor_struct.h"
#include "cursorUtilities.h"

#include "rawMode.h"

void die(const char *s)//editorUtil
{
	moveCursor(KILLED);
 //	E.cx=0;
	clear();
	perror(s);
	exit(1);
}
void disableRawMode()
{
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&E.orig_termios)==-1)
		die("tcsetattr");
}
void enableRawMode()
{
	if(tcgetattr(STDIN_FILENO,&E.orig_termios)==-1)//fetch the terminal attr
		die("tcgetattr");
	atexit(disableRawMode);
	struct termios raw=E.orig_termios;

	raw.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);//local flags
	//to disable echo; no text appears on screen
	//ISIG diables ctl+C ctrl-z
	//IEXTEN disables ctl+v
	//ICANON disables canonical mode; input isn't altered anymore
	raw.c_iflag&=~(IXON|ICRNL|INPCK|ISTRIP|BRKINT);//input flags
	//IXON disables ctl+s ctl+q
	raw.c_oflag&= ~(OPOST);//OPOST disables all post-processing including carriage operator
	//Following are to edit config of read()
	raw.c_cc[VMIN]=0;//min chars after which read returns
	raw.c_cc[VTIME]=1;//max time read() waits before returning


	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1)//apply the attrs(file,delay,which one)
		die("tcsetattr");
}
void initEditor(){
	E.cx=0;
	E.cy=0;
	E.rx=0;
	E.rowOffset=0;
	E.colOffset=0;
	E.numRows=0;
	E.row=NULL;
	E.file=NULL;
	E.status[0]='\0';
	E.statusTime=0;
	E.syntax=NULL;
	E.dirty=0;

	if(getWindowSize(&E.screenRows,&E.screenColumns)==-1)
		die("Window");
	E.screenRows-=2;
}
int readKey()//reads input character-by-character
{
	int nread=0;
	char c='\0';
	while((nread=read(STDIN_FILENO,&c,1))!=1){
		if(nread==-1)
			die("read");
	}
	if(c == '\x1b'){
		char seq[3];
		if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
		if (seq[0] == '[')
		{
			if (seq[1] >= '0' && seq[1] <= '9')
			{
				if (read(STDIN_FILENO, &seq[2], 1) != 1)
					return '\x1b';
				if (seq[2] == '~')
				{
					switch (seq[1]){
						case '1': return HOME_KEY;
						case '3': return DEL_KEY;
						case '4': return END_KEY;
						case '5': return PAGE_UP;
						case '6': return PAGE_DOWN;
						case '7': return HOME_KEY;
						case '8': return END_KEY;
					}
				}
			}
			else{
				switch (seq[1]){
					case 'A': return ARROW_UP;
					case 'B': return ARROW_DOWN;
					case 'C': return ARROW_RIGHT;
					case 'D': return ARROW_LEFT;
					case 'H': return HOME_KEY;
					case 'F': return END_KEY;
				}
			}
		}
		else if (seq[0] == 'O'){
			switch (seq[1]){
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
			}
		}
		return '\x1b';
	}
	else{
		return c;
	}
}
void clear()
{
	editorScroll();
	strBuffer ab=str_INIT;
	bufAppend(&ab,"\x1b[?25l",6);
	E.cx=E.cy=0;
	bufAppend(&ab,"\x1b[2J",4);
	bufAppend(&ab,"\x1b[H",3);
	char buf[32];
	snprintf(buf,sizeof(buf),"\x1b[%d;%dH",E.cy -E.rowOffset+1,E.rx-E.colOffset+1);
	bufAppend(&ab,buf,strlen(buf));
	bufAppend(&ab,"\x1b[?25h",6);
	write(STDOUT_FILENO,ab.buffer,ab.len);
	bufFree(&ab);
}