//System libraries
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>


//Project libraries
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"


/* Function detecated to get the card holder name from the user to and check its length */
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{	/* Refrestand flush standard input */
	fflush(stdin);

	/* Ask user to input the CardHolderName */
	printf("Kindly, input your Card Holder Name \nKeep in mind that it cannot exceed 24 characters \n");

	/* Using gets() function to input the card holder name */
	gets(cardData->cardHolderName);

	/*Testing the user input result*/
	//printf("Card Holder Name you input is : %s \n", cardData->cardHolderName);

	/*Check whether the CardHolderName meets the required length or not*/
	if	(	strlen(cardData->cardHolderName) > 24 ||
			strlen(cardData->cardHolderName) < 20 ||
			strlen(cardData->cardHolderName) == 0
		)

		{
			return WRONG_NAME;
		}
	else
		{
			return CARD_OK;
		}

}

/* Function detecated to get and check the format of CardExpirationDate */
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	/* Ask user to input the card expiration date according to a certain criteria */
	printf("Kindly, input your Card Expiration Date \nKeep in mind that it must be in xx/xx formate \n");

	/* Using gets() function to input the Card Expiration Date */
	gets(cardData->cardExpirationDate);

	/*Testing the user input result*/
	//printf("Card Expiration date you input is : %s \n", cardData->cardHolderName);

	/*Check whether the CardExpirationDate meets regulations or not */
	if	(	strlen(cardData->cardExpirationDate) != 5	|| 
			(int)(cardData->cardExpirationDate[2]) != 47	|| /* 47 is the ASCII int representation of '/' character */
			cardData->cardExpirationDate == 0
		)

		{
			return WRONG_EXP_DATE;
		}
	else
		{
			return CARD_OK;
		}

}

/* Function detecated to get and check the format of CardPAN*/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	/* Ask user to input the PAN */
	printf("Kindly, input your PAN with no spaces \n");

	/* Using gets() function to input the PAN */
	gets(cardData->primaryAccountNumber);

	/*Testing the user input result*/
	//printf("PAN you input is : %s \n", cardData->primaryAccountNumber);

	/*------------------------------------------------------------------------------------*/
	/* Start Luhn Algorithm Check */

	/* Initialize Luhn_flag value ( -1 -> initial , 1 -> passed , 0 -> failed ) */
	uint8_t Luhn_flag = -1;

	/* Dynamicall allocate a new Luhn_arr in the size of CardPAN string size to save memory */
	uint8_t* Luhn_arr = (uint8_t*)malloc(strlen(cardData->primaryAccountNumber) * sizeof(uint8_t));

	/* create static weights array */
	uint8_t Luhn_weight[20] = { 2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1 };

	/* Luhn_sum is used to determine summing of Luhn number generated after multiplication */
	uint8_t Luhn_sum = 0;

	/*Loop through Luhn_arr limited to it dynamically allocated size which is determind using _msize function*/
	for (int i = 0; i < _msize(Luhn_arr); i++)
		{	
			/*Get each number from CardPAN and multiply it by Luhn weights then save it into Luhn_array*/
			Luhn_arr[i] = (cardData->primaryAccountNumber[i]-'0') * Luhn_weight[i];

			/* check whether the number is consist of two digits or not, if though, sum the two digit and overwrite them*/
			if (Luhn_arr[i] > 9)
			{
				Luhn_arr[i] = (Luhn_arr[i] % 10) + 1;

			}

			/*sum Luhn_arr digits after all operations to determin the final value*/
			Luhn_sum += Luhn_arr[i];
		}

	/* if Luhn_sum ends with 0 then its is a valid PAN number*/
	if (Luhn_sum % 10 == 0)
	{
		Luhn_flag = 1;
	}
	else
	{
		Luhn_flag = 0;
	}

	/* End Luhn Algorithm Check */
	/*------------------------------------------------------------------------------------*/

	

	if	(	strlen(cardData->primaryAccountNumber) > 19		||
			strlen(cardData->primaryAccountNumber) < 16		||
			Luhn_flag == 0
		)

		{
		return WRONG_PAN;
		}

	else
		{
		return CARD_OK;
		}

}