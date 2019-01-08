#ifndef __FLASH_H__
#define __FLASH_H__

#include "common.h"

typedef struct{
	IoRegister ACR; // 0x00  Access control register
	IoRegister KEYR;// 0x04  Flash key register
	IoRegister OPTKEYR; // 0x08 Flash option key register
	IoRegister SR;	// 0x0c	Flash status register
	IoRegister CR;	// 0x10 Flash control register
	IoRegister OPTCR; // 0x14 Flash option control register
	IoRegister OPTCR1; // 0x18 Flash option contol register
}flashRegs;

#define flash (flashRegs *)(0x40023C00)
// FLASH_ACR config
#define FLASH_LATENCY0			(0 << 0)		// 0 wait states
#define FLASH_LATENCY1			(1 << 0)		// 1 wait states
#define FLASH_LATENCY2			(2 << 0)		// 2 wait states
#define FLASH_LATENCY3			(3 << 0)		// 3 wait states
#define FLASH_LATENCY4			(4 << 0)		// 4 wait states
#define FLASH_LATENCY5			(5 << 0)		// 5 wait states
#define FLASH_LATENCY6			(6 << 0)		// 6 wait states and so on..
#define FLASH_LATENCY7			(7 << 0)
#define FLASH_LATENCY8			(8 << 0)
#define FLASH_LATENCY9			(9 << 0)
#define FLASH_LATENCY10			(10 << 0)
#define FLASH_LATENCY11			(11 << 0)
#define FLASH_LATENCY12			(12 << 0)
#define FLASH_LATENCY13			(13 << 0)
#define FLASH_LATENCY14			(14 << 0)
#define FLASH_LATENCY15			(15 << 0)

#define FLASH_PRFT_EN			(1 << 8)		// Prefetch enable
#define FLASH_PRFT_DIS			(0 << 8)		// Prefetch disable

#define FLASH_IC_EN				(1 << 9)		// Instruction cache enable
#define FLASH_IC_DIS			(0 << 9)		// Instruction cache disable

#define FLASH_DC_EN				(1 << 10)		// Data cache enable
#define FLASH_DC_DIS			(0 << 10)		// Data cache disable

#define FLASH_IC_RST			(1 << 11)		// Instruction cache reset
#define FLASH_DC_RST			(1 << 12)		// Data cache reset

// FLASH_SR
#define FLASH_EOP_CLR				(1 << 0)		// Clear end of operation flag
#define FLASH_OPERR_CLR				(1 << 1)		// Clear operation flag
#define FLASH_WRPERR_CLR			(1 << 4)		// Clear write protection error
#define FLASH_PGAERR_CLR			(1 << 5)		// Clear programming alignment error.
#define FLASH_PGPERR_CLR			(1 << 6)		// Clear programming parallelism error.
#define FLASH_PGSERR_CLR			(1 << 7)		// Clear programming sequence error.
#define FLASH_RDERR_CLR				(1 << 8)		// Clear proprietary readout protection (PCROP) error.
#define FLASH_BSY_ONG				(1 << 16)		// Flash memory ongoing.

// FLASH_CR
#define FLASH_PC					(1 << 0)	// Flash programming activated.
#define FLASH_SER					(1 << 1)	// Flash sector erase activated.
#define FLASH_MER					(1 << 2)	// Mass erase of bank 1 sectors activated.
//#define FLASH_SNB_SEC0
#define FLASH_PSIZE_8				(0 << 8)	// Program parallelism x8
#define FLASH_PSIZE_16				(1 << 8)	// Program parallelism x16
#define FLASH_PSIZE_32				(2 << 8)	// Program parallelism x32
#define FLASH_PSIZE_64				(3 << 8)	// Program parallelism x64

#define FLASH_MER1					(1 << 15)	// Mass erase of bank 2 user sectors (12 to 23)
#define FLASH_STRT					(1 << 16)	// Trigger an erase operation when set

#define FLASH_EOPIE_EN				(1 << 24)	// Interrupt generation enable
#define FLASH_ERRIE_EN				(1 << 24)	// Interrupt error generation enable

#define FLASH_LOCK					(1 << 31)	// Locked the FLASH_CR register

// FLASH_OPTCR
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

void flashSetWaitState(int noOfWaitStates);
int calculateWaitStates(int referenceFreq, int hclkFreq);
void configureFlashWaitStates(float voltage, int hclkFreq);
#endif /* __FLASH_H__ */
