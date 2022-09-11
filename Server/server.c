//System library definitions
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>


//Project libraries definitions
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"




EN_serverError_t isValidAccount(ST_cardData_t* cardData, long* record_pos_in_DB)
{

	FILE* acc_DB_file_ptr; /* Pointer to the file retrieved from fopen() function */

	/*	temporary string consist of one char and NULL char, so that it could be used with (Strings Handling) functions without exceptional error
		this temporary holder would be used to read .txt file characters to fetch PAN number	*/
	char temp_char_holder[2] = "\0";

	/* temporary string to hold the PAN number retrieved from the .txt file to compare it later */
	char temp_PAN[25] = "\0";

	/* a flag indicated the result of comparison between each PAN record in the .txt file with the user input PAN*/
	int strcmp_result = -1;

	/* assign pointer to the DB file in which accounts are stored using fopen() function in "r" read only mode */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r");


	/* Set the cursor to point to the first character in the text file (initialization step) */
	fseek(acc_DB_file_ptr, 0, SEEK_SET);

	/* the big while loop (Loop1), stay looping through each char in the txt file and only breaks if it found the required inserted PAN
	or it reached the end of the txt file (EOF > End Of File). keep in mind it breaks with the flag strcmp_result equals '0' or else
	if it is '0', then we have found the PAN in the DB so break. If not, that means the loop reached the end of the file without
	finding the PAN in the database */
	while ((temp_char_holder[0] = fgetc(acc_DB_file_ptr)) != EOF)				/*****Loop1*****/
	{
		/* Get the first ',' character as each PAN is enveloped by two ',' character in the database, so finding the first one
		means we got to the first number in the PAN.
		This loop (Loop2) exits if it found ',' or if it reached the end of the file (as a safety step, in case the DB is corrupted)*/
		while (temp_char_holder[0] != ',' && temp_char_holder[0] != EOF)		/*****Loop2*****/
		{
			/* fetch the next char, save it in the temporary char holder. then loop and check against ',' character
			   once (loop2) find ',' character, it breaks.*/
			temp_char_holder[0] = fgetc(acc_DB_file_ptr);
		}

		/* move to the next character after ',' as it is the first number in the PAN in this record */
		temp_char_holder[0] = fgetc(acc_DB_file_ptr);

		/*	This loop (Loop3) pushes the record PAN number to the temporary PAN holder string, it breaks when the end of the PAN reached
			which is marked by another ',' in the end.
			It also breaks if it reached the end of the text as a safety feature in case the DB is corrupted */
		while (temp_char_holder[0] != ',' && temp_char_holder[0] != EOF)		/*****Loop3*****/
		{
			/*append each number to the end of temp_PAN string holder to be compared totally later*/
			strcat(temp_PAN, temp_char_holder);
			temp_char_holder[0] = fgetc(acc_DB_file_ptr);
		}

		/*Check the retrieved PAN number against the user input PAn number. returning '0' means they are matched */
		strcmp_result = strcmp(cardData->primaryAccountNumber, temp_PAN);

		/* if the PAN is found, then break from (Loop1)*/
		if (!strcmp_result)
		{

			break;
		}

		/* re-initialize the temp_PAN and loop again to through the next records (Loop1) */
		temp_PAN[0] = '\0';

	}



	/* Close the file using fclose() function */
	fclose(acc_DB_file_ptr);

	/* Free file pointer memory (save memory!) */
	free(acc_DB_file_ptr);

	puts(strcmp_result == 0 ? "Valid" : "Not");
	if (!strcmp_result)
	{
		return SERV_OK;
	}
	else
	{
		return ACCOUNT_NOT_FOUND;
	}

}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, long* record_pos_in_DB)
{


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
EN_serverError_t saveTransaction(ST_transaction_t* transData, long* record_pos_in_DB)
{

}


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{

	long record_pos_in_DB = 0;
	if (isValidAccount(&transData->cardHolderData, &record_pos_in_DB) == SERV_OK &&
		isAmountAvailable(&transData->terminalData, &record_pos_in_DB) == SERV_OK)
	{
		return APPROVED;
	}

	else if (isValidAccount(&transData->cardHolderData, &record_pos_in_DB) == ACCOUNT_NOT_FOUND)
	{
		return DECLINED_STOLEN_CARD;
	}

	else if (isAmountAvailable(&transData->terminalData, &record_pos_in_DB) == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}

	else if (saveTransaction(transData, &record_pos_in_DB) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}


}




