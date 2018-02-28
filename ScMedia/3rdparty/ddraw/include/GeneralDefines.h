//////////////////////////////////////////////////////////////////////
// GeneralDefines.h 
//
//////////////////////////////////////////////////////////////////////
#if !defined(__GENARAL_APPLICATION_DEFINES__INCLUDED)
#define __GENARAL_APPLICATION_DEFINES__INCLUDED

//====================================================================
#define PACKETLENGHT				188
#define SLIDER_RANGE				1000	
#define TRANSPORT_BUFFER_LENGTH		16384 // 16*1024
#define TRANSPORT_BUFFERS_NUMBER	500   

#define APM_BUFFER_LENGTH			131072 // 128*1024

#define PES_BUFFER_LENGTH			65600 // ( 64*1024 + 64 )

#define MAX_FILE_NAME_LENGTH		512

#define USUAL_WAIT_TIME				400
#define WAIT_TIME_TO_TERMINATE		600
#define REQUEST_WAIT_TIME			1000

#define NO_TIME_STAMP				-1
#define NO_PCR_PID					-1
#define NO_RECIEVER_DELAY			-1

#define DEFAULT_BIT_RATE			4000000
#define SYS_CLOCK_FREQ				27000000
#define FILE_PRES_DELAY				500
#define NET_PRES_DELAY				1500
#define SYNC_BYTE					0x47

// Messages defines
#define NO_MESSAGE					-1

//====================================================================
#endif //__GENARAL_APPLICATION_DEFINES__INCLUDED
