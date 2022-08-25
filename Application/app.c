//System liberary definations
#include <stdio.h>
#include <stdint.h>

//Project libraries definations
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"
#include "../Server/server.h"






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
				/* continue  */
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



int main()
{
	char choise = 'A';
	for (;;)
	{
		appStart();

		askAgain:

		puts("\n Do you want to repeat the process again ? \n Make a number choice : \n (Y) Yes \n (N) No");
		gets(choise);
		if (choise == 'Y')
		{
			continue;
		}

		else if (choise == 'N')
		{
			break;
		}

		else
		{
			goto askAgain;
		}
	}

	return 0;
}
