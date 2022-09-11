//System literary definitions
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

int main1()
{	
	ST_cardData_t cardData;
	long record_pos_in_DB = 0;
	
	FILE* acc_DB_file_ptr; /* Pointer to the file retrieved from fopen() function */

	/*	temporary string consist of one char and NULL char, so that it could be used with (Strings Handling) functions without exceptional error
		this temporary holder would be used to read .txt file characters to fetch PAN number	*/
	char temp_char_holder [2] = "\0";

	/* temporary string to hold the PAN number retrieved from the .txt file to compare it later */
	char temp_PAN [25] = "\0";

	/* a flag indicated the result of comparison between each PAN record in the .txt file with the user input PAN*/
	int strcmp_result = -1;

	/* assign pointer to the DB file in which accounts are stored using fopen() function in "r" read only mode */
	acc_DB_file_ptr = fopen("./DB/Accounts DB.txt", "r");

	strcpy(cardData.cardExpirationDate, "09/25");
	strcpy(cardData.cardHolderName, "Mohamed Samy Ahmed");
	strcpy(cardData.primaryAccountNumber, "20522522545621554");

	/* Set the cursor to point to the first character in the text file (initialization step) */
	fseek(acc_DB_file_ptr, 0, SEEK_SET);

	/* the big while loop (Loop1), stay looping through each char in the txt file and only breaks if it found the required inserted PAN
	or it reached the end of the txt file (EOF > End Of File). keep in mind it breaks with the flag strcmp_result equals '0' or else
	if it is '0', then we have found the PAN in the DB so break. If not, that means the loop reached the end of the file without
	finding the PAN in the database */
	while ( (temp_char_holder[0] = fgetc(acc_DB_file_ptr)) != EOF )				/*****Loop1*****/
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
		strcmp_result = strcmp(cardData.primaryAccountNumber, temp_PAN);

		/* if the PAN is found, then break from (Loop1)*/
		if (!strcmp_result)
		{
			record_pos_in_DB = ftell(acc_DB_file_ptr);
			break;
		}

		/* re-initialize the temp_PAN and loop again to through the next records (Loop1) */
		temp_PAN[0] = '\0';

	}
	
	puts(strcmp_result == 0 ? "Valid" : "Not");
	printf("%ld", record_pos_in_DB);
	/* Close the file using fclose() function */
	fclose(acc_DB_file_ptr);

	/* Free file pointer memory (save memory!) */
	free(acc_DB_file_ptr);

}