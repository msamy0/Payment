//System liberary definations
//System libraries
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

//Project libraries
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"

int main()
{
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;

		time_t Current_t = time(NULL);
		struct tm Current_Time = *localtime(&Current_t);
		printf("now: %d-%02d-%02d\n", Current_Time.tm_year + 1900, Current_Time.tm_mon + 1, Current_Time.tm_mday);

		/* Implement this into terminal then push it to the structure*/


}