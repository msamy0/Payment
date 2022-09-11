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

int main()
{	
	ST_transaction_t transactionsData;
	ST_transaction_t* transData = &transactionsData;

	strcpy(transData->cardHolderData.primaryAccountNumber, "20522522545621554");
	strcpy(transData->terminalData.transactionDate, "11/09/2022");
	transData->terminalData.transAmount = 2000;

	long record_pos_in_DB_val = 227; //test on 4th record, (Gaber Said Wael Noah		,20522522545621554,		13000$)
	long* record_pos_in_DB = &record_pos_in_DB_val;

	float new_balacne = 0;

	/* Pointer to the file retrieved from fopen() function */
	FILE* acc_DB_file_ptr;

	/* assign pointer to the DB file in which accounts are stored using fopen() function in "r" read only mode */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r+");

	/* Temporary buffer for the fetched characters */
	char temp_char_holder[2] = "\0";

	/* Temporary string and float buffer for the float amount in the account*/
	char temp_balance_str[40] = "\0";
	float temp_money_val = 0;
	char old_record[100];
	char updated_record[100];

	/* Get the record in buffer*/
	fseek(acc_DB_file_ptr, *record_pos_in_DB, SEEK_SET);
	for (int i = *record_pos_in_DB; i >= 0; i--)
	{
		char r;
		fseek(acc_DB_file_ptr, i, SEEK_SET);
		if ((r=fgetc(acc_DB_file_ptr)) == '\n')
		{
			fseek(acc_DB_file_ptr, i-1, SEEK_SET);
			fgetc(acc_DB_file_ptr);
			break;
		}
	}
	fgets(old_record, 100, acc_DB_file_ptr);
	
	for (int i = 0; i < 100; i++)
	{

	}
		

	/* Covert string to float using std function "atof()" */
	temp_money_val = atof(temp_balance_str);
	new_balacne = temp_money_val - transData->terminalData.transAmount;

	/* Close the file using fclose() function */
	fclose(acc_DB_file_ptr);

	/* Free file pointer memory (save memory!) */
	free(acc_DB_file_ptr);
}