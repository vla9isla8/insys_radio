/*
 ****************** File Ctrl.h *****************************
 *
 *  Definitions of user application interface
 *	structures and constants
 *	for BRD_ctrl
 *
 * (C) InSys by Dorokhin Andrey Jan, 2002
 *
 *	27.11.2003 - add BRDctrl_PIO (Dorokhin A.)
 *	11.02.2004 - add BRDctrl_DIO32IN, BRDctrl_DIO32OUT, BRD_CMPSRC (Dorokhin A.)
 *	06.04.2004 - add BRD_DataBuf, BRD_SyncMode, BRD_StartMode (Dorokhin A.)
 *  18.05.2005 - add BRD_IniFile, BRD_PropertyPage, BRD_PropertyList (Dorokhin A.)
 *	02.06.2005 - add BRDctrl_DSPNODE (Dorokhin A.)
 *	30.08.2005 - add BRDctrl_TEST (Dorokhin A.)
 *
 ************************************************************
*/

#ifndef _CTRL_H
 #define _CTRL_H

#include "utypes.h"

#define BRD_SECTIONSIZE 256 // start comparators numbers

enum {
	BRDctrl_DAQ			= BRD_SECTIONSIZE*8,	// DAQ control
	BRDctrl_CMN			= BRD_SECTIONSIZE*9,	// common control parameters
	BRDctrl_ADC			= BRD_SECTIONSIZE*10,	// ADC control parameters
	BRDctrl_DAC			= BRD_SECTIONSIZE*11,	// DAC control parameters
	BRDctrl_DDC			= BRD_SECTIONSIZE*12,	// DDC control parameters
	BRDctrl_PIO			= BRD_SECTIONSIZE*13,	// PIO control parameters
	BRDctrl_STREAM		= BRD_SECTIONSIZE*14,	// STREAM control parameters
	BRDctrl_SVEAM		= BRD_SECTIONSIZE*15,	// SVEAM control parameters
	BRDctrl_DIOIN		= BRD_SECTIONSIZE*16,	// DIOIN control parameters
	BRDctrl_DIOOUT		= BRD_SECTIONSIZE*17,	// DIOOUT control parameters
	BRDctrl_SDRAM		= BRD_SECTIONSIZE*18,	// SDRAM control parameters
	BRDctrl_DSPNODE		= BRD_SECTIONSIZE*19,	// DSP node control parameters
	BRDctrl_TEST		= BRD_SECTIONSIZE*20,	// Test control parameters
	BRDctrl_CCP			= BRD_SECTIONSIZE*21,	// CCP control parameters
	BRDctrl_ILLEGAL
};

#pragma pack(push, 1)    

// for set and get Parameter value of the each channel
typedef struct _BRD_ValChan 
{
	REAL64	value;		// Parameter value
	U32		chan;		// Channel number
} BRD_ValChan, *PBRD_ValChan;

// for set and get Parameter value 
typedef struct _BRD_EnVal
{
	U32		enable;	// enable/disable parameter
	U32		value;	// value  parameter
} BRD_EnVal, *PBRD_EnVal;

#pragma pack(pop)    

#endif // _CTRL_H

//
// End of file
//

