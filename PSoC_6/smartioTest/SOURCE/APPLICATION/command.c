/*
 * command.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */



#include "command.h"

void lowerCasing(char* str)
{
	for(int i = 0; str[i]; i++)
	{
	  str[i] = tolower(str[i]);
	}
}


void spacer(char* string,char *word)
{
	char chartemp[50];
	int index1;
	int index2;
	strcpy(word,"NO DATA");

	for(index2 = 0; index2 < strlen(string) ;index2++)
	{
		if(string[index2] != ' ')
		{
			break;
		}
	}

	for(index1 = 0;string[index2] != ' ' && string[index2] != '\0'; index2++,index1++)
	{
		word[index1] = string[index2];
	}
	word[index1] = '\0';
//	sprintf(temp,"\r\n\r\nWORD: %s",word);
//	serialPrint(temp);

	strcpy(chartemp,&string[index2+1]);
//	sprintf(temp,"\r\nchartemp: %s",chartemp);
//	serialPrint(temp);

	strcpy(string,chartemp);
//	sprintf(temp,"\r\nstring: %s\r\n",string);
//	serialPrint(temp);

}


static bool poweronFlag;

void commandReception_TASK()
{
	char command[50] ,subCmd[10];

	if(!poweronFlag)
	{
		serialPrint("\r\n[ COMMAND ] :");
		poweronFlag++;
	}
	if(commandIssued(command))
	{
		lowerCasing(command);

		spacer(command,subCmd);
		if(!strcmp(subCmd,"dc"))
		{
			spacer(command,subCmd);
			if(!strcmp(subCmd,"green"))
			{
				commandResponse("Green Command Accepted");
			}
			if(!strcmp(subCmd,"red"))
			{
				commandResponse("red Command Accepted");
			}
			if(!strcmp(subCmd,"blue"))
			{
				commandResponse("blue Command Accepted");
			}
		}

		if(!strcmp(subCmd,"gpset"))
		{
			spacer(command,subCmd);
			if(!strcmp(subCmd,"1"))
			{
				spacer(command,subCmd);
				commandResponse("Accepted for 1");
				cyhal_gpio_write(TEST_OUT_1, atoi(subCmd));
			}
			if(!strcmp(subCmd,"2"))
			{
				spacer(command,subCmd);
				commandResponse("Accepted for 2");
				cyhal_gpio_write(TEST_OUT_2, atoi(subCmd));
			}
			if(!strcmp(subCmd,"3"))
			{
				spacer(command,subCmd);
				commandResponse("Accepted for 3");
				cyhal_gpio_write(TEST_OUT_3, atoi(subCmd));
			}
		}

		serialPrint("\r\n\r\n[ COMMAND ] : ");
	}

}
