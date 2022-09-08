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


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    /* initialization string variables to save date portions retrived from time() function */
    uint8_t temp_day_str[3] ="\0";
    uint8_t temp_month_str[3] = "\0";
    uint8_t temp_year_str[5] = "\0";

    /* initialization of transactionDate member with NULL to avoid expceptional handeling while using strcat() function */
    for (int i = 0; i < sizeof(termData->transactionDate); i++)
    {
        termData->transactionDate[i] = NULL;
    }

    /* Using builtin function time() to fetch the current PC time and save it a pre-defined struct in time.h header*/
    time_t Current_t = time(NULL);
    struct tm Current_Time = *localtime(&Current_t);

    /* Using sprintf() function to convert Current.Time integer members to string values. Keep in mind time() function fetches time then
    push it back as integer numbers in the Current.Time struct members, representing each portion of time (day, month, year, hour, ..etc) */
    sprintf(temp_day_str,   "%d", Current_Time.tm_mday);
    sprintf(temp_month_str, "%d", Current_Time.tm_mon+1);/*Months from 0 to 11 */
    sprintf(temp_year_str,  "%d", Current_Time.tm_year+1900); /* Years counting from 0 to infinity, representing (1900) year as (0) */
    
    /* Checks if the day is one digit value to add 0 before it for a consistant date formate */
    if (strlen(temp_day_str) < 2)
    {
        temp_day_str[1] = temp_day_str[0];
        temp_day_str[0] = '0';
    }

    /* Checks if the month is one digit value to add 0 before it for a consistant date formate */
    if (strlen(temp_month_str) < 2)
    {
        temp_month_str[1] = temp_month_str[0];
        temp_month_str[0] = '0';
    }

    /* copying fetched date to terminal data date member by refernce */
    strcpy(termData->transactionDate, temp_day_str);
    strcat(termData->transactionDate, "/");
    strcat(termData->transactionDate, temp_month_str);
    strcat(termData->transactionDate, "/");
    strcat(termData->transactionDate, temp_year_str);

    /* Check for the date format to make sure it matchs the Rubric reglations (it was only useful if date is entered manually !)*/
    if (strlen(termData->transactionDate) != 10 ||
        (termData->transactionDate[2]) != '/' ||
        (termData->transactionDate[5]) != '/')
    {
        return WRONG_DATE;
    }
    else
    {
        return TERM_OK;
    }


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
            printf("ok is ok \n");
            return TERM_OK;
        }
        else
        {
            printf("Expired Month \n");
            return EXPIRED_CARD;
        }
    }
    else 
    {
        printf("Expired Year \n");
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



