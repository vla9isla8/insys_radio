#pragma once
#include	<map>
#include	<iostream>
#include	"INCLUDE\utypes.h"
#include	"INCLUDE\brd.h"
#include	"INCLUDE\brderr.h"
#include	"INCLUDE\brdpu.h"

#define		CURRFILE			"RADIO" 
using namespace std;

typedef map <U32, BRD_Info> d_list;

class	Radio
{
	d_list		deviceList;
	BRD_PuList* puList;
	U32			puCout = 0;
	char		boardname[128]	=	"ADP101PCI";
	BRD_Handle	handle	=	-1;
	BRD_Info	info = { sizeof(info) };
	char*		g_PLD_path			= "PLD_HEX\\ad416s24.h40";
	char*		g_dsp_program_path	= "dddpro.dxe";
	U32			
		openMode,
		g_nodeId	= NODE0;	
public:
	Radio();
	~Radio();
	boolean			init();
	S32				num;
	S32				err;
	U32				g_lidArray[16];
	d_list			getdeviceList();
	boolean			openDevice(S32 g_lid = -1);
	void			closeDevice();
	BRD_PuList *	getPuList(U32 * item);
	int				displayPuList();
	int				displayDevices();
	boolean			loadDSPProgram();
	boolean			startDSPProgram();
	boolean			stopDSPProgram();
	boolean			getPLD(BRD_PuList * pld);
	boolean			getPuState(U32 puId, U32 * state);
	boolean			getPLDState(int * state);
	boolean			loadPLD(int * state);
};

