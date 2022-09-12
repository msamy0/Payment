//System library definitions
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

int isAmountavailableTest()
{	
	ST_terminalData_t terminalData;
	ST_terminalData_t* termData = &terminalData;

	termData->maxTransAmount = 8000;
	termData->transAmount = 8500;//user input
	strcpy(termData->transactionDate, "02/05/2023");
	long record_pos_in_DB_val = 186; //test on 4th record, (Gaber Said Wael Noah	,20522522545621554,13000$)
	long* record_pos_in_DB = &record_pos_in_DB_val;


	/* Pointer to the file retrieved from fopen() function */
	FILE* acc_DB_file_ptr;
	/* assign pointer to the DB file in which accounts are stored using fopen() function in "r" read only mode */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r");

	/* Temporary buffer for the fetched characters */
	char temp_char_holder[2] = "\0";

	/* Temporary string and float buffer for the float amount in the account*/
	char temp_balacne_str[40] = "\0";
	float temp_money_val = 0;

	/*	Seek to cursor to the position of the record fetched by the function isValidAccount, so that we could fetch the
		account available balance without re-fetching the PAN number to get the record again consuming valuable execution time. */
	fseek(acc_DB_file_ptr, *record_pos_in_DB, SEEK_SET);

	/*	- Loop and grab characters till you face '$' character, which considered the end of the balance number */
	while (temp_char_holder[0] != '$')
	{
		/* Fetch the first character after ',' and check if is it number or not to add it to the balance temp holder*/
		temp_char_holder[0] = fgetc(acc_DB_file_ptr);
		if (temp_char_holder[0] < 48 || temp_char_holder[0] > 57)
		{
			continue;
		}
		/* if it is number add it to the temp holder*/
		strcat(temp_balacne_str, temp_char_holder);
	}

	/* Covert string to float using std function "atof()" */
	temp_money_val = atof(temp_balacne_str);

	/* Check values and return !*/
	if (termData->transAmount <= temp_money_val)
	{
		printf("SERV_OK");
		return SERV_OK;
	}
	else
	{
		rintf("LOW_BALANCE");
		return LOW_BALANCE;
	}

	/* Close the file using fclose() function */
	fclose(acc_DB_file_ptr);

	/* Free file pointer memory (save memory!) */
	free(acc_DB_file_ptr);
}