//System liberary definations
#include <stdio.h>
#include <stdint.h>

//Project libraries definations
#include "../Server/server.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"

extern ST_accountsDB_t accountsDB_array[255];
extern ST_transaction_t transaction_array[255];
extern int transaction_array_last;



void appStart(void)
{
	
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transData;

	while (getCardHolderName(&cardData))
	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");
	
	while (getCardExpiryDate(&cardData))
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	 while (getCardPAN(&cardData))
	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	
	while (getTransactionDate(&terminalData))
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");
		
	
	if(isCardExpired(cardData, terminalData) ==	 TERM_OK)
	{
				
		while (getTransactionAmount(&terminalData) != TERM_OK)
		{
			printf("Enter Correct transaction amount \n");	
		}

		setMaxAmount(&terminalData);
		if(isBelowMaxAmount(&terminalData) == TERM_OK)
		{

			transData.cardHolderData = cardData;
			transData.terminalData = terminalData;

			EN_transState_t trans_status = -1;
			trans_status = recieveTransactionData(&transData);

			if (trans_status == APPROVED)
			{
				printf("Approved \n");
			}
			else if (trans_status == DECLINED_INSUFFECIENT_FUND)
			{
				printf("Error : Insuffcient Fund ! \n");
				
			}

			else if (trans_status == DECLINED_STOLEN_CARD)
			{
				printf("Error : Stolen Card ! \n");
			}

			else if (trans_status == INTERNAL_SERVER_ERROR)
			{
				printf("Error : Server is Full ! \n");
			}

		
		}

		else
		{
			printf("Declined amount, exceeded limit \n");

		}


	}
	else
	{
		printf("Expired Card  \n");

	}



	
}



int main1()
{
	int choise = -1;
	while (1)
	{

		appStart();

		printf("\n Do you want to repeat the process again ? \n Make a number choice : \n (1) Yes \n (2) No \n");
;		fflush(stdin);
		scanf("%d", &choise);
		fflush(stdin);

		if (choise == 1)
		{
			choise = -1;
			continue;
		}

		else if (choise == 2)
		{
			choise = -1;
			break;
		}
		
	}
	return 0;
}
