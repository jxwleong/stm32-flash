#ifndef __FLASH_H__
#define __FLASH_H__

#include "common.h"

typedef struct{
	IoRegister ACR;     // 0x00  Access control register
	IoRegister KEYR;    // 0x04  Flash key register
	IoRegister OPTKEYR; // 0x08  Flash option key register
	IoRegister SR;	    // 0x0c	 Flash status register
	IoRegister CR;	    // 0x10  Flash control register
	IoRegister OPTCR;   // 0x14  Flash option control register
	IoRegister OPTCR1;  // 0x18  Flash option control register
}flashRegs;

#define flash (flashRegs *)(0x40023C00)

#define FLASH_PRFT_EN			(1 << 8)		// Prefetch enable
#define FLASH_PRFT_DIS			(0 << 8)		// Prefetch disable

#define FLASH_IC_EN				(1 << 9)		// Instruction cache enable
#define FLASH_IC_DIS			(0 << 9)		// Instruction cache disable

#define FLASH_DC_EN				(1 << 10)		// Data cache enable
#define FLASH_DC_DIS			(0 << 10)		// Data cache disable

#define FLASH_IC_RST			(1 << 11)		// Instruction cache reset/ flush cache
#define FLASH_DC_RST			(1 << 12)		// Data cache reset/ flush cache

// FLASH_SR (status register)
#define FLASH_EOP				(1 << 0)		// Clear by writing 1 ,end of operation flag
#define FLASH_OPERR				(1 << 1)		// Clear by writing 1 ,operation flag
#define FLASH_WRPERR			(1 << 4)		// Clear by writing 1 ,write protection error
#define FLASH_PGAERR			(1 << 5)		// Clear by writing 1 ,programming alignment error.
#define FLASH_PGPERR			(1 << 6)		// Clear by writing 1 ,programming parallelism error.
#define FLASH_PGSERR			(1 << 7)		// Clear by writing 1 ,programming sequence error.
#define FLASH_RDERR				(1 << 8)		// Clear by writing 1 ,proprietary readout protection (PCROP) error.
#define FLASH_BSY				(1 << 16)		// Clear by writing 1 ,Flash memory ongoing.


// Sectors constant
#define FLASH_MEM_SECTOR_0					 0
#define FLASH_MEM_SECTOR_1					 1
#define FLASH_MEM_SECTOR_2					 2
#define FLASH_MEM_SECTOR_3					 3
#define FLASH_MEM_SECTOR_4					 4
#define FLASH_MEM_SECTOR_5					 5
#define FLASH_MEM_SECTOR_6					 6
#define FLASH_MEM_SECTOR_7					 7
#define FLASH_MEM_SECTOR_8					 8
#define FLASH_MEM_SECTOR_9					 9
#define FLASH_MEM_SECTOR_10					10
#define FLASH_MEM_SECTOR_11	 				11
#define FLASH_MEM_SECTOR_12					12
#define FLASH_MEM_SECTOR_13					13
#define FLASH_MEM_SECTOR_14					14
#define FLASH_MEM_SECTOR_15				 	15
#define FLASH_MEM_SECTOR_16					16
#define FLASH_MEM_SECTOR_17					17
#define FLASH_MEM_SECTOR_18					18
#define FLASH_MEM_SECTOR_19					19
#define FLASH_MEM_SECTOR_20					20
#define FLASH_MEM_SECTOR_21					21
#define FLASH_MEM_SECTOR_22					22
#define FLASH_MEM_SECTOR_23	 				23

#define FLASH_MEM_BANK_1				1
#define FLASH_MEM_BANK_2				2

// FLASH_CR (control register)
#define FLASH_PG					(1 << 0)	// Flash programming activated.
#define FLASH_SER					(1 << 1)	// Flash sector erase activated.
#define FLASH_MASS_ERASE_1			(1 << 2)	// Mass erase of bank 1 sectors activated.

#define FLASH_PSIZE_8				(0 << 8)	// Program parallelism x8, 1 byte
#define FLASH_PSIZE_16				(1 << 8)	// Program parallelism x16, 2 byte
#define FLASH_PSIZE_32				(2 << 8)	// Program parallelism x32, 4 byte
#define FLASH_PSIZE_64				(3 << 8)	// Program parallelism x64, 8 byte

#define FLASH_MASS_ERASE_2			(1 << 15)	// Mass erase of bank 2 user sectors (12 to 23)
#define FLASH_STRT					(1 << 16)	// Trigger an erase operation when set

#define FLASH_EOPIE_EN				(1 << 24)	// End of operation interrupt enable
#define FLASH_ERRIE_EN				(1 << 25)	// Error Interrupt enable

#define FLASH_LOCK					(1 << 31)	// Locked the FLASH_CR register

// FLASH_OPTCR (option control register)
#define FLASH_OPTLOCK				(1 << 0)	// Locked the FLASH_OPTCR register
#define FLASH_OPTSTRT				(1 << 1)	// Triggers a user option operation when set

#define FLASH_BOR_LEV_off			(3 << 2)	// BOR off, POR/PDR reset threshold level is applied
#define FLASH_BOR_LEV_1				(2 << 2)	// BOR level 1
#define FLASH_BOR_LEV_2				(1 << 2)	// BOR level 2
#define FLASH_BOR_LEV_3				(0 << 2)	// BOR level 3



typedef struct WaitStates WaitStates;
struct WaitStates{
	int waitStates;
};


void flashSetWaitState(int noOfWaitStates);
int calculateWaitStates(int referenceFreq, int hclkFreq);
void configureFlashWaitStates(float voltage, int hclkFreq);

int flashUnlock(void);
int flashLock(void);
int flashOptionUnlock(void);
int flashOptionLock(void);

void flashMassErase(int whichBank);
void flashSectorErase(int sectorNumber);
void flashEnableProgramming(int programSize);
void flashDisableProgramming(void);

#define flashSetWaitState(waitStates)									\
										do{								\
											(flash)->ACR &= ~(0xf);		\
											(flash)->ACR |= noOfWaitStates;	\
										}while(0)

#define flashGetStatus(whichFlags)		(flash)->SR & whichFlags
#define isFlashBusy()					flashGetStatus(FLASH_BSY)



// Table for wait states
// Voltage range 1.8V - 2.1V
/*
WaitStates waitStates2p1[50] = {
		[0] = {.waitStates = 0},
		[1] = {.waitStates = 1},
		[2] = {.waitStates = 2},
		[3] = {.waitStates = 3},
		[4] = {.waitStates = 4},
		[5] = {.waitStates = 5},
		[6] = {.waitStates = 6},
		[7] = {.waitStates = 7},
		[8] = {.waitStates = 8},
};

// Voltage range 2.1V - 2.4V
WaitStates waitStates2p4[50] = {
		[0] = {.waitStates = 0},
		[1] = {.waitStates = 1},
		[2] = {.waitStates = 2},
		[3] = {.waitStates = 3},
		[4] = {.waitStates = 4},
		[5] = {.waitStates = 5},
		[6] = {.waitStates = 6},
		[7] = {.waitStates = 7},
		[8] = {.waitStates = 8},
};

// Voltage range 2.4V - 2.7V
WaitStates waitStates2p7[50] = {
		[0] = {.waitStates = 0},
		[1] = {.waitStates = 1},
		[2] = {.waitStates = 2},
		[3] = {.waitStates = 3},
		[4] = {.waitStates = 4},
		[5] = {.waitStates = 5},
		[6] = {.waitStates = 6},
		[7] = {.waitStates = 7},
};

// Voltage range 2.7V - 3.6V
WaitStates waitStates3p6[50] = {
		[0] = {.waitStates = 0},
		[1] = {.waitStates = 1},
		[2] = {.waitStates = 2},
		[3] = {.waitStates = 3},
		[4] = {.waitStates = 4},
		[5] = {.waitStates = 5},
};

*/

#endif /* __FLASH_H__ */
