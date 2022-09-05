//#pragma once

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <string.h>
#include "../Card/card.h"
/*Setting the maximum amount for the terminal in the device internal frimware as it is more sensible */
#define MAX_AMOUNT_LIMIT 8000

typedef struct ST_terminalData_t
{
	float transAmount; /* User input for transaction*/
	float maxTransAmount; /* Value to be set by the terminal*/
	uint8_t transactionDate[11]; /* Date of the process*/
}ST_terminalData_t;


typedef enum EN_terminalError_t
{
	TERM_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
//EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);



#endif
// #pragma once
