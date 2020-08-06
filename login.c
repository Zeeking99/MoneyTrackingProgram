#include <stdio.h>
#include <time.h>
#include "console.h"

void GetCredentials(char *pass, int flag) // function which will take the password and print a * for every character 
{
	char ch;
	int len=0;
	
	while((ch=getch())!='\n') // will take input until enter is pressed
	{
		if(ch==0x7f) // checks that the character pressed is not backspace or delete
		{
			if(len==0)	continue; 
			printf("\b \b"); // deletes the * , prints a space and then clears it 
			len--; // decrementing length because the character is deleted
			continue;
		}

		if(flag==0)	// for zero prints a * for every character entered
			printf("*");
		else if(flag==1)	// for one prints the character for every character entered
			printf("%c",ch);
			
		pass[len]=ch; 
		len++;
	}
	pass[len]='\0'; // assigning the null character to mark the end of the string
}

void delay(int seconds)
{
	clock_t start_time = clock();

	while((clock() - start_time)/CLOCKS_PER_SEC <= seconds);
}
	
