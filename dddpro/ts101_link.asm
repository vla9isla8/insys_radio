/*
	This is the Universal Link Boot Loader for TigerSHARC, based on
	Analog Devices VisualDSP 3.0 Link Boot Loader ts101_link.asm.

	This program was prepared by Antonov Andrew, "Instrumental Systems",
	Moscow, 2004.

	This loader has two differences from Analog Device Link loader,
	first - it automatically determines loading Link.
	Another difference is in more simplicity - there are more less
	self-overwriting codes.

	There are two program variants.
	1. "Not Last DMA Link Boot Loader" is the full functionally analog
	of Analog Devices Link Boot Loader.
	2. "Last DMA Link Boot Loader" has only one functionally difference -
	last block of 256 instructions with tag = 0 is loaded automatically
	by DMA. In this case it needs 4 words alignment before transfer of last
	block. Once more - there is no IMASKH initialization in loader and the
	user program starts as ISR.

	Algorithm "Not Last DMA Link Boot Loader" description:

	1. This boot loader gets loaded by link port DMA of the TigerSharc
	initially into memory 0x00000000 - 0x000000ff. After the boot loader is
	loaded, the link port DMA interrupt wakes the TigerSharc up and starts
	the execution of the loader at location 0x00000000. At this stage, the
	TigerSharc is at the interrupt level of the link DMA and, thus, further
	link port DMAs and global (PMASK[60]) interrupts are disabled.

	2. In _init section the loader:

		2.1 sets the NMOD bit in SQCTL register to insure supervisor
		mode;

		2.2 saves PMASKL in yr5;

		2.3 reduces the interrupt to subroutine level by RDS, link port
		DMAs and global interrupts (PMASK[60]) are  enabled;

		2.4 determines loading link port by data in yr5, disables
		unused link ports and all link DMA, and initialize loading link
		port and its DMA;

		2.5 if loading link port is not Link0, rewrites too instructions
		in _read_word subroutine for DMA channel correspondence;

		2.6 moves first DMA ISR instruction to _r_dma_int location.

	3. The DMA that will bring the data in from the link port will do this
	one quad word at a time. XR3:0 are preset with the required values for
	the TCB.

	4. Data from the link is read by the routine _read_word. Since data
	from link is always in quad format, but the processor needs to parse
	it in single 32-bit words at a time, an internal FIFO buffer is
	maintained. This is implemented as a circular buffer in memory
	locations _fifi-_fifo+0x03, J2 is dedicated as the read pointer to
	the buffer and, thus, J2, JB2 and JL2 are all initialized accordingly.
	The execution flow of _read_word is:
		
		4.1 First J2 is checked to see if it has wrapped back to _fifo
		(i.e. all the data in the buffer has been read) and, if it has
		not, go to step 4.2 to read the next piece of data from the
		buffer.

		4.2 Another quad word is brought into the buffer from the link
		port. To avoid data coming in at precisely the time the DMA is
		started (which will cause data miss due to silicon errata
		#147), corresponding LSTAT is monitored in loop waiting for the
		receiver buffer to be full. Then, the corresponding link port
		DMA is started by writing XR3:0 to the TCB and the routine
		waits for the DMA interrupt in IDLE.

		4.3 When the new quad word arrives from the link port, DMA
		interrupt wakes the processor up from IDLE and execution is
		branched to _r_dma_int, where NOP;; followed by RTI;; returns
		it back to one instruction past the IDLE. Note that the NOP;;
		is necessary here, RTI;; is not allowed to be the first
		instruction of an ISR.

		4.4 Data from the buffer pointed to by J2 is read into xR4 and
		J2 is incremented circularly.

	5. Now, the loader parses the blocks of data from the link port. Two
	words are moved to yR4 and J0. These are the tag words of the block to
	follow. In the first word, bits 31:30 are block TYPE (0=final init,
	1=non-zero init, 2=zero init), bits 29:16 are reserved and bits 15:0
	are the block COUNT. The second tag word is the pointer to DESTINATION.

	6. If type is 1, COUNT number of words are moved one word at a time via
	_read_word to the DESTINATION. Once finished, algorithm goes to step 5.

	7. If type is 2, the COUNT number of zeros are moved to the DESTINATION.
	Once finished, algorithm goes to step 5.

	8. If type is 0, the loader performs the final init, i.e. it overwrites
	itself with the user code. The following algorithm is used:

		8.1 First 32 instructions of user code (destined to locations
		0x0000 - (_init-1)) are moved from the link port via _read_word
		and stored in the registers xR31:8 and yR15:8.

		8.2 Branch Target Buffer is invalidated (BTBINV) to clear
		cached branches.

		8.3 yR1 is initialized to 0x80000000, this value will be
		written to IMASKH to disable global interrupts before start
		of user code (note that the emulation interrupt is left
		enabled).

		8.4 J1 is initialized to _init (first location past relocated
		code) and LC0 is initialized to (0x100 - J1) - number of words
		to be read in the _final_init1.

		8.5 The code execution jumps to _final_init1.

		8.6 Locations _init-0xFF are filled with data from the link
		port.

		8.7 Now link receiving is finished, correct data is in
		_init-0xFF and the data that should be in 0x00-(_init-1) is in
		registers xR31:8 and yR15:8. The remaining code overwrites
		memory location 0x00 - (_init-5) with the data in xR31:8 and
		yR11:8,  finally, the last line of code overwrites locations
		(_init-4)-(_ini-1), including itself, with data from yR15:12
		while executing an absolute jump to 0x00.

	9. The user code starts at 0x00 cleanly.

 

	Algorithm "Last DMA Link Boot Loader" in steps 1 to 7 is the same, as
	algorithm "Not Last DMA Link Boot Loader". Step 8 description:
	
	8. If type is 0, the loader performs the final init, i.e. it overwrites
	itself with the user code. The following algorithm is used:
	
		8.1 Branch Target Buffer is invalidated (BTBINV) to clear
		cached branches.

		8.2 yR1 is initialized to 0x80000000, this value may be written
		to IMASKH to disable global interrupts at start of user code
		(note that the emulation interrupt is left enabled).

		8.3 Link port DMA TCB is initialized to read last block of 256
		words from link port and locate it in 0x0000 - 0x00FF.

		8.4 The code execution jumps to _wait_for_data where DMA is
		started and the routine waits for the DMA interrupt in IDLE.

		8.5 Locations 0x0000 - 0x00FF are filled automatically with
		data from the link port.

		8.6 After link receiving is finished, DMA interrupt wakes the
		processor up from IDLE and execution is branched to 0x00.

	9. The user code starts at 0x00 cleanly.
*/


// Not Last DMA Link Boot Loader

//****************************************************************************

.section seg_ldr;

//********************** Start of code ***************************************
_main:
_r_dma_int:
	jump _init (NP);;	//will be overwrited by nop;; instruction
	rti (ABS)(NP);;	// the second instruction in DMA ISR
	nop;;
	nop;;
_fifo:
	nop;;
	nop;;
	nop;;
	nop;;
//****************************************************************************
_read_word:
	comp(j2,j4);;
	if njeq, jump _read_buffer (NP);;

_wait_for_data:
_lstatxx:
	yr2 = LSTAT0;;		//will be overwrited if loading Link is not 0
        ybitest r2 by 3;;
        if ySEQ, jump _wait_for_data (NP);;
_dc8xx:
        DC8 = xr3:0;;		// start the DMA (will be overwrited if
				// loading Link is not 0)
	idle;;                  // wait till DMA interrupts

_read_buffer:
	xr4 = cb[j2+=1];;                   // read the word from the buffer
	cjmp (ABS) (NP);;                   // and return
//****************************************************************************

_final_init1:
	call _read_word (NP);;                  // read word
	[j1 += 1] = xr4;;                       // write it
	if NLC0E, jump _final_init1 (NP);;

	IMASKH = yr1;;			// disable all ints except emulation

	nop;;
	nop;;

//the last code of this part must be strongly at last 4 memory locations
//before _init, others instructions of this part can not have discontinuity 

	Q[j31 + 0] = xr11:8;;                   // overwrite 0x00-0x03
	Q[j31 + 4] = xr15:12;;                  // overwrite 0x04-0x07
	Q[j31 + 8] = xr19:16;;                  // overwrite 0x08-0x0b
	Q[j31 + 0xc] = xr23:20;;                // overwrite 0x0c-0x0f
	Q[j31 + 0x10] = xr27:24;;               // overwrite 0x10-0x13
	Q[j31 + 0x14] = xr31:28;;               // overwrite 0x14-0x17
	Q[j31 + 0x18] = yr11:8;;               // overwrite 0x18-0x1b
//last code
	jump 0x0 (ABS)(NP);
	Q[j31 + 0x1c] = yr15:12;;               // overwrite 0x1c-0x1f

//***************************************************************************

.align_code 4;
_init:	
	xr0 = 0x00000301;;	// set NMOD and BTB enable bits
				//in sequencer control register
	SQCTL = xr0;;                       

	yr5 = PMASKL;;	//read Links DMA interrupt status
			//if loading from Link 0, yr5 & 0xe0000000 = 0x20000000
			//if loading from Link 1, yr5 & 0xe0000000 = 0x40000000
			//if loading from Link 2, yr5 & 0xe0000000 = 0x80000000
			//if loading from Link 3, yr5 & 0xe0000000 = 0x00000000

	rds;;		// reduce interrupt to subroutine level

	j0 = j31 + _r_dma_int;;               
	yr0 = 0;;	// will be used for zeros
	xr1 = 0x000004C0;;

_link0:
        ybitest r5 by 29;;
        if ySEQ, jump _link1 (NP);;
        IVDMA8 = j0;;       
        LCTL1 = yr0;;                       
        LCTL2 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC2;;
        xr0 = LSTATC3;;
        LCTL0 = xr1;;                   
        jump	_endL0 (NP);;

_link1:
        ybitest r5 by 30;;
        if ySEQ, jump _link2 (NP);;
        IVDMA9 = j0;;       
        LCTL0 = yr0;; 
        LCTL2 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC2;;
        xr0 = LSTATC3;;
        LCTL1 = xr1;;                   
	j0 = j31 + _lstat1;;               
        jump	_end (NP);;

_link2:
        ybitest r5 by 31;;
        if ySEQ, jump _link3 (NP);;
        IVDMA10 = j0;;      
        LCTL0 = yr0;;                       
        LCTL1 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC3;;
        LCTL2 = xr1;;                   
	j0 = j31 + _lstat2;;               
	jump	_end (NP);;

_link3:
        IVDMA11 = j0;;      
        LCTL0 = yr0;;                       
        LCTL1 = yr0;;
        LCTL2 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC2;;
        LCTL3 = xr1;;
	j0 = j31 + _lstat3;;               

_end:
    	j1 = j31 + _lstatxx;; //move Link depended instructions into _read_word
	j3 = j31 + _dc8xx;;
	xr4 = [j0 += 1];;                   
	[j1 += 1] = xr4;;
	xr4 = [j0 += 1];;
	[j3 += 1] = xr4;;

_endL0:
	xr3 = yr0;;

	DC4 = xr3:0;;                       
	DC5 = xr3:0;;                       
	DC6 = xr3:0;;                       
	DC7 = xr3:0;;                       
	DC8 = xr3:0;;                       
	DC9 = xr3:0;;                       
	DC10 = xr3:0;;                      
	DC11 = xr3:0;;                      
    
	j4 = j31 + _fifo;;
	xr0 = j4;;		// xr0 = destination index to <_fifo:>
	xr1 = 0x00040004;;	// count = 4, modify = 4
	xr3 = 0x47000000;;	// int mem,prio=norm,2D=no,word=quad,
				// int=yes,RQ=enbl,chain=no

	jL2 = j31 + 4;;	// J2 is the intermediate link data buffer pointer
	jB2 = xr0;;	// of length 4, base at _fifo:
	j2 = xr0;;                      

	yr3 = 1;;

	j0 = j31 + _dma_int;;               
	j1 = j31 + _r_dma_int;;       // move first ISR nstruction into 0x00
	xr4 = [j0 += 1];;                   
	[j1 += 1] = xr4;;

//***************************************************************************
_boot_loop: 
	call _read_word (NP);;              // read word    
	yr4 = xr4;;                         // read type and count
	nop; nop;;
	call _read_word (NP);;              // read word    
	j0 = xr4;;                          // read destination

	yr2 = 0x0000FFFF;;
	yr2 = r2 and r4;;                   // mask count
	yr1 = 0x00001E02;;
	yr1 = fext r4 by r1;;               // extract type
	if yseq, jump _final_init (NP);;    // if zero - final init...
	yr1 = r1 - r3;;                     // ...else if 1...
	if yaeq, jump _init_data (NP);;     // ...non-zero data init...

//****************************************************************************
_zero_init:                             // ...else zero-data init
   	LC0 = yr2;;                     // put count into counter

_zero_init_loop:
        [j0+=1] = yr0;;                 // init with 0's
        if NLC0E, jump _zero_init_loop (NP);;
	jump _boot_loop (NP);;		// check next block

//****************************************************************************
_init_data:
	LC0 = yr2;;                     // put count into loop counter

_init_data_1:
	call _read_word (NP);;              // read word    
	[j0 += 1] = xr4;;                   // write word
	if NLC0E, jump _init_data_1 (NP);;
	jump _boot_loop (NP);;              // check next block

//************************Final Init******************************************
_final_init:
	// to _xx: must be read the number of words, that will be placed
	// at addresses from 0x0000 to <_init - 1>  

	xr8 = xr4;;			    // put word 1
	call _read_word (NP);;              // read word 2   
	xr9 = xr4;; 
	call _read_word (NP);;              // read word 3   
	xr10 = xr4;;    
	call _read_word (NP);;              // read word 4   
	xr11 = xr4;;    

	call _read_word (NP);;              // read word 5
	xr12 = xr4;;    
	call _read_word (NP);;              // read word 6
	xr13 = xr4;;    
	call _read_word (NP);;              // read word 7   
	xr14 = xr4;;    
	call _read_word (NP);;              // read word 8   
	xr15 = xr4;;    

	call _read_word (NP);;              // read word 9
	xr16 = xr4;;    
	call _read_word (NP);;              // read word 10
	xr17 = xr4;;    
	call _read_word (NP);;              // read word 11   
	xr18 = xr4;;    
	call _read_word (NP);;              // read word 12   
	xr19 = xr4;;    

	call _read_word (NP);;              // read word 13
	xr20 = xr4;;    
	call _read_word (NP);;              // read word 14
	xr21 = xr4;;    
	call _read_word (NP);;              // read word 15   
	xr22 = xr4;;    
	call _read_word (NP);;              // read word 16   
	xr23 = xr4;;    

	call _read_word (NP);;              // read word  17
	xr24 = xr4;;    
	call _read_word (NP);;              // read word  18
	xr25 = xr4;;    
	call _read_word (NP);;              // read word  19  
	xr26 = xr4;;    
	call _read_word (NP);;              // read word  20  
	xr27 = xr4;;    

	call _read_word (NP);;              // read word 21
	xr28 = xr4;;    
	call _read_word (NP);;              // read word  22
	xr29 = xr4;;    
	call _read_word (NP);;              // read word  23  
	xr30 = xr4;;    
	call _read_word (NP);;              // read word  24  
	xr31 = xr4;;    

	call _read_word (NP);;              // read word 25
	yr8 = xr4;;    
	call _read_word (NP);;              // read word 26
	yr9 = xr4;;    
	call _read_word (NP);;              // read word 27   
	yr10 = xr4;;    
	call _read_word (NP);;              // read word 28   
	yr11 = xr4;;    
    
	call _read_word (NP);;              // read word 29
	yr12 = xr4;;    
	call _read_word (NP);;              // read word 30
	yr13 = xr4;;    
	call _read_word (NP);;              // read word 31   
	yr14 = xr4;;    
	call _read_word (NP);;              // read word 32   
	yr15 = xr4;;    
_xx:

	btbinv;;                            // invalidate BTB

	j1 = j31 + _init;;
	yr0 = j1;;
	yr2 = 0x100;;
	yr2 = r2 - r0;;
	LC0 = yr2;;

	yr1 = 0x80000000;;	// IMASKH value - disable all ints
				// except emulation

	jump _final_init1 (NP);;   // and jump to _final_init1

//***************************LINK DMA ISR************************************
_dma_int:
    nop;;

//***************************LINK DEPENDED INSTRUCTIONS**********************
_lstat1:	yr2 = LSTAT1;;
		DC9 = xr3:0;;

_lstat2:	yr2 = LSTAT2;;
		DC10 = xr3:0;;

_lstat3:	yr2 = LSTAT3;;
		DC11 = xr3:0;;

_ende:    
//***************************************************************************
  













// Last DMA Link Boot Loader
/*
//****************************************************************************

.section seg_ldr;

//********************** Start of code ***************************************
_main:
_r_dma_int:
	jump _init (NP);;	//will be overwrited nop;; instruction
	rti (ABS)(NP);;	// the second instruction in DMA ISR
	nop;;
	nop;;
_fifo:
	nop;;
	nop;;
	nop;;
	nop;;
//****************************************************************************
_read_word:
	comp(j2,j4);;
	if njeq, jump _read_buffer (NP);;

_wait_for_data:
_lstatxx:
	yr2 = LSTAT0;;		//will be overwrited  if loading Link is not 0
        ybitest r2 by 3;;
        if ySEQ, jump _wait_for_data (NP);;
_dc8xx:
        DC8 = xr3:0;;           // start the DMA (will be overwrited if loading
				// Link is not 0)
	idle;;                  // wait till DMA interrupts

_read_buffer:
	xr4 = cb[j2+=1];;                   // read the word from the buffer
	cjmp (ABS) (NP);;                   // and return
//****************************************************************************

_init:	
	xr0 = 0x00000301;;	// set NMOD and BTB enable bits
				//in sequencer control register
	SQCTL = xr0;;                       

	yr5 = PMASKL;;	//read Links DMA interrupt status
			//if loading from Link 0, yr5 & 0xe0000000 = 0x20000000
			//if loading from Link 1, yr5 & 0xe0000000 = 0x40000000
			//if loading from Link 2, yr5 & 0xe0000000 = 0x80000000
			//if loading from Link 3, yr5 & 0xe0000000 = 0x00000000

	rds;;		// reduce interrupt to subroutine level

	j0 = j31 + _r_dma_int;;               
	yr0 = 0;;	// will be used for zeros
	xr1 = 0x000004C0;;

_link0:
        ybitest r5 by 29;;
        if ySEQ, jump _link1 (NP);;
        IVDMA8 = j0;;       
        LCTL1 = yr0;;                       
        LCTL2 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC2;;
        xr0 = LSTATC3;;
        LCTL0 = xr1;;                   
        jump	_endL0 (NP);;

_link1:
        ybitest r5 by 30;;
        if ySEQ, jump _link2 (NP);;
        IVDMA9 = j0;;       
        LCTL0 = yr0;; 
        LCTL2 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC2;;
        xr0 = LSTATC3;;
        LCTL1 = xr1;;                   
	j0 = j31 + _lstat1;;               
        jump	_end (NP);;

_link2:
        ybitest r5 by 31;;
        if ySEQ, jump _link3 (NP);;
        IVDMA10 = j0;;      
        LCTL0 = yr0;;                       
        LCTL1 = yr0;;
        LCTL3 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC3;;
        LCTL2 = xr1;;                   
	j0 = j31 + _lstat2;;               
	jump	_end (NP);;

_link3:
        IVDMA11 = j0;;      
        LCTL0 = yr0;;                       
        LCTL1 = yr0;;
        LCTL2 = yr0;;
        xr0 = LSTATC0;;
        xr0 = LSTATC1;;
        xr0 = LSTATC2;;
        LCTL3 = xr1;;
	j0 = j31 + _lstat3;;               

_end:
    	j1 = j31 + _lstatxx;; //move Link depended instructions into _read_word
	j3 = j31 + _dc8xx;;
	xr4 = [j0 += 1];;                   
	[j1 += 1] = xr4;;
	xr4 = [j0 += 1];;
	[j3 += 1] = xr4;;

_endL0:
	xr3 = yr0;;

	DC4 = xr3:0;;                       
	DC5 = xr3:0;;                       
	DC6 = xr3:0;;                       
	DC7 = xr3:0;;                       
	DC8 = xr3:0;;                       
	DC9 = xr3:0;;                       
	DC10 = xr3:0;;                      
	DC11 = xr3:0;;                      
    
	j4 = j31 + _fifo;;
	xr0 = j4;;		// xr0 = destination index to <_fifo:>
	xr1 = 0x00040004;;	// count = 4, modify = 4
	xr3 = 0x47000000;;	// int mem,prio=norm,2D=no,word=quad,
				// int=yes,RQ=enbl,chain=no

	jL2 = j31 + 4;;	// J2 is the intermediate link data buffer pointer
	jB2 = xr0;;	// of length 4, base at _fifo:
	j2 = xr0;;                      

	yr3 = 1;;

	j0 = j31 + _dma_int;;               
	j1 = j31 + _r_dma_int;;       // move first ISR nstruction into 0x00
	xr4 = [j0 += 1];;                   
	[j1 += 1] = xr4;;

//***************************************************************************
_boot_loop: 
	call _read_word (NP);;              // read word    
	yr8 = xr4;;                         // read type and count
	yr24 = 0x0000FFFF;;
	yr24 = r24 and r8;;                   // mask count
	yr1 = 0x00001E02;;
	yr1 = fext r8 by r1;;               // extract type
	if yseq, jump _final_init (NP);;    // if zero - final init...
	call _read_word (NP);;              // read word    
	j0 = xr4;;                          // read destination
	yr1 = r1 - r3;;                     // ...else if 1...
	if yaeq, jump _init_data (NP);;     // ...non-zero data init...

//****************************************************************************
_zero_init:                             // ...else zero-data init
   	LC0 = yr24;;                     // put count into counter

_zero_init_loop:
        [j0 += 1] = yr0;;                 // init with 0's
        if NLC0E, jump _zero_init_loop (NP);;
	jump _boot_loop (NP);;		// check next block

//****************************************************************************
_init_data:
	LC0 = yr24;;                     // put count into loop counter

_init_data_1:
	call _read_word (NP);;              // read word    
	[j0 += 1] = xr4;;                   // write word
	if NLC0E, jump _init_data_1 (NP);;
	jump _boot_loop (NP);;              // check next block

//************************Final Init******************************************
_final_init:

	btbinv;;                            // invalidate BTB
	yr1 = 0x80000000;;                  // IMASKH value - disable all ints except emulation

	xr0 = 0x0;;                         // xr0 = destination index
	xr1 = 0x01000004;;                  // count = 4, modify = 4
	xr3 = 0x47000000;;	// int mem,prio=norm,2D=no, word=quad,int=yes,
				// RQ=enbl,chain=no

	jump _wait_for_data (NP);;   // and jump to patch - _final_init1

//***************************LINK DMA ISR************************************
_dma_int:
    nop;;

//***************************LINK DEPENDED INSTRUCTIONS**********************
_lstat1:	yr2 = LSTAT1;;
		DC9 = xr3:0;;

_lstat2:	yr2 = LSTAT2;;
		DC10 = xr3:0;;

_lstat3:	yr2 = LSTAT3;;
		DC11 = xr3:0;;

_ende:    
//***************************************************************************
*/