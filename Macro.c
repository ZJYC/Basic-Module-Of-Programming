//--------------------------------------------------------------------------
//反转32位数
#define CONVERT32(x)	(((x) & 0x000000FF) << 24 |((x) & 0x0000FF00) << 8 |((x) & 0x00FF0000) >> 8 |((x) & 0xFF000000) >> 24)
//反转16位数
#define CONVERT16(x)	(((x) & 0x00FF) << 8 | ((x) & 0xFF00) >> 8)
//--------------------------------------------------------------------------
typedef unsigned short   					US16;
typedef signed short     					SS16;
typedef unsigned char    					UB08;
typedef signed char      					SB08;
typedef unsigned long    					UL32;
typedef signed long      					SL32;
typedef float            					FT32;
typedef double           					DB64;
typedef unsigned long long int              UL64;
typedef signed long long int                SL64;

typedef unsigned short   					*PUS16;
typedef signed short     					*PSS16;
typedef unsigned char    					*PUB08;
typedef signed char      					*PSB08;
typedef unsigned long    					*PUL32;
typedef signed long      					*PSL32;
typedef float            					*PFT32;
typedef double           					*PDB64;
typedef unsigned long long int              *PUL64;
typedef signed long long int                *PSL64;


//--------------------------------------------------------------------------
#define US16_MAX	(0xFFFF)
#define US16_MIN	(0x0000)
#define UB08_MAX	(0xFF)
#define UB08_MIN	(0x00)
#define UL32_MAX	(0xFFFFFFFF)
#define UL32_MIN	(0x00000000)
#define SS16_MAX	(+32767)
#define SS16_MIN	(-32768)
#define SB08_MAX	(+127)
#define SB08_MIN	(-128)
#define SL32_MAX	(+2147483647)
#define SL32_MIN	(-2147483648)
#define TRUE		(0x01)
#define FALSE		(0x00)
#define NULL		(0x00)
#define T			(1)
#define F			(0)
#define FAIL		(0x00)
#define PASS		(0x01)
//Bool
typedef enum {eFAIL = 0x00,eTRUE = 0x01,}BOOL;

//--------------------------------------------------------------------------
#ifndef MIN
	#define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif
#ifndef MAX
	#define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif
#ifndef ABS
	#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif
#define BIT(x)		(UL32)(1 << (x))
//--------------------------------------------------------------------------
//__LINE__：在源代码中插入当前源代码行号；
//__FILE__：在源文件中插入当前源文件名；
//__DATE__：在源文件中插入当前的编译日期
//__TIME__：在源文件中插入当前编译时间；
//__STDC__：当要求程序严格遵循ANSI C标准时该标识被赋值为1；
//__cplusplus：当编写C++程序时该标识符被定义。
//--------------------------------------------------------------------------
#ifdef __DEBUG__
#define DEBUG(format,...) printf("Date: "__DATE__",File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
DEBUG("%s",str);
#define dprint(X,args... ) printf( X, ##args) 
//--------------------------------------------------------------------------

#define GET_BIT_(x,y)			()

/* 大端 I:p[Index++] */
#define BR4(I,O)	{(O) = 0;(O) |= (I) << 0x18;(O) |= (I) << 0x10;(O) |= (I) << 0x08;(O) |= (I) << 0x00;}
#define BR2(I,O)	{(O) = 0;(O) |= (I) << 0x08;(O) |= (I) << 0x00;}
#define BR1(I,O)	{(O) = (I);}
/* 大端 O:p[Index++]  */
#define BW4(I,O)	{(O) = ((I)&0xFF000000) >> 0x18;(O) = ((I)&0x00FF0000) >> 0x10;(O) = ((I)&0x0000FF00) >> 0x08;(O) = ((I)&0x000000FF) >> 0x00;}
#define BW2(I,O)	{(O) = ((I)&0xFF00) >> 0x08;(O) = ((I)&0x00FF) >> 0x00;}
#define BW1(I,O)	{(O) = (I);}
/* 小端 I:p[Index++] */
#define SR4(I,O)	{(O) = 0;(O) |= (I) << 0X00;(O) |= (I) << 0X08;(O) |= (I) << 0X10;(O) |= (I) << 0X18;}
#define SR2(I,O)	{(O) = 0;(O) |= (I) << 0X00;(O) |= (I) << 0X08;}
#define SR1(I,O)	{(O) = (I);}
/* 小端 O:p[Index++] */
#define SW4(I,O)	{(O) = ((I)&0X000000FF) >> 0X00;(O) = ((I) & 0X0000FF00) >> 0X08;(O) = ((I) & 0X00FF0000) >> 0X10;(O) = ((I) & 0XFF000000) >> 0X18; }
#define SW2(I,O)	{(O) = ((I)&0X00FF) >> 0X00;(O) = ((I) & 0XFF00) >> 0X08;}
#define SW1(I,O)	{(O) = (I);}
--------------------------------------------------------------------------
#define ENUM(x)	(sizeof(x)/sizeof((x)[0]))
#define LL8(x)		(((x) & 0x000000FF) >> 0x00)
#define LH8(x)		(((x) & 0x0000FF00) >> 0x08)
#define HL8(x)		(((x) & 0x00FF0000) >> 0x10)
#define HH8(x)		(((x) & 0xFF000000) >> 0x18)
#define H16(x)      (((x) & 0xFFFF0000) >> 0x10)
#define L16(x)      (((x) & 0x0000FFFF) >> 0x00)
--------------------------------------------------------------------------














