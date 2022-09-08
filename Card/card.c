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
			
	/*Check Pan lenght and Luhn algorithm check*/
	if	(	strlen(cardData->primaryAccountNumber) > 19		||
			strlen(cardData->primaryAccountNumber) < 16		
			// || Luhn_flag == 0
		)

		{
		return WRONG_PAN;
		}

	else
		{
		return CARD_OK;
		}

}