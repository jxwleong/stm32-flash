#include "FLASH.h"
#include "common.h"



/*
 * @desc    Calculate the number of wait states based on CPU frequency
 * @param	referenceFreq - based frequency to calculate wait states
 * 			hclkFreq = frequency of CPU
 * @retval  return the number of wait states
 */
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

/*
 * @desc	configre the CPU wait states based on flash voltage and
 * 			clock frequency
 * @param	voltage - flash voltage
 * 			hclkFreq = frequency of CPU
 */
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


/*
 * @desc 	Unlock flash control register so that erase and programming
 * 		 	can be done.
 * @retval  return the status of lock bit in flash control register
 */
int flashUnlock(void){
	(flash)->KEYR = 0x45670123;			// KEY1
	(flash)->KEYR = 0xCDEF89AB;			// KEY2 Must configured consecutively
	return (flash)->CR & FLASH_LOCK;
}

/*
 * @desc 	Lock flash control register, after lock erase and
 * 		 	programming can't be done
 * @retval  return the status of lock bit in flash control register
 */
int flashLock(void){
	(Flash)->CR &= ~(FLASH_PG | FLASH_SER | FLASH_MASS_ERASE_1 \
					 | FLASH_MASS_ERASE_2);
	(flash)->CR |= FLASH_LOCK;
	return (flash)->CR & FLASH_LOCK;
}

/*
 * @desc 	Lock flash option control register, so that flash protection
 * 			configuration is allowed.
 */
int flashOptionUnlock(void){
	(flash)->OPTKEYR = 0x08192A3B;			// KEY1
	(flash)->OPTKEYR = 0x4C5D6E7F;			// KEY2 Must configured consecutively
	return (flash)->OPTCR & FLASH_OPTLOCK;
}

/*
 * @desc 	Lock flash option control register, so that flash protection
 * 			configuration is forbidden
 */
int flashOptionLock(void){
	(flash)->OPTCR |= FLASH_OPTLOCK;
	return (flash)->OPTCR & FLASH_OPTLOCK;
}


/*
 * @desc 	Mass erase the specified bank
 * @param	The memory bank number to erase
 */
void flashMassErase(int whichBank){
	while(isFlashBusy()){} // wait for BSY flag cleared
		(flash)->CR &= ~(FLASH_PG | FLASH_SER | FLASH_MASS_ERASE_1 \
						| FLASH_MASS_ERASE_2);

		if(whichBank == FLASH_MEM_BANK_1)
			(flash)->CR |= FLASH_MASS_ERASE_1;
		else if(whichBank == FLASH_MEM_BANK_2)
			(flash)->CR |= FLASH_MASS_ERASE_2;
		(flash)->CR |= FLASH_STRT;

}
/*
 * @desc 	Erase one sector of the Flash Momory
 * @param	The memory sector number to erase
 * @note	Sector information for STM32F429ZIT6 (2 MB Flash is as followed) :
 * 		BANK NUM    SECTOR	   MEMORY RANGE                SIZE
 * --------------------------------------------------------------
 * 					Sector 0   0x0800 0000 - 0x0800 3FFF   16 Kbytes
 *					Sector 1   0x0800 4000 - 0x0800 7FFF   16 Kbytes
 *					Sector 2   0x0800 8000 - 0x0800 BFFF   16 Kbytes
 *		Bank 1		Sector 3   0x0800 C000 - 0x0800 FFFF   16 Kbyte
 *					Sector 4   0x0801 0000 - 0x0801 FFFF   64 Kbytes
 *					Sector 5   0x0802 0000 - 0x0803 FFFF  128 Kbytes
 *					Sector 6   0x0804 0000 - 0x0805 FFFF  128 Kbytes
 *					...
 *					Sector 11  0x080E 0000 - 0x080F FFFF  128 Kbytes
 *---------------------------------------------------------------
 *
 *					Sector 12  0x0810 0000 - 0x0810 3FFF   16 Kbytes
 *					Sector 13  0x0810 4000 - 0x0810 7FFF   16 Kbytes
 *					Sector 14  0x0810 8000 - 0x0810 BFFF   16 Kbytes
 *		Bank 2		Sector 15  0x0810 C000 - 0x0810 FFFF   16 Kbytes
 *					Sector 16  0x0811 0000 - 0x0811 FFFF   64 Kbytes
 *					Sector 17  0x0812 0000 - 0x0813 FFFF  128 Kbytes
 *					Sector 18  0x0814 0000 - 0x0815 FFFF  128 Kbytes
 *					...
 *					Sector 23  0x081E 0000 - 0x081F FFFF  128 Kbytes
*
 */
void flashSectorErase(int sectorNumber){
	// if sectorNumber < 12, use the number directly
	// ...
	// else if less than 23, then use this formula to calculate
	// the sector value : 0x10 + ( sectorNumber -12 )
	// ...
	// else don't sector erase bcoz the sectorNumber given is invalid
	while(isFlashBusy()){} // wait for BSY flag cleared

		(flash)->CR &= ~(FLASH_PG | FLASH_MASS_ERASE_1 | FLASH_MASS_ERASE_2); // forced these bits to '0'
		(flash)->CR |= FLASH_SER;

		(flash)->CR &= ~(31 << 3);
		if(sectorNumber < 12){
			(flash)-> CR |= sectorNumber << 3;
		}
		else if (sectorNumber < 23){
			(flash)-> CR |= ( 0x10 + (sectorNumber - 12)) << 3;
		}
		(flash)->CR |= FLASH_STRT;

}

/*
 * @desc 	To enable Flash programming
 * @param	Constant for size of data to be program :
 * 			FLASH_PSIZE_8		(1 Bytes)
 * 			FLASH_PSIZE_16		(2 Bytes)
 * 			FLASH_PSIZE_32		(4 Bytes)
 * 			FLASH_PSIZE_64		(8 Bytes)
 */
void flashEnableProgramming(int programSize){
// check BSY flag cleared.
// ...
// Set PG = 1 and ensure MER , MER1, SER = 0;
// SET STRT = 1
// Wait for BSY flag to be cleared
	while(isFlashBusy()){} // wait for BSY flag cleared

		(flash)->CR &= ~(FLASH_SER |FLASH_MASS_ERASE_1 | FLASH_MASS_ERASE_2);
		(flash)->CR |= FLASH_PG;
		(flash)->CR |= FLASH_PSIZE_8;
	}


/*
 * @desc 	To disable Flash programming, any programming to flash
 * 			will be forbidden.
 */
void flashDisableProgramming(void){
	while(isFlashBusy()){} // wait for BSY flag cleared
		(flash)->CR &= ~(FLASH_PG);
}

/*
 * @desc 	Wait until the BUSY bit is clear (ready for operation)
 *
 */
void flashWaitTillNotBusy(void){
	while(isFlashBusy()){

	} // wait for BSY flag cleared
}
