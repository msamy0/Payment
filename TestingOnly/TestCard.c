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

int TestCard()
{
	ST_cardData_t cardData;
	
	//while (getCardHolderName(&cardData))
	//	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	//while (getCardExpiryDate(&cardData))
	//	puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");

	while (getCardPAN(&cardData))
		puts("\nXXX Error in input, Please Try again or click ( CTRL + C ) to terminate ! XXX\n");





}