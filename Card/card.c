//System liberary definations
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>


//Project libraries definations
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	
	printf("Kindly, input your Card Holder Name \nKeep in mind that it cannot exceed 24 characters \n");
	gets(cardData->cardHolderName);
	//printf("Card Holder Name you input is : %s \n", cardData->cardHolderName);

	if (strlen(cardData->cardHolderName) > 24		||
		strlen(cardData->cardHolderName) < 20		||
		strlen(cardData->cardHolderName) == 0	)
		return WRONG_NAME;
	else
		return CARD_OK;

}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{

	printf("Kindly, input your Card Expiration Date \nKeep in mind that it must be in xx/xx formate \n");
	gets(cardData->cardExpirationDate);
	//printf("Card Expiration date you input is : %s \n", cardData->cardHolderName);

	if (	strlen(cardData->cardExpirationDate) != 5		||
			(int)(cardData->cardExpirationDate[2]) != 47	|| // 47 is the ASCII int representation of / charater 
			cardData->cardExpirationDate == 0			) 			
		return WRONG_EXP_DATE;
	else
		return CARD_OK;

}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Kindly, input your PAN \nKeep in mind that it mustn't exceed 19 number \n");
	gets(cardData->primaryAccountNumber);
	//printf("PAN you input is : %s \n", cardData->primaryAccountNumber);
	
	if (strlen(cardData->primaryAccountNumber) > 19		||
		strlen(cardData->primaryAccountNumber) < 16		)
		return WRONG_PAN;
	else
		return CARD_OK;

}