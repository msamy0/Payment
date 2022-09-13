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

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    /* A pre-defined macro in the terminal.h header file. if it is not zero, this function will put it into the maxTransAmount member*/
    if (MAX_AMOUNT_LIMIT <= 0)
    {
        return INVALID_MAX_AMOUNT;
    }
    else
    {
        termData->maxTransAmount = MAX_AMOUNT_LIMIT;
        return TERM_OK;
    }
}

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

    /* Checks if the month is one digit value to add 0 before it for a consistent date formate */
    if (strlen(temp_month_str) < 2)
    {
        temp_month_str[1] = temp_month_str[0];
        temp_month_str[0] = '0';
    }

    /* copying fetched date to terminal data date member by reference */
    strcpy(termData->transactionDate, temp_day_str);
    strcat(termData->transactionDate, "/");
    strcat(termData->transactionDate, temp_month_str);
    strcat(termData->transactionDate, "/");
    strcat(termData->transactionDate, temp_year_str);

    /* Check for the date format to make sure it matches the Rubric regulations (it was only useful if date is entered manually !)*/
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
    /*  - Conversion of string month and year values to decimal values to be comparable
        - subtracting any number in ASCII code from zero character gives us the decimal value of the number
        - multiplying 10's digit by 10 to reproduce a two real integer value for month and year    
        - summing the 10's digit with the 1's digit gives us the true value of the month or the year  */

    uint8_t cardExpMonth = (cardData.cardExpirationDate[0] - '0') * 10 + (cardData.cardExpirationDate[1] - '0');
    uint8_t cardExpYear = (cardData.cardExpirationDate[3] - '0') * 10 + (cardData.cardExpirationDate[4] - '0');
    uint8_t currentMonth = (termData.transactionDate[3] - '0') * 10 + (termData.transactionDate[4] - '0');
    uint8_t currentYear = (termData.transactionDate[8] - '0') * 10 + (termData.transactionDate[9] - '0');

    /* Comparing the years with each others and if it passes, then no need to check month */
    if (currentYear < cardExpYear)

    {
        //printf("year is ok \n");
        return TERM_OK;
    }

    /* if years are equal, then check against months. keep in mind if the card is still in the last month, it is dealed as a valid card */
    else if (currentYear == cardExpYear)
    {
        if (currentMonth <= cardExpMonth)
        {
            //printf("Month is ok 1 \n");
            return TERM_OK;
        }
        else
        {
            //printf("Expired Card (Month Check) \n");
            return EXPIRED_CARD;
        }
    }
    else 
    {
        //printf("Expired Card (Year Check) \n");
        return EXPIRED_CARD;
    }

}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    /*------------------------------------------------------------------------------------*/
/* Start Luhn Algorithm Check */

/* Initialize Luhn_flag value ( -1 -> initial , 1 -> passed , 0 -> failed ) */
    uint8_t Luhn_flag = -1;

    /* Dynamically allocate a new Luhn_arr in the size of CardPAN string size to save memory */
    uint8_t* Luhn_arr = (uint8_t*)malloc((strlen(cardData->primaryAccountNumber)) * sizeof(uint8_t));

    /*Initialize PAN array*/
    for (int i = 0; i < _msize(Luhn_arr); i++)
    {
        Luhn_arr[i] = '\0';
    }
    /* create static Luhn weights array */
    uint8_t Luhn_weight[20] = {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2 };

    /* Luhn_sum is used to determine summing of Luhn number generated after multiplication */
    uint8_t Luhn_sum = 0;

    /*Loop through Luhn_arr limited to it dynamically allocated size which is determind using _msize function*/
    for (int i = 0; i < _msize(Luhn_arr); i++)
    {
        /*Get each number from CardPAN and multiply it by Luhn weights then save it into Luhn_array*/
        Luhn_arr[i] = (cardData->primaryAccountNumber[i] - '0') * Luhn_weight[i];

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
        return TERM_OK;
        
    }
    else
    {
        Luhn_flag = 0;
        return INVALID_CARD;
    }

    /* End Luhn Algorithm Check */
    /*------------------------------------------------------------------------------------*/
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    /* Using puts to show an input request*/
    puts("Enter desired cash amount \n");
    
    /*Get the user amount and put it in the transAmount member in term data*/
    scanf("%f",&termData->transAmount);
    fflush(stdin); /* Flushing the STD*/
    
    /* CHeck whether it is  0 or not */
    if(termData->transAmount <=0)
    {
        return INVALID_AMOUNT;
    } 
    else 
    {
        return TERM_OK;
    }
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    /* Check whether it is below the macimum allowable amount or not*/
    if(termData->transAmount > termData->maxTransAmount)
    {
        return EXCEED_MAX_AMOUNT;
    } 
    else 
    {
        return TERM_OK;
    }

}



