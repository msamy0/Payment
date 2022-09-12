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


int saveTransactionTest()
{	/***************************initializations for Testing ONLY !***************************/
	ST_transaction_t transactionsData;
	ST_transaction_t* transData = &transactionsData;

	strcpy(transData->cardHolderData.primaryAccountNumber, "20522522545621554");
	strcpy(transData->terminalData.transactionDate, "12/09/2022");
	transData->terminalData.transAmount = 6500;

	long record_pos_in_DB_val = 227; //test on 4th record, (Gaber Said Wael Noah		,20522522545621554,		13000$)
	long* record_pos_in_DB = &record_pos_in_DB_val;
	/***************************initializations for Testing ONLY !***************************/
	

	#define MAX_RECORD_SIZE 256

	/* Pointers to the files retrieved from fopen() function */
	FILE *acc_DB_file_ptr, *temp_file, *trans_DB_file_ptr;

	/*	- open files in read mode for the first one and write mode for the second
		- then check if they don't exist to terminate the program */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r");
	trans_DB_file_ptr = fopen("./DB/Transactions DB.txt", "r+");
	temp_file = fopen("./DB/temp_Accounts DB.txt", "w");

	if (acc_DB_file_ptr == NULL || temp_file == NULL)
	{
		printf("Error opening file(s) \n");
		return SAVING_FAILED;
	}

	/* Temporary buffer for the fetched character from files */
	char buffer_char[2] = "\0";

	/* Temporary string and float buffer for the float amount in the account*/
	char buffer_balance_str[40] = "\0";
	float old_balance_float = 0;
	float updated_balacne_float = 0;

	/* buffer string used as temporary holder along the code*/
	char general_buffer_str[MAX_RECORD_SIZE] = "\0";

	/* strings to save database record of the client's data in use to manipulate and update it */
	char old_record[MAX_RECORD_SIZE] = "\0";
	char updated_record[MAX_RECORD_SIZE] = "\0";

	/* string used in transactions saving that could hold integer max decimal number as string value*/
	char trans_num_buffer_str[10] = "\0";
	int	trans_num_buffer_int = 0;

	/* create transaction amount string*/
	char trans_amount[40] = "\0";
	sprintf(trans_amount, "%d", (int)transData->terminalData.transAmount);

	/*	seek the record evaluated from the previous step in isValidAccount()
		then push client's record to a temporary string "old_record" */	
	fseek(acc_DB_file_ptr, *record_pos_in_DB, SEEK_SET);
	for (int i = *record_pos_in_DB; i >= 0; i--)
	{
		char r;
		fseek(acc_DB_file_ptr, i, SEEK_SET);
		if ((r=fgetc(acc_DB_file_ptr)) == '\n')
		{
			fseek(acc_DB_file_ptr, i-1, SEEK_SET);
			r = fgetc(acc_DB_file_ptr);
			break;
		}
	}
	fgets(old_record, MAX_RECORD_SIZE, acc_DB_file_ptr);
	
	/* create a new reorde for the client with clear balance value, ready to be updated */
	strcpy(updated_record, old_record);
	int pos_of_$ = strchr(updated_record, '$') - updated_record;
	for (int i = pos_of_$; i >= 0; i--)
	{
		if (updated_record[i] == ',')
		{
			break;
		}
		else if ((updated_record[i] >= 48 && updated_record[i] <= 57) || updated_record[i] =='$')
		{
			buffer_balance_str[strlen(buffer_balance_str)] = updated_record[i];
			updated_record[i] = '\0';
		}
		else
		{
			continue;
		}
	}
	_strrev(buffer_balance_str);/* flipping the balance as it was fetched reversed from the record*/

	/* evaluation of the new balance value and pushing it to the new record that was cleared in the previous step */
	/* convert string value to float value*/
	old_balance_float = atof(buffer_balance_str); 
	/* doing math*/
	updated_balacne_float = old_balance_float - transData->terminalData.transAmount;
	/* re-covert float value to string*/
	sprintf(buffer_balance_str, "%d", (int)updated_balacne_float);
	/* push the new balance value to the updated record*/
	strcat(updated_record, buffer_balance_str);
	/* add  dollar mark to the end of the record */
	strcat(updated_record, "$\n"); 

	/*	update Accounts_DB file with new balance 
		- Fetch file's home
		- loop till the end of the file. "feof" = Function End Of File
		- if the fetched record matches the old one, then it is the one to be updated
		- else, just duplicate the old records without touching them */

	fseek(acc_DB_file_ptr, 0, SEEK_SET);
	while (!feof(acc_DB_file_ptr)) 
	{
		fgets(general_buffer_str, MAX_RECORD_SIZE, acc_DB_file_ptr);
		if (!strcmp(general_buffer_str, old_record))
		{
		fputs(updated_record, temp_file);
		}
		else 
		{
		fputs(general_buffer_str, temp_file);
		}
	}

	/* reset string buffer*/
	general_buffer_str[0] = '\0';

	/*********************************************** PUSH TRANSACTION TO DATABASE SECTION ***********************************************/

	/* seek end of transaction file*/
	fseek(trans_DB_file_ptr, 0, SEEK_END);

	/*	Loop through characters of the last line starting from the tail until reaching the transaction number
		which is the number after the third "," comma character*/
	uint8_t trans_num_reach_flag = 0;
	for (int i = 0; i >= ( - 1 * MAX_RECORD_SIZE); i--)//Loop1
	{	
		fseek(trans_DB_file_ptr, i, SEEK_END);
		if (fgetc(trans_DB_file_ptr) == ',')
		{
			trans_num_reach_flag++;
		}
		if(trans_num_reach_flag == 3)
		{
			/*starting from the character before comma (i-1), fetching the number before comma as it is the last transaction number saved*/
			for (int x = i-1 ; x >= ( - 1 * MAX_RECORD_SIZE); x--)//Loop2
			{
				/* seek the last num in transaction number */
				fseek(trans_DB_file_ptr, x, SEEK_END);
				/*push it to the transaction number buffer */
				trans_num_buffer_str[strlen(trans_num_buffer_str)] = fgetc(trans_DB_file_ptr);
				/* break if you reached the previous line*/
				if (trans_num_buffer_str[strlen(trans_num_buffer_str)-1] == '\n')
				{
					trans_num_buffer_str[strlen(trans_num_buffer_str)-1] = '\0';
					break;/*break Loop2*/
				}

			}
			break;/* break Loop1*/
		}
	}
	
	/*flip string as it is fetched reversed*/
	_strrev(trans_num_buffer_str);
	/*convert to integer and increase it*/
	trans_num_buffer_int = atoi(trans_num_buffer_str);
	trans_num_buffer_int++;
	/* re-convert it to string*/
	sprintf(trans_num_buffer_str, "%d", trans_num_buffer_int);

	/*seek end of the file again and push new transaction record data*/
	fseek(trans_DB_file_ptr, 0, SEEK_END);
	fputc('\n', trans_DB_file_ptr);

	strcpy(general_buffer_str, trans_num_buffer_str);
	strcat(general_buffer_str, ",\t\t\t\t\t");

	strcat(general_buffer_str, transData->cardHolderData.primaryAccountNumber);
	strcat(general_buffer_str, ",\t\t");

	strcat(general_buffer_str, transData->terminalData.transactionDate);
	strcat(general_buffer_str, ",\t");
	
	strcat(general_buffer_str, trans_amount);
	strcat(general_buffer_str, "$");

	fputs(general_buffer_str, trans_DB_file_ptr);


	/* Close files using fclose() function */
	fclose(acc_DB_file_ptr);
	fclose(temp_file);
	fclose(trans_DB_file_ptr);

	/* overwrite the old database with the new database created*/
	remove("./DB/Accounts DB.txt");
	rename("./DB/temp_Accounts DB.txt", "./DB/Accounts DB.txt");

	/* Free file pointer memory (save memory!) */
	free(acc_DB_file_ptr);
	free(temp_file);
}