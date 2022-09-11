//System liberary definations
#include <stdio.h>
#include <stdint.h>

//Project libraries definations
#include "../Server/server.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Application/app.h"



void appStart(void)
{

	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transData;
	

	setMaxAmount(&terminalData);

	while (getCardHolderName(&cardData) != CARD_OK)
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	while (getCardExpiryDate(&cardData) != CARD_OK)
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	while (getCardPAN(&cardData) != CARD_OK)
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	if (getTransactionDate(&terminalData) == TERM_OK)
	{
	
		if (isValidCardPAN(&cardData) == CARD_OK)
		{
			if (isCardExpired(cardData, terminalData) == TERM_OK)
			{
				while (getTransactionAmount(&terminalData) != TERM_OK)
				{
					printf("Enter Correct transaction amount !! \n");
				}

				if (isBelowMaxAmount(&terminalData) == TERM_OK)
				{
					transData.cardHolderData = cardData;
					transData.terminalData = terminalData;
					if (recieveTransactionData(&transData) == APPROVED)
					{
						/* Do the getTransaction check*/

					}
					else if (recieveTransactionData(&transData) == DECLINED_INSUFFECIENT_FUND)
					{
						printf("Error : DECLINED_INSUFFECIENT_FUND \n");
					}
					else if (recieveTransactionData(&transData) == DECLINED_STOLEN_CARD)
					{
						printf("Error : DECLINED_STOLEN_CARD \n");
					}
					else if (recieveTransactionData(&transData) == INTERNAL_SERVER_ERROR)
					{
						printf("Error : INTERNAL_SERVER_ERROR \n");
					}


				}
				else
				{
					printf("Declined amount, exceeded max limit \n");
				}

				
			}
			else
			{
				printf("Expired Card  \n");
			}

		}
		else
		{
			printf("Luhn Check failed for the PAN number you entered !  \n");
		}
	}
	else
	{
		puts("Error fetching the date automatically !! \n");
	}
}




int main11()
{
	int choise = -1;
	while (1)
	{

		appStart();

		printf("\n Do you want to repeat the process again ? \n Make a number choice : \n (1) Yes \n (2) No \n");
		fflush(stdin);
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
