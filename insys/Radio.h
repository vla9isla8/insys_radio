#pragma once
#include	<map>
#include	<iostream>
#include	"INCLUDE\utypes.h"
#include	"INCLUDE\brd.h"
#include	"INCLUDE\brderr.h"

using namespace std;

#define		CURRFILE			"RADIO"

typedef map <U32, BRD_Info> d_list;

class Radio
{
	d_list		deviceList;
	BRD_Handle	handle	=	-1;
	BRD_Info	info = { sizeof(info) };
	U32			
		openMode,
		g_autoinit	= 0, 
		g_isQuiet	= 0, 
		g_nodeId	= NODE0;	
public:
	Radio();
	~Radio();
	boolean init();
	S32			num;
	S32			err;
	U32			g_lidArray[16];
	d_list	    getdeviceList();
	boolean		openDevice(U32 g_lid);
	void		closeDevice();
	BRD_PuList* getPuList();
	int			displayDevices();
	boolean		loadDSPProgramm(char* fileName);
};

