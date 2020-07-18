#ifndef _DataType_h_
#define _DataType_h_

#ifdef WIN32
#define __packed
#endif

#define IN							
#define EX							
#define IE

typedef unsigned short              US16;
typedef signed short                SS16;
typedef unsigned char               UB08;
typedef signed char                 SB08;
typedef unsigned long               UL32;
typedef signed long                 SL32;
typedef float                       FT32;
typedef double                      DB64;
typedef unsigned long long int      UL64;
typedef signed long long int        SL64;

typedef unsigned short              BUS16;
typedef signed short                BSS16;
typedef unsigned char               BUB08;
typedef signed char                 BSB08;
typedef unsigned long               BUL32;
typedef signed long                 BSL32;
typedef float                       BFT32;
typedef double                      BDB64;
typedef unsigned long long int      BUL64;
typedef signed long long int        BSL64;
typedef void						BVOID;

typedef unsigned short              *PUS16;
typedef signed short                *PSS16;
typedef unsigned char               *PUB08;
typedef signed char                 *PSB08;
typedef unsigned long               *PUL32;
typedef signed long                 *PSL32;
typedef float                       *PFT32;
typedef double                      *PDB64;
typedef unsigned long long int      *PUL64;
typedef signed long long int        *PSL64;
typedef BVOID						*PVOID;
typedef BVOID					   **PPVOD;

#define US16_MAX					(0xFFFF)
#define US16_MIN					(0x0000)
#define UB08_MAX					(0xFF)
#define UB08_MIN					(0x00)
#define UL32_MAX					(0xFFFFFFFF)
#define UL32_MIN					(0x00000000)

#define PNT0(x)						(& (x))
#define PNT1(x)						(* (x))
#define PNT2(x)						(**(x))

#define BIT(x)						(1 << (x))

#define NULL						(0x00)
#define T							(1)
#define F							(0)
#define FAIL						(0x00)
#define PASS						(0x01)

#define CRITICAL_ENTER()			{;}
#define CRITICAL_EXITX()			{;}

typedef enum {eFAIL = 0x00,eTRUE = 0x01,}EBOOL;

typedef union {
	BUB08 Ub08[4];
	BUS16 Us16[2];
	BSS16 Ss16[2];
	BFT32 Ft32;
	BUL32 Ul32;
	BSL32 Sl32;
	struct OrderLocal {
		BUL32 Byte1 : 8;
		BUL32 Byte2 : 8;
		BUL32 Byte3 : 8;
		BUL32 Byte4 : 8;
	}OrderLocal;
	struct OrderRLocal {
		BUL32 Byte4 : 8;
		BUL32 Byte3 : 8;
		BUL32 Byte2 : 8;
		BUL32 Byte1 : 8;
	}OrderRLocal;
}ByteConvert;

#define ENUM(x)	(sizeof(x)/sizeof((x)[0]))
#define SIZE(x)	(sizeof(x))

#define ABS(n)     (((n) < 0) ? -(n) : (n))

#endif
