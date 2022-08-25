//System liberary definations
#include <stdio.h>
#include <stdint.h>

//Project libraries definations
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Kindly, input Transaction Date \nKeep in mind that it must be in DD/MM/YYYY formate \n");
	gets(termData->transactionDate);

	if (	strlen(termData->transactionDate) != 10		||
			(termData->transactionDate[2]) != '/'	    ||
			(termData->transactionDate[5]) != '/') 			
		return WRONG_DATE;
	else
		return TERM_OK;

}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    uint8_t cardExpMonth = (cardData.cardExpirationDate[1] - '0') * 10 + (cardData.cardExpirationDate[0] - '0');
    uint8_t cardExpYear = (cardData.cardExpirationDate[4] - '0') * 10 + (cardData.cardExpirationDate[3] - '0');
    // printf ("fist month Digit is %c \n",cardData.cardExpirationDate[0]);
    // printf ("second month Digit is %c \n",cardData.cardExpirationDate[1]);

    // printf ("fist year Digit is %c \n",cardData.cardExpirationDate[3]);
    // printf ("second Year Digit is %c \n",cardData.cardExpirationDate[4]);

    uint8_t currentMonth = (termData.transactionDate[4] - '0') * 10 + (termData.transactionDate[3] - '0');
    uint8_t currentYear = (termData.transactionDate[9] - '0') * 10 + (termData.transactionDate[8] - '0');

    if (currentYear < cardExpYear)
    {
        printf("ok is ok \n");
        return TERM_OK;
    }
    else if (currentYear == cardExpYear)
    {
        if (currentMonth <= cardExpMonth)
        {
            //printf("ok is ok \n");
            return TERM_OK;
        }
        else
        {
            //printf("Expired Month \n");
            return EXPIRED_CARD;
        }
    }
    else 
    {
        //printf("Expired Year \n");
        return EXPIRED_CARD;
    }

}


/* function for presetting the maximum amount for each transaction 8000LE */
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    if(MAX_AMOUNT_LIMIT <= 0)
    {
        return INVALID_MAX_AMOUNT;
    }
    else
    {
        termData->maxTransAmount = MAX_AMOUNT_LIMIT;
        return TERM_OK;
    }
}



//EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{

    puts("Enter desired cash amount \n");
    
    fflush(stdin);
    scanf("%f",&termData->transAmount);
    fflush(stdin);

    if(termData->transAmount <=0)
    {
        puts("INVALID_AMOUNT \n");
        return INVALID_AMOUNT;
    } 
    else 
    {
        puts("ITERM_OK \n");
        return TERM_OK;
    }
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if(termData->transAmount > termData->maxTransAmount)
{
    return EXCEED_MAX_AMOUNT;
} 
else 
{
   return TERM_OK;
}

}



