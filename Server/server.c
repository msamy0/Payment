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
			*record_pos_in_DB = ftell(acc_DB_file_ptr);
			break;
		}

		/* re-initialize the temp_PAN and loop again to through the next records (Loop1) */
		temp_PAN[0] = '\0';

	}



	/* Close the file using fclose() function */
	if (fclose(acc_DB_file_ptr) == 0)
	{
		//puts(strcmp_result == 0 ? "Valid" : "Not");
		if (!strcmp_result)
		{
			return SERV_OK;
		}
		else
		{
			return ACCOUNT_NOT_FOUND;
		}
	}
	else
	{
		return ACCOUNT_NOT_FOUND;
	}



}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, long* record_pos_in_DB)
{
	/* Pointer to the file retrieved from fopen() function */
	FILE* acc_DB_file_ptr=0;
	/* assign pointer to the DB file in which accounts are stored using fopen() function in "r" read only mode */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r");
	if (acc_DB_file_ptr == NULL)
	{
		printf("Error opening file(s) \n");
		return SAVING_FAILED;
	}

	/* Temporary buffer for the fetched characters */
	char temp_char_holder[2] = "\0";

	/* Temporary string and float buffer for the float amount in the account*/
	char temp_balacne_str[40] = "\0";
	float temp_money_val = 0;

	/*	Seek to cursor to the position of the record fetched by the function isValidAccount, so that we could fetch the
		account available balance without re-fetching the PAN number to get the record again consuming valuable execution time. */
	fseek(acc_DB_file_ptr, *record_pos_in_DB, SEEK_SET);
	temp_char_holder[0] = fgetc(acc_DB_file_ptr);
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
	temp_money_val = atoi(temp_balacne_str);

	/* Check values and return !*/
	if (termData->transAmount <= temp_money_val)
	{
		printf("SERV_OK");
		return SERV_OK;
	}
	else
	{
		printf("LOW_BALANCE");
		return LOW_BALANCE;
	}

	/* Close the file using fclose() function */
	fclose(acc_DB_file_ptr);


}
EN_serverError_t saveTransaction(ST_transaction_t* transData, long* record_pos_in_DB)
{
	/* Pointers to the files retrieved from fopen() function */
	FILE* acc_DB_file_ptr, * temp_file, * trans_DB_file_ptr;

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
	else
	{
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
			if ((r = fgetc(acc_DB_file_ptr)) == '\n')
			{
				fseek(acc_DB_file_ptr, i - 1, SEEK_SET);
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
			else if ((updated_record[i] >= 48 && updated_record[i] <= 57) || updated_record[i] == '$')
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
		for (int i = 0; i >= (-1 * MAX_RECORD_SIZE); i--)//Loop1
		{
			fseek(trans_DB_file_ptr, i, SEEK_END);
			if (fgetc(trans_DB_file_ptr) == ',')
			{
				trans_num_reach_flag++;
			}
			if (trans_num_reach_flag == 3)
			{
				/*starting from the character before comma (i-1), fetching the number before comma as it is the last transaction number saved*/
				for (int x = i - 1; x >= (-1 * MAX_RECORD_SIZE); x--)//Loop2
				{
					/* seek the last num in transaction number */
					fseek(trans_DB_file_ptr, x, SEEK_END);
					/*push it to the transaction number buffer */
					trans_num_buffer_str[strlen(trans_num_buffer_str)] = fgetc(trans_DB_file_ptr);
					/* break if you reached the previous line*/
					if (trans_num_buffer_str[strlen(trans_num_buffer_str) - 1] == '\n')
					{
						trans_num_buffer_str[strlen(trans_num_buffer_str) - 1] = '\0';
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

		return SERV_OK;
	}
}
//EN_serverError_t getTransaction(ST_transaction_t* transData)

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	long record_pos_in_DB = 0;

	//EN_serverError_t is_Valid_Account;
	//EN_serverError_t is_Amount_Available;
	//EN_serverError_t save_Transaction;

	//is_Valid_Account = isValidAccount(&transData->cardHolderData, &record_pos_in_DB);
	//is_Amount_Available = isAmountAvailable(&transData->terminalData, &record_pos_in_DB);
	//save_Transaction = saveTransaction(transData, &record_pos_in_DB);

	printf("Check");

	if (isValidAccount(&transData->cardHolderData, &record_pos_in_DB) == SERV_OK &&
		isAmountAvailable(&transData->terminalData, &record_pos_in_DB) == SERV_OK &&
		saveTransaction(transData, &record_pos_in_DB) == SERV_OK)
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




