/*
 *	===== drvcfg.c =====
 *	Drivers configuration file
 *
 */

#include <stdlib.h>
#include <dzy\drv.h>

/*	Driver present defines */
#define	DRV_LINK	1	/* link port driver */
#define	DRV_PIPE	0	/* shared memory driver */
#define	DRV_SINE	0	/* sine generator driver*/
#define	DRV_RAND	0	/* random generator driver*/
#define	DRV_HOST	1	/* host driver for ADP101PCI version 2.0 */

#if	DRV_HOST
#include <prf\drvhost.h>
HOST_PARAM	host_param={	/* default parameters */
	(void*)0x0,	/* start address */
	0x0,		/* size of host buffer */
};
DEVICE	DEV_HOST = {
	"host",
	&HOST_Fxn,
	(void*)&host_param
};
#endif

#if	DRV_SINE
#include <prf\drvsine.h>
SINE_PARAM	sine_param={	/* default parameters */
	1000.0,		/* freq */
	50000.0,	/* sample rate */
	0.0,            /* phase */
	50.0,           /* gain */
};
DEVICE	DEV_SINE = {
	"sine",
	&SINE_Fxn,
	(void*)&sine_param
};
#endif

#if	DRV_RAND
#include <prf\drvrand.h>
RAND_PARAM	rand_param={	/* default parameters */
	1.0,		/* seed */
	50.0,		/* upper */
	-50.0,          /* lower */
};
DEVICE	DEV_RAND = {
	"rand",
	&RAND_Fxn,
	(void*)&rand_param
};
#endif

#if	DRV_LINK
#include <prf\drvlink.h>
DEVICE	DEV_LINK = {
	"link",
	&LINK_Fxn,
	NULL
};
#endif

/*--------------------------------------------
 *	Device Table
  -------------------------------------------*/
DEVICE	*DEV_TAB[]={
#if	DRV_HOST
	&DEV_HOST,
#endif
#if	DRV_LINK
	&DEV_LINK,
#endif
#if	DRV_SINE
	&DEV_SINE,
#endif
#if	DRV_RAND
	&DEV_RAND,
#endif
	NULL
};

