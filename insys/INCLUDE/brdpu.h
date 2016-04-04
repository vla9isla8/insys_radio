/*
 ****************** File brdpu.h ************************
 *
 * Definitions of structures and constants
 * for Programmable Unit (struc BRD_PuList)
 *
 * (C) InSys by Dorokhin A. Apr 2004
 *
 ********************************************************
*/

#ifndef _BRDPU_H_
 #define _BRDPU_H_

// Programmable Unit ID Types (puId)
enum
{
	BRDpui_BASEICR	= 0x01,	// ICR on base unit
	BRDpui_PCICFG	= 0x02,	// PCI configuration space
	BRDpui_SUBICR	= 0x03,	// ICR on subunit
	BRDpui_PLD		= 0x100	// XILINX XC....
};

// Programmable Unit Attribute Types (puAttr)
enum
{
	BRDpua_Load		= 0x01,
	BRDpua_Read		= 0x02,
	BRDpua_Write	= 0x04,
	BRDpua_Danger	= 0x08,
	BRDpua_LOAD		= 0x01,
	BRDpua_READ		= 0x02,
	BRDpua_WRITE	= 0x04,
	BRDpua_DANGER	= 0x08
};

#endif	// _BRDPU_H_

// ****************** End of file brdpu.h **********************
