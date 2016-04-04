/***************************************************
*
* UTYPES.H - define usefull types.
*
* (C) Instrumental Systems Corp. Ekkore, Dec. 1997-2001
*
****************************************************/


#ifndef _UTYPES_H_
#define _UTYPES_H_


/*************************************
*
*  Types for MSDOS
*/


#ifdef __MSDOS__

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned long	UINT32;
typedef	signed   long	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned long	U32, *PU32;
typedef	signed   long	S32, *PS32;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

//typedef enum{ FALSE=0, TRUE=1}        BOOL;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
typedef unsigned long   HANDLE;

#endif /* __MSDOS__ */


/*************************************
*
*  Types for Windows
*/


#if defined(_Windows) || defined(_WIN32)

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

#endif /* _Windows */


/*************************************
*
*  Types for TMS320C3x/C4x
*/


#if defined(_TMS320C30) || defined(_TMS320C40 )

/*
typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
*/
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* _TMS320C30 || _TMS320C40 */


/*************************************
*
*  Types for TMS320C6x
*/


#ifdef _TMS320C6X

typedef	unsigned char	UINT08;
typedef	signed   char	SINT08;
typedef	unsigned short	UINT16;
typedef	signed   short	SINT16;
typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned char	U08, *PU08;
typedef	signed   char	S08, *PS08;
typedef	unsigned short	U16, *PU16;
typedef	signed   short	S16, *PS16;
typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float			REAL32, *PREAL32;
typedef double			REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT16	USHORT;
typedef UINT08	UCHAR;

#endif /* _TMS320C6X */


/*************************************
*
*  Types for ADSP2106x
*/


#if defined(__ADSP21060__) || defined(__ADSP21061__) || defined(__ADSP21062__)|| defined(__ADSP21065L__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float	 REAL32, *PREAL32;
typedef double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSP2106x__ */

/*************************************
*
*  Types for ADSP2116x
*/


#if defined(__ADSP21160__) || defined(__ADSP21161__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef float	 REAL32, *PREAL32;
typedef double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSP2116x__ */

/*************************************
*
*  Types for ADSP-TS101
*/


#if defined(__ADSPTS__)

typedef	unsigned int	UINT32;
typedef	signed   int	SINT32;

typedef	unsigned int	U32, *PU32;
typedef	signed   int	S32, *PS32;

typedef	unsigned long long int	U64, *PU64;
typedef	signed   long long int	S64, *PS64;

typedef float	 REAL32, *PREAL32;
typedef long double	 REAL64, *PREAL64;

typedef UINT32	ULONG;
typedef UINT32	USHORT;
typedef UINT32	UCHAR;

#endif /* __ADSPTS__ */

/*************************************
*
*  Types for MC24
*/

#if defined(__GNUC__)

typedef unsigned char   UINT08;
typedef signed   char   SINT08;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned long   UINT32;
typedef signed   long   SINT32;

typedef unsigned char   U08, *PU08;
typedef signed   char   S08, *PS08;
typedef unsigned short  U16, *PU16;
typedef signed   short  S16, *PS16;
typedef unsigned long   U32, *PU32;
typedef signed   long   S32, *PS32;

typedef unsigned long long int	U64, *PU64;
typedef signed   long long int	S64, *PS64;

typedef float           REAL32, *PREAL32;
typedef double          REAL64, *PREAL64;

typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
typedef unsigned long   HANDLE;

#endif /* __GNUC__ */

/*************************************
*
*  Type Aliasing
*/

typedef UINT32	Uns;

/*************************************************
*
* Entry Point types
*
*/
#if !defined(WIN32) && !defined(__WIN32__)
//#ifndef WIN32
	#define FENTRY
	#define STDCALL
#else
	#include <windows.h>
	#define DllImport	__declspec( dllimport )
	#define DllExport	__declspec( dllexport )
	#define FENTRY		DllExport
	#define STDCALL		__stdcall
	#define	huge
#endif  // WIN32


#endif /* _UTYPES_H_ */

/*
*  End of File
*/



