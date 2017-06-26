/*************************************************
	* C program to count no of lines, words and 	 *
	* characters in a file.			 *
	*************************************************/

#include <stdio.h>
#define FILEPATH "/Users/ruoyancai/Desktop/Project1/books"

//#define FILEPATH "/tmp/CSCI4730/books"

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

int main(int argc, char **argv)
{
		int i, numFiles;
		char filename[100];
		count_t count, tmp;

		if(argc < 2) {
				printf("usage: wc <# of files to count(1-10)>\n");
				return 0;
		}

		numFiles = atoi(argv[1]);
		if(numFiles <= 0 || numFiles > 10) {
				printf("usage: wc <# of files to count(1-10)>\n");
				return 0;
		}

		count.linecount = 0;
		count.wordcount = 0;
		count.charcount = 0;

		printf("counting %d files..\n", numFiles);

		for(i = 0; i < numFiles; i++)
		{
				sprintf(filename, "%s/text.%02d", FILEPATH, i);
				printf("read: %s\n", filename);
				tmp = word_count(filename);

				count.charcount += tmp.charcount;
				count.linecount += tmp.linecount;
				count.wordcount += tmp.wordcount;
		}

		printf("\n=========================================\n");
		printf("Total Lines : %d \n", count.linecount);
		printf("Total Words : %d \n", count.wordcount);
		printf("Total Characters : %d \n", count.charcount);
		printf("=========================================\n");

		return(0);
}
