//#pragma once

#ifndef SERVER_H
#define SERVER_H

//System library definitions
#include <stdio.h>
#include <stdint.h>

//Project libraries definitions
#include "../Card/card.h"
#include "../Application/app.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

#define MAX_RECORD_SIZE 256

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
	int is_blocked;
} ST_accountsDB_t;


typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef enum EN_serverError_t {
	SERV_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
} EN_serverError_t;

//Create a global array of ST_transaction_t 
typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
} ST_transaction_t;



EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData, long* record_pos_in_DB);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, long* record_pos_in_DB);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(ST_transaction_t* transData);

#endif