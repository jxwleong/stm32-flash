#include "common.h"
#include "FLASH.h"


void flashSetWaitState(int noOfWaitStates){
	(flash)->ACR = noOfWaitStates;
}


int calculateWaitStates(int referenceFreq, int hclkFreq){
	float tempFloat;
	int tempInt;

	tempFloat = (hclkFreq % referenceFreq);				// If hclkFreq = 40M
	if((int)tempFloat == 0)						// tempFloat = 0
		tempInt = (hclkFreq / referenceFreq) - 1;			// then tempInt - 1 = 1 wait states
	else
		tempInt = (hclkFreq / referenceFreq);
	return tempInt;
}

void configureFlashWaitStates(float voltage, int hclkFreq){
	int noOfWaitStates;

	if(voltage < 1.8){
		noOfWaitStates = 8;
	}
	else if(voltage <= 2.1){
		noOfWaitStates = calculateWaitStates(20000000, hclkFreq);
	}
	else if(voltage <= 2.4){
		noOfWaitStates = calculateWaitStates(22000000, hclkFreq);
	}
	else if(voltage <= 2.7){
		noOfWaitStates = calculateWaitStates(24000000, hclkFreq);
	}
	else{
		noOfWaitStates = calculateWaitStates(30000000, hclkFreq);
	}
	flashSetWaitState(noOfWaitStates);
}
