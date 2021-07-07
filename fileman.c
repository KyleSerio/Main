/************************************************************************
 * 
 * UCSC Operating Systems Assignment
 * 
 * DESCRIPTION
 * Defines and manipulates a filesystem/files
 * 
 * This solution written by Kyle Serio.
 * Original assignment written 2020-2021 David C. Harrison.
 ************************************************************************/

#include "fileman.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

//Reads in and readies a file
int fileman_read(char *fname, size_t foffset, char *buf, size_t boffset, size_t size) 
{
	int desc = open(fname,O_RDWR);
	int fileSize;
	
	if(desc < 0)
	{
		return -1;
	}

	if(foffset > 0)
	{
		lseek(desc,foffset,SEEK_SET);
	}

	if(boffset > 0)
	{
		buf = buf + boffset;
	}
	
	fileSize = read(desc,buf,size);

	if(close(desc) < 0)
	{
		printf("CLOSE ERROR\n");
	}

	return fileSize;
}

//Write to a file
int fileman_write(char *fname, size_t foffset, char *buf, size_t boffset, size_t size) 
{
	int desc = open(fname,O_RDWR | O_CREAT | O_EXCL,00700);
	int fileSize;

	if(desc < 0)
	{
		return -1;
	}

	if(foffset > 0)
	{
		lseek(desc,foffset,SEEK_SET);
	}

	if(boffset > 0)
	{
		buf = buf + boffset;
	}

	fileSize = write(desc,buf,size);

	if(close(desc) < 0)
	{
		printf("CLOSE ERROR\n");
	}

	return fileSize;
}

//Append to the end of a given file
int fileman_append(char *fname, char *buf, size_t size) 
{
	int desc = open(fname,O_RDWR);
	int fileSize;

	if(desc < 0)
	{
		return -1;
	}

	lseek(desc,0,SEEK_END);
	fileSize = write(desc,buf,size);

	if(close(desc) < 0)
	{
		printf("CLOSE ERROR\n");
	}

	return fileSize;
}

//Makes a copy of a given file
int fileman_copy(char *fsrc, char *fdest) 
{
	
	int desc = open(fsrc,O_RDONLY);
	int newDesc = open(fdest, O_WRONLY | O_CREAT | O_EXCL, 00700);
	int size = lseek(desc,0,SEEK_END);
	lseek(desc,0,SEEK_SET);
	char *buf;

	if(size > 0)
	{
		buf = malloc(size);
	}

	if(desc < 0 || newDesc < 0 || buf == NULL)
	{
		return -1;
	}

	int fileSize = read(desc,buf,size);
	fileSize = write(newDesc,buf,size);

	if(close(desc) < 0 || close(newDesc) < 0)
	{
		printf("CLOSE ERROR\n");
	}

	free(buf);
	return fileSize;
}

//Recursively prints the entire directory structure
void printDir(char *newDir,int depth,int fd)
{
	struct dirent **dp;
	int size;
	int i = 0;
	char spaces[5] = "    ";
	size = scandir(newDir,&dp,NULL,alphasort);

	if(size == -1)
	{
		return;
	}

	while(i < size)
	{
		if((strcmp(dp[i]->d_name,".") != 0 && strcmp(dp[i]->d_name,"..") != 0))
		{
			char *test = malloc(100);
			char *out = malloc(100);
			memset(out, 0x00, 100);
			memset(test, 0x00, 100);
			
			for(int index = 0; index < depth; index++)
			{
				strcat(out,spaces);
			}
			
			strcat(out,dp[i]->d_name);
			strcat(out,"\n");
			write(fd, out, strlen(out));
			
			strcpy(test,newDir);
			strcat(test,"/");
			strcat(test,dp[i]->d_name);

			printDir(test,depth + 1,fd);

			free(test);
			free(out);
		}
		free(dp[i]);
		i++;
	}

	free(dp);
}

//Caller for recursive directory print function
void fileman_dir(int fd, char *dname) 
{
	
	int testSize = strlen("\n") + strlen(dname) + 1;
	char *test = malloc(testSize);
	memset(test, 0x00, testSize);
	test[0] = '\0';
	strcat(test,dname);
	strcat(test,"\n");

	write(fd, test, testSize - 1);
	printDir(dname,1,fd);

	free(test);
}

//Possible solution for the directory printing >>unused<<
void fileman_tree(int fd, char *dname) 
{
	int testSize = sizeof("\n") + sizeof(dname)/sizeof(dname[0]);
	char *test = malloc(testSize);
	memset(test, 0x00, testSize);
	strcat(test,dname);
	strcat(test,"\n");

	write(fd, test, testSize);

	printDir(dname,1,fd);

	free(test);
}

