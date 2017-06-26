/*************************************************
	* C program to count no of lines, words and 	 *
	* characters in a file.			 *
	*************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
//#define FILEPATH "/Users/ruoyancai/Desktop/Project1/books"
#define FILEPATH "/tmp/CSCI4730/books"

typedef struct count_t {
		int linecount;
		int wordcount;
		int charcount;
} count_t;

count_t word_count(char *file)
{
		FILE *fp;
		char ch;

		count_t count;
		// Initialize counter variables
		count.linecount = 0;
		count.wordcount = 0;
		count.charcount = 0;

		// Open file in read-only mode
		fp = fopen(file, "r");

		// If file opened successfully, then write the string to file
		if ( fp )
		{
				//Repeat until End Of File character is reached.	
				while ((ch=getc(fp)) != EOF) {
						// Increment character count if NOT new line or space
						if (ch != ' ' && ch != '\n') { ++count.charcount; }

						// Increment word count if new line or space character
						if (ch == ' ' || ch == '\n') { ++count.wordcount; }

						// Increment line count if new line character
						if (ch == '\n') { ++count.linecount; }
				}

				if (count.charcount > 0) {
						++count.linecount;
						++count.wordcount;
				}

				fclose(fp);
		}
		else
		{
				printf("Failed to open the file: %s\n", file);
		}
		
		return count;
}

#define MAX_PROC 100

int main(int argc, char **argv)
{
		int i, numFiles,status;
		char filename[100];
		count_t count, tmp, count_total;
		count_t *count_point, *count_point2;
		int pipe_fd[2];
		pid_t pid;
		int returnStatues;

		count_point = malloc(sizeof(count_t));
		count_point2 = malloc(sizeof(count_t));

		if (pipe(pipe_fd) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
         }

		if(argc < 2) {
				printf("usage: wc <# of files to count(1-10)>\n");
				return 0;
		}

		numFiles = atoi(argv[1]);
		if(numFiles <= 0 || numFiles > 10) {
				printf("usage: wc <# of files to count(1-10)>\n");
				return 0;
		}


		count_total.charcount = 0;
		count_total.linecount = 0;
	    count_total.wordcount = 0;

		printf("counting %d files..\n\n", numFiles);

		//start multipe child processes
		for(i = 0; i < numFiles; i++)
		{
				sprintf(filename, "%s/text.%02d", FILEPATH, i);
				status = fork();
				
				if(status == 0)
				{
					printf("Start child pid = %d , parent pid = %d\n", getpid(), getppid());
					break;
				}
				if(status == -1)
				{
					printf("fork error\n");
					exit(0);
				}
		}

		if(status == 0)
		{
			tmp = word_count(filename);
			count_point->charcount = tmp.charcount;
			count_point->linecount = tmp.linecount;
			count_point->wordcount = tmp.wordcount;
			close(pipe_fd[0]);
			write(pipe_fd[1],count_point,sizeof(count_t));
			//printf("pid %d read: %s\n", getpid(),filename);
			//printf("------------Child process ------------\n");
			//printf("Child pid = %d , parent pid = %d\n", getpid(), getppid());
			//printf("read: %s\n", filename);
			//printf("lines = %d, words = %d, characters = %d\n",tmp.linecount,tmp.wordcount,tmp.charcount);
		}
		else
		{
			//once a child process exit, read data from pipe
			while((pid = waitpid(0,&returnStatues,WUNTRACED | WCONTINUED)) > 0)
			{	

				if(WIFEXITED(returnStatues))
				{
					printf("Child pid = %d exit normally.\n",pid);
					read(pipe_fd[0],count_point2,sizeof(count_t));
					count_total.charcount += count_point2->charcount;
					count_total.linecount += count_point2->linecount;
					count_total.wordcount += count_point2->wordcount;
				}
				else if(WIFSIGNALED(returnStatues))
				{
					printf("#KILL# Child pid = %d killed by signal %d.\n", pid,WTERMSIG(returnStatues));
					
				}
				else if(WIFSTOPPED(returnStatues))
				{

					printf("#STOP# Child pid = %d stopped by signal %d, waiting for continue signal.\n", pid,WSTOPSIG(returnStatues));
				}
				else if (WIFCONTINUED(returnStatues))
				{
					printf("#CONTINUE# Child pid = %d continued...\n", pid);
				}
				else
				{
					printf("#ERROR# Child pid = %d exit with an error\n",pid);
				}

			}
			
			printf("\n=========================================\n");
	    	printf("Total Lines : %d \n", count_total.linecount);
			printf("Total Words : %d \n", count_total.wordcount);
			printf("Total Characters : %d \n", count_total.charcount);
			printf("=========================================\n");	
			return 0;
		}		
}
