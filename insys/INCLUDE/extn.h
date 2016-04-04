/***************************************************
*
* EXTN.H
*
* BRD_extension() cmd codes and data types
*
* (C) Instr.Sys. by Ekkore Apr, 1997-2003
*
****************************************************/


#ifndef	__EXTN_H_
#define	__EXTN_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_BASE			= 0x100,	// Base addres of code range
	BRDextn_GET_MINPERBYTE	= 0x101,	// Get sizeof(char) in bytes
	BRDextn_TURN_DEASY		= 0x102,	// Turn Deasy Server On/Off
};

//
//=== BRD_extension() data types
//

typedef struct
{
	U32	val;					// sizeof(char) in bytes
} BRDextn_GetMinperbyte;

typedef struct
{
	U32	isTurnOn;				// 0 - Turn Off, 1 - Turn On
	U32	timeout;				// milliseconds or BRDtim_FOREVER (if Turn Off)
	U32	isConsole;				// Create New Console
} BRDextn_TurnDeasy;
 

#endif	// __EXTN_H_ 

//
//  End of File
//


