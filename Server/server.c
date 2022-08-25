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







EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_cardData_t x = transData->cardHolderData;
	ST_terminalData_t y = transData->terminalData;
	
	if (transaction_array[255].cardHolderData.primaryAccountNumber != 0)
	{
		return INTERNAL_SERVER_ERROR;
	}

	for (int r = 0; r < 255; r++)
	{
		if (accountsDB_array[r].primaryAccountNumber == x.primaryAccountNumber)
		{

			if (accountsDB_array[r].balance >= y.transAmount)
			{
				transaction_array[transaction_array_last].cardHolderData = x;
				transaction_array[transaction_array_last].terminalData = y;
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





