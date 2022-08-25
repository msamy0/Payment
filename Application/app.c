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
	
	// while (getCardHolderName(&cardData))
	// 	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");
	
	while (getCardExpiryDate(&cardData))
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	//  while (getCardPAN(&cardData))
	// 	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");


	
	 while (getTransactionDate(&terminalData))
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");
	isCardExpired(cardData, terminalData);



	
}



int main()
{
	appStart();
	return 0;
}