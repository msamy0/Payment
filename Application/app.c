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
	/* Delectation of data handling structure*/
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transData;
	
	/*Function to set the maximum transaction amount for a single payment process >> MAX_AMOUNT_LIMIT */
	setMaxAmount(&terminalData);
	
	/* Looping through this function until user puts a valid card holder name*/
	while (getCardHolderName(&cardData) != CARD_OK)
	{
		puts("\nXXX Error in Holder name, it doesn't match rubric criteria \nPlease Try again or click ( CTRL + C ) to terminate ! XXX\n");
	}

	/* Looping through this function until the user puts date in the proper formate*/
	while (getCardExpiryDate(&cardData) != CARD_OK)
	{
		puts("\nXXX Error in expiry date formate, it doesn't match rubric criteria \nPlease Try again or click ( CTRL + C ) to terminate ! XXX\n");
	}

	/* Looping through this function until the user puts a PAN with a valid lenght*/
	while (getCardPAN(&cardData) != CARD_OK)
	{
		puts("\nXXX Error in PAN formate of number of digits, it doesn't match rubric criteria \nPlease Try again or click ( CTRL + C ) to terminate ! XXX\n");
	}

	/* if the program succeed to get todays day, it will pass to the next check*/
	if (getTransactionDate(&terminalData) == TERM_OK)
	{
		
		/* checking the card PAN by Luhn algorithm*/
		if (isValidCardPAN(&cardData) == CARD_OK)
		{
			/* checking the expiration date of the card if it exceeds today's date*/
			if (isCardExpired(cardData, terminalData) == TERM_OK)
			{
				/* Loop asking the user to put the amount required to be withdrawn */
				while (getTransactionAmount(&terminalData) != TERM_OK)
				{
					printf("Enter Correct transaction amount, or Ctrl+C to terminate the process \n");
				}
				 
				/* checking whether that value is below the maximum limit pre_defined >> MAX_AMOUNT_LIMIT  */
				if (isBelowMaxAmount(&terminalData) == TERM_OK)
				{
					/* pass all data to transData structure to be further processed */
					transData.cardHolderData = cardData;
					transData.terminalData = terminalData;

					/* this function processes the user data against the local server database, and considered the last step in the process */
					EN_transState_t recieveTransactionData_result = recieveTransactionData(&transData);

					if (recieveTransactionData_result == APPROVED)
					{
						printf("Transaction was done successfully, Thanks");
						/* Do the getTransaction check*/
					}

					/* if the server failed to complete the process due to low balance in the user account, it will prompt the following*/
					else if (recieveTransactionData_result == DECLINED_INSUFFECIENT_FUND)
					{
						printf("Error : DECLINED_INSUFFECIENT_FUND \n");
					}
					/* if the server failed to complete the process due to unknown PAN which is not in the database, it will prompt the following*/
					else if (recieveTransactionData_result == DECLINED_STOLEN_CARD)
					{
						printf("Error : DECLINED_STOLEN_CARD \n");
					}

					/* if the server failed to process of save the transaction due to any issue, it will prompt the following*/
					else if (recieveTransactionData_result == INTERNAL_SERVER_ERROR)
					{
						printf("Error : INTERNAL_SERVER_ERROR \n");
					}


				}

				/* if the user put amount greater than MAX_AMOUNT_LIMIT */
				else
				{
					printf("Declined amount, exceeded single transaction maximum limit !! \n");
				}	
			}

			/* if the card is expired*/
			else
			{
				printf("Expired Card  \n");
			}
		}

		/* if Luhn check failed while checking the PAN*/
		else
		{
			printf("Luhn Check failed for the PAN number you entered !  \n");
		}
	}

	/* if the program failed to fetch today's date automatically to use it in expiration date comparison, it will prompt the following*/
	else
	{
		puts("Error fetching the date automatically !! \n");
	}
}

int main()
{	
	/* choice is a flage used to save the users decision to go for another process or terminate the program */
	int choice = -1;

	/* loop through the program*/
	while (1)
	{	
		/* function that handles all processes */
		appStart();

		/* after handling a successful or failed process, the user is asked if he wants to do another process */
		printf("\n Do you want to repeat the process again ? \n enter a number to choose : \n (1) Yes \n (2) No \n");

		/* flushing the std input*/
		fflush(stdin);

		/* get user's choice */
		scanf("%d", &choice);

		/* flushing the std input*/
		fflush(stdin);

		/* if YES, then loop again starting from appStart()*/
		if (choice == 1)
		{
			choice = -1;
			continue;
		}
		
		else
		{
			choice = -1;
			break;
		}
	}
	return 0;
}
