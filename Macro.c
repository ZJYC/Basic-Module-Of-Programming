--------------------------------------------------------------------------
#define CONVERT32(x)	(((x) & 0x000000FF) << 24 |\((x) & 0x0000FF00) << 8 |\((x) & 0x00FF0000) >> 8 |\((x) & 0xFF000000) >> 24)
#define CONVERT16(x)	(((x) & 0x00FF) << 8 | ((x) & 0xFF00) >> 8)
#ifdef CPU_Length_32
--------------------------------------------------------------------------
#define US16	unsigned short
#define SS16	signed short
#define UB08	unsigned char
#define SB08	signed char
#define UL32	unsigned long
#define SL32	signed long
#define FT32	float
#define DB64	double
--------------------------------------------------------------------------
typedef unsigned short   					US16;
typedef signed short     					SS16;
typedef unsigned char    					UB08;
typedef signed char      					SB08;
typedef unsigned long    					UL32;
typedef signed long      					SL32;
typedef float            					FT32;
typedef double           					DB64;
--------------------------------------------------------------------------
typedef volatile unsigned char    			VU08;
typedef volatile signed char    			VS08;
typedef volatile unsigned short   			VU16;
typedef volatile signed short     			VS16;
typedef volatile unsigned long    			VU32;
typedef volatile signed long      			VS32;
typedef volatile unsigned long long  		VU64;
typedef volatile signed long long  			VS64;
typedef volatile float						VF32;
typedef volatile double						VF64;
--------------------------------------------------------------------------
typedef unsigned char    					U08;
typedef signed char    						S08;
typedef unsigned short   					U16;
typedef signed short     					S16;
typedef unsigned long    					U32;
typedef signed long      					S32;
typedef unsigned long long  				U64;
typedef signed long long  					S64;
typedef float								F32;
typedef double								F64;
--------------------------------------------------------------------------
typedef const unsigned char    				CU08;
typedef const signed char					CS08;
typedef const unsigned short   				CU16;
typedef const signed short     				CS16;
typedef const unsigned long    				CU32;
typedef const signed long      				CS32;
typedef const unsigned long long  			CU64;
typedef const signed long long  			CS64;
typedef const float							CF32;
typedef const double						CF64;
--------------------------------------------------------------------------
typedef volatile const unsigned char		VCU08;
typedef volatile const signed char			VCS08;
typedef volatile const unsigned short		VCU16;
typedef volatile const signed short			VCS16;
typedef volatile const unsigned long		VCU32;
typedef volatile const signed long			VCS32;
typedef volatile const unsigned long long	VCU64;
typedef volatile const signed long long		VCS64;
typedef volatile const float				VCF32;
typedef volatile const double				VCF64;
--------------------------------------------------------------------------
#define U16_MAX	(0xFFFF)
#define U16_MIN	(0x0000)
#define U08_MAX	(0xFF)
#define U08_MIN	(0x00)
#define U32_MAX	(0xFFFFFFFF)
#define U32_MIN	(0x00000000)
#define S16_MAX	(+32767)
#define S16_MIN	(-32768)
#define S08_MAX	(+127)
#define S08_MIN	(-128)
#define S32_MAX	(+2147483647)
#define S32_MIN	(-2147483648)
#define TRUE		(0x01)
#define FALSE		(0x00)
#define NULL		(0x00)
#define T			(1)
#define F			(0)
#define FAIL		(0x00)
#define PASS		(0x01)
--------------------------------------------------------------------------
#define US16_NULL	(0)
#define SS16_NULL	(0)
#define UB08_NULL	(0)
#define SB08_NULL	(0)
#define UL32_NULL	(0)
#define SL32_NULL	(0)
#define FT32_NULL	(0)
#define DB64_NULL	(0)
#endif
--------------------------------------------------------------------------
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
--------------------------------------------------------------------------
__LINE__：在源代码中插入当前源代码行号；
__FILE__：在源文件中插入当前源文件名；
__DATE__：在源文件中插入当前的编译日期
__TIME__：在源文件中插入当前编译时间；
__STDC__：当要求程序严格遵循ANSI C标准时该标识被赋值为1；
__cplusplus：当编写C++程序时该标识符被定义。
--------------------------------------------------------------------------
#ifdef __DEBUG__
#define DEBUG(format,...) printf("Date: "__DATE__",File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
DEBUG("%s",str);
#define dprint(X,args... ) printf( X, ##args) 
--------------------------------------------------------------------------
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














