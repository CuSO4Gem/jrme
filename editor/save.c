#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "editor_struct.h"
#include "editorControl.h"
#include "editorUtilities.h"
#include "rowUtilities.h"
#include "save.h"

void saveToFile(){//TODO probably with the automatic cursor movement
	if(E.file==NULL){
		E.file=editorPrompt("Save as: %s(ESC to cancel)",NULL);
		if(E.file==NULL){
			editorSetStatusMessage("Save Aborted");
			return;
		}
	}
	int len;
	char *text=editorRowsToString(&len);
	int fd=open(E.file, O_RDWR|O_CREAT, 0644);
	//O_RDWR: Read and write
	//O_CREAT: creat if it doesn't exist //0644 defines permissions for the new file
	if(fd!=-1){
		if(ftruncate(fd,len)!=-1){
			if(write(fd,text,len)==len){
				close(fd);
				free(text);
				E.dirty=0;
				editorSetStatusMessage("%d bytes written to disk",len);
				return;
			}
		}
		close(fd);
	}
	free(text);
	//	editorSetStatusMessage("Can't save! I/O error: %s",strerror(errno));
}