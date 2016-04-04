/***************************************************
*
* BRDERR.H
*
* BRD Shell Error Code Definitions
*
* (C) Instr.Sys. by Ekkore Dec, 2001
*
****************************************************/


#ifndef	__BRDERR_H_
#define	__BRDERR_H_


//  31 30 29...19  18  17   16 15   ...   0
// +-----+-------+------------+------------+
// | LVL |Reserv |Error Source| Error Code |
// +-----+-------+------------+------------+
//
//  LVL - Error Level:
//        00 - Success, Information
//        01 - Warning
//        10 - Error
//        11 - Fatal Error
//
//  Code Producer:
//        1 - BRD Environment
//        2 - Driver
//
//  Error Code: depend on Board
//

//=========================================================
//
// Error Support Macros
//


//
// MKERR - Create Error Code
//
#define MKERR(lvl,src,code) ( (((lvl)&3)<<30) || (((src)&0x7)<<16) || (((code)&0xFFFF)<<0) )

//
// EXTERRLVL - Extract Error Level from Error Code
//
#define EXTERRLVL(err)		 (((err)>>30)&3)

//
//  BRD_errext() - Remove Source Code from Error Code
//
#define BRD_errext(err)		 ((err)&~0x00070000)

//
// S32 BRD_errcmp( S32 errcode1, S32 errcode2 );
//
// Remove Source Code from Error Code
//
#define BRD_errcmp(err1,err2)		 ( (((err1)&~0x00070000)==((err2)&~0x00070000)) ? 1 : 0 )

//
// DRVERR - Convert Error Code for BRD Shell
//
#define BRDERR(err)			((err)|0x10000L)

//
// DRVERR - Convert Error Code for Base Unit Driver
//
#define DRVERR(err)			((err)|0x20000L)

//
// SUBERR - Convert Error Code for SubUnit Driver
//
#define SUBERR(err)			((err)|0x30000L)

//
// SIDEERR - Convert Error Code for Side Driver
//
#define SIDEERR(err)			((err)|0x30000L)

//
// TECHERR - Convert Error Code for TECH Driver
//
#define TECHERR(err)			((err)|0x40000L)

//
// WDMERR - Convert Error Code for WDM Driver
//
#define WDMERR(err)			((err)|0x50000L)

//=========================================================
//
// General Error Codes
//
//=========================================================

//
//	BRDerr_OK	
//
//  No Error
//
#define	BRDerr_OK						0x00000000L

//
//	BRDerr_INFO	
//
//  Undefined Information (the same as BRDerr_OK)
//
#define	BRDerr_INFO						0x00000000L

//
//	BRDerr_WARN	
//
//  Undefined Warning
//
#define	BRDerr_WARN						0x40000000L

//
//	BRDerr_ERROR	
//
//  Undefined Error
//
#define	BRDerr_ERROR					0x80000000L

//
//	BRDerr_FATAL
//
//  Undefined Fatal Error
//
#define	BRDerr_FATAL					0xC0000000L


//=========================================================
//
// BRD Shell Error Codes
//
//=========================================================

//
//	BRDerr_SHELL_IN_USE	
//
//  Info: BRD Shell just is used (for BRD_cleanup())
//
#define	BRDerr_SHELL_IN_USE				0x00000100L

//
//	BRDerr_INIT_REPEATEDLY	
//
//  Info: BRD Shell is already initialazed (for BRD_init(), BRD_autoinit())
//
#define	BRDerr_INIT_REPEATEDLY			0x00000101L

//
//	BRDerr_NONE_DEVICE	
//
//  Warn: None boards was initialized (for BRD_init(), BRD_autoinit())
//
#define	BRDerr_NONE_DEVICE				0x40000100L

//
//	BRDerr_BAD_INI_FILE	
//
//  Error: Can't open INI file (for BRD_init(), BRD_autoinit())
//
#define	BRDerr_BAD_INI_FILE				0xC0000100L

//
//	BRDerr_BAD_REGISTRY	
//
//  Error: Can't open "BRD Shell" section of  Registry (for BRD_init(), BRD_autoinit())
//
#define	BRDerr_BAD_REGISTRY				0xC0000101L

//
//	BRDerr_BAD_LOG_FILE	
//
//  Error: Can't open LOG file (for BRD_autoinit())
//
#define	BRDerr_BAD_LOG_FILE				0xC0000102L

//
//	BRDerr_BAD_ENUMERATION	
//
//  Error: Can't open "ENUM" section of INI File 
//         or Registry (for BRD_autoinit())
//
#define	BRDerr_BAD_ENUMERATION			0xC0000103L

//
//	BRDerr_SHELL_UNINITIALIZED	
//
//  Error: Try open if BRD Shell is uninitialized (for BRD_open())
//
#define	BRDerr_SHELL_UNINITIALIZED		0xC0000104L

//
//	BRDerr_BAD_LID	
//
//  Error: Wrong Local ID (for BRD_open(), BRD_getinfo())
//
#define	BRDerr_BAD_LID					0xC0000105L

//
//	BRDerr_BAD_HANDLE	
//
//  Wrong Handle
//
#define	BRDerr_BAD_HANDLE				0xC0000106L

//
//	BRDerr_CLOSED_HANDLE	
//
//  Handle was closed or isn't opened
//
#define	BRDerr_CLOSED_HANDLE			0xC0000108L

//
//	BRDerr_ALREADY_OPENED	
//
//  Can't open device, because it is already opened
//
#define	BRDerr_ALREADY_OPENED			0xC0000109L

//
//	BRDerr_BAD_MODE	
//
//   Wrong:
//    - Device Open Mode
//    - Resource Capture Mode
//
#define	BRDerr_BAD_MODE					0xC000010AL

//
//	BRDerr_INSUFFICIENT_SERVICES	
//
//  Can't allocate some resource
//
#define	BRDerr_INSUFFICIENT_SERVICES	0xC000010BL



//=========================================================
//
// General Driver Error Codes
//
//=========================================================

//
//	BRDerr_WAIT_ABANDONED	
//
//  WaitForXXXObject returned WAIT_ABANDONED
//
#define	BRDerr_WAIT_ABANDONED			0x40000400L

//
//	BRDerr_NOT_ACTION	
//
//  Result is OK, but not has any action
//
#define	BRDerr_NOT_ACTION				0x40000401L

//
//	BRDerr_INSUFFICIENT_RESOURCES	
//
//  Can't allocate some resource
//
#define	BRDerr_INSUFFICIENT_RESOURCES	0xC0000400L

//
//	BRDerr_BAD_PID	
//
//  Wrong Device PID
//
#define	BRDerr_BAD_PID					0xC0000401L

//
//	BRDerr_BAD_PARAMETER	
//
//  Wrong Handle
//
#define	BRDerr_BAD_PARAMETER			0xC0000402L

//
//	BRDerr_BUFFER_TOO_SMALL	
//
//  Buffer is shorter than need
//
#define	BRDerr_BUFFER_TOO_SMALL			0xC0000403L

//
//	BRDerr_DEVICE_LOST	
//
//  Hot Swaping Device is Surprise Removed
//
#define	BRDerr_DEVICE_LOST				0xC0000404L

//
//	BRDerr_DEVICE_UNSHAREABLE	
//
//  Try to open Device without BRDopen_SHARE flag
//
#define	BRDerr_DEVICE_UNSHAREABLE		0xC0000405L

//
//	BRDerr_FUNC_UNIMPLEMENTED	
//
//  Called Function isn't implemented
//
#define	BRDerr_FUNC_UNIMPLEMENTED		0xC0000406L

//
//	BRDerr_WAIT_TIMEOUT	
//
//  WaitForXXXObject returned WAIT_TIMEOUT
//
#define	BRDerr_WAIT_TIMEOUT				0xC0000407L

//
//	BRDerr_BAD_FILE	
//
//  Can't open File
//
#define	BRDerr_BAD_FILE					0xC0000408L

//
//	BRDerr_BAD_FILE_FORMAT	
//
//  Bad File Contention
//
#define	BRDerr_BAD_FILE_FORMAT			0xC0000409L

//
//	BRDerr_BAD_DEVICE_VITAL_DATA	
//
//  Device can't return PID, PCI bus and slot, 
//  memory and I/O ranges and so on.
//
#define	BRDerr_BAD_DEVICE_VITAL_DATA	0xC000040AL

//
//	BRDerr_NO_KERNEL_SUPPORT	
//
//  Bad kernel mode driver support.
//
#define	BRDerr_NO_KERNEL_SUPPORT		0xC000040BL

//
//	BRDerr_NO_DATA
//
//  Can't get found data
//
#define	BRDerr_NO_DATA					0xC000040CL

//
//	BRDerr_CTRL_UNSUPPORTED	
//
//  Wrong CMD Code for BRD_ctrl()
//
#define	BRDerr_CMD_UNSUPPORTED			0xC000040DL

//
//	BRDerr_HW_ERROR	
//
//  Undefined Hardware Error
//
#define	BRDerr_HW_ERROR					0xC000040EL

//
//	BRDerr_BAD_NODEID	
//
//  Wrong Node ID
//
#define	BRDerr_BAD_NODEID				0xC000040FL

//
//	BRDerr_INVALID_FUNCTION	
//
//  Function is Unimplemented because of some reasons
//
#define	BRDerr_INVALID_FUNCTION			0xC0000410L

//
//	BRDerr_ACCESS_VIOLATION	
//
//  User's Buffer is out of Range or Unaccessible
//
#define	BRDerr_ACCESS_VIOLATION			0xC0000411L

//
//	BRDerr_BAD_ID	
//
//  Wrong ID
//
#define	BRDerr_BAD_ID					0xC0000412L

//
//	BRDerr_NOT_ENOUGH_MEMORY
//
//  Not enough storage is available to process this command.
//
//	Added by Dorokhin A. (23.09.2003)
//
#define	BRDerr_NOT_ENOUGH_MEMORY		0xC0000413L

//
//	BRDerr_NOT_READY
//
//  Some software object is not Ready to complete the operation.
//
#define	BRDerr_NOT_READY				0xC0000414L

//
//	BRDerr_HW_BUSY
//
//  Hardware is Busy so it can't complete the operation.
//
#define	BRDerr_HW_BUSY					0xC0000415L

//
//	BRDerr_INCORRECT_COMMAND_USING
//
//  It's a bad place to call the command.
//
#define	BRDerr_INCORRECT_COMMAND_USING	0xC0000416L

//
//	BRDerr_PLD_TEST_DATA_ERROR
//
//  PLD test returned data error.
//
#define	BRDerr_PLD_TEST_DATA_ERROR		0xC0000417L

//
//	BRDerr_PLD_TEST_ADDRESS_ERROR
//
//  PLD test returned data error.
//
#define	BRDerr_PLD_TEST_ADDRESS_ERROR	0xC0000418L


#endif	// __BRDERR_H_ 

//
//  End of File
//


