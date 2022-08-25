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

ST_accountsDB_t accountsDB_array[255] = {	{ 20000 , "20522522545621556" },
											{ 30000 , "20522522545621557" },
											{ 15000 , "20522522545621558" },
											{ 1800 ,  "20522522545621559" },
											{ 6000 ,  "20522522545621550" }
};
ST_transaction_t transaction_array[255];
int transaction_array_last = 0;


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_cardData_t x = transData->cardHolderData;
	ST_terminalData_t y = transData->terminalData;
	
	//if (transaction_array[254].cardHolderData.primaryAccountNumber != 0)
	//{
	//	return INTERNAL_SERVER_ERROR;
	//}

	for (int r = 0; r < 255; r++)
	{
		if (!strcmp(accountsDB_array[r].primaryAccountNumber, x.primaryAccountNumber))
			
		{

			if (accountsDB_array[r].balance >= y.transAmount)
			{
				transaction_array[transaction_array_last].cardHolderData = x;
				transaction_array[transaction_array_last].terminalData = y;
				accountsDB_array[r].balance -= y.transAmount;
				transaction_array[transaction_array_last].transactionSequenceNumber = transaction_array_last;
				transaction_array[transaction_array_last].transState = APPROVED;
				transaction_array_last++;
				return APPROVED;
			}
			else
			{
				return DECLINED_INSUFFECIENT_FUND;
			}
			
		}

	}
	return DECLINED_STOLEN_CARD;

}





