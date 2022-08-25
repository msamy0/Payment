// #pragma once

#ifndef SERVER_H
#define SERVER_H

//System library definations
#include <stdio.h>
#include <stdint.h>

//Project libraries definations
#include "../Card/card.h"
#include "../Application/app.h"
#include "../Server/server.h"


//Create a global array of ST_accountsDB_t for the valid accounts database
typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

ST_accountsDB_t accountsDB_array[255] = {	{ 20000 , "20522522545621556" },
											{ 30000 , "20522522545621557" },
											{ 15000 , "20522522545621558" },
											{ 1800 ,  "20522522545621559" },
											{ 6000 ,  "205225225456215510"}
										};

//Create a global array of ST_transaction_t 

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	int transactionSequenceNumber;
	EN_transState_t transState;
} ST_transaction_t;

ST_transaction_t transaction_array[255] = { {{ 0 }} };
int transaction_array_last = 0;



typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;


typedef enum EN_serverError_t {
	SERV_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;



EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(ST_transaction_t* transData);

#endif