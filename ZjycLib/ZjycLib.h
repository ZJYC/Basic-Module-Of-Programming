#ifndef _ZjycLib_h_
#define _ZjycLib_h_

#include "DataType.h"

#pragma region 内存相关
typedef struct {
	EBOOL(*MemCpy)(PUB08 Src, PUB08 Dst, BUL32 Len, EBOOL Reverse);
	EBOOL(*MemCmp)(PUB08 Mem1, PUB08 Mem2, BUL32 Len);
}S_Mem;
extern S_Mem Mem;
#pragma endregion
#pragma region 字序转换相关
typedef enum {
	BigOrder = 0,
	SmlOrder = 1,
}E_ByteOrder;
typedef struct {
	PUB08(*US16_2_UB08)(BUS16 Input, E_ByteOrder OutputOrder);
	PUB08(*SS16_2_UB08)(BSS16 Input, E_ByteOrder OutputOrder);
	PUB08(*UL32_2_UB08)(BUL32 Input, E_ByteOrder OutputOrder);
	PUB08(*SL32_2_UB08)(BSL32 Input, E_ByteOrder OutputOrder);
	PUB08(*FT32_2_UB08)(BFT32 Input, E_ByteOrder OutputOrder);
	BUS16(*UB08_2_US16)(PUB08 Input, E_ByteOrder InputOrderx);
	BSS16(*UB08_2_SS16)(PUB08 Input, E_ByteOrder InputOrderx);
	BUL32(*UB08_2_UL32)(PUB08 Input, E_ByteOrder InputOrderx);
	BSL32(*UB08_2_SL32)(PUB08 Input, E_ByteOrder InputOrderx);
	BFT32(*UB08_2_FT32)(PUB08 Input, E_ByteOrder InputOrderx);
	PUB08(*US16_2_UB08X)(BUS16 Input, PUB08 Output, E_ByteOrder OutputOrder);
	PUB08(*SS16_2_UB08X)(BSS16 Input, PUB08 Output, E_ByteOrder OutputOrder);
	PUB08(*UL32_2_UB08X)(BUL32 Input, PUB08 Output, E_ByteOrder OutputOrder);
	PUB08(*SL32_2_UB08X)(BSL32 Input, PUB08 Output, E_ByteOrder OutputOrder);
	PUB08(*FT32_2_UB08X)(BFT32 Input, PUB08 Output, E_ByteOrder OutputOrder);

	E_ByteOrder LocalCpuOrder;
}S_Convert;
extern S_Convert Convert;
#pragma endregion
#pragma region 哈希表
typedef struct {
	BUL32(*JSHash)(PUB08 Data, BUL32 Length);
}S_JsHashApi;

extern S_JsHashApi JsHashApi;
#pragma endregion
#pragma region CRC
typedef struct {
	BUS16(*CRC16)(PUB08 pucFrame, BUS16 usLen);
}S_CrcApi;
extern S_CrcApi CrcApi;
#pragma endregion
#pragma region Sem
//信号量结构体
typedef struct {
	EBOOL Occupied;
	BUS16 Cnt;
	BUS16 Max;
}S_Sem, *PS_Sem;
//接口
typedef struct {
	EBOOL(*Init)(PS_Sem pSem, US16 SetValue);
	EBOOL(*Take)(PS_Sem pSem);
	EBOOL(*Give)(PS_Sem pSem);
	EBOOL(*Peek)(PS_Sem pSem);
}S_SemApi;
extern S_SemApi SemApi;
#pragma endregion
#pragma region Mail
typedef enum {
	/* 邮箱空 */
	eEmpty = 0,
	/* 邮箱满 */
	eFully = 1,
}E_MailState;
typedef struct {
	/* 邮箱状态:空|满 */
	E_MailState State;
	/* 邮箱内容 */
	BUL32 MailContent;
}S_Mail, *S_PMail;
typedef struct {
	EBOOL(*Take)(S_PMail pMail, PPVOD MailContent);
	EBOOL(*Give)(S_PMail pMail, PVOID MailContent);
}S_MailApi;
extern S_MailApi MailApi;
typedef struct {
	BUB08(*GetValOfPosSL32)(BSL32 Input, BUB08 Pos, BUB08 Base);
	BSL32(*SetValOfPosSL32)(BSL32 Input, BUB08 Pos, BUB08 Base, BUB08 Val);
	BUB08(*GetValOfPosDB64)(BDB64 Input, BSB08 Pos);
	BDB64(*SetValOfPosDB64)(BDB64 Input, BSB08 Pos, BUB08 Val);
	BUL32(*Distance)(BUL32 Smaller, BUL32 Bigger, BUL32 Max);
	BDB64(*Amplify)(BDB64 Input, BSB08 Factor_Log10);
}S_Num;
extern S_Num Num;

#pragma endregion
#pragma region Ringbuff
typedef struct {
	PUB08 BufferPointer;
	BUS16 BufferSize;
	BUS16 FilledSize;
	PUB08 ReadPointer;
	PUB08 WritPointer;
}S_RingBuff, *S_PRingBuff;

typedef struct {
	BUS16(*Init)(S_PRingBuff pRingBuff);
	BUS16(*Read)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
	BUS16(*Peek)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
	BUS16(*Writ)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
	BUS16(*GetFree)(S_PRingBuff pRingBuff);
	BUS16(*GetUsed)(S_PRingBuff pRingBuff);
}S_RingBuffApi;

extern S_RingBuffApi RingBuffApi;

#define RINGBUFFER_NEW(name, BufferSize)        \
        static BUB08 ringmem##name[BufferSize]; \
        S_RingBuff name = {ringmem##name, (BufferSize), 0, ringmem##name, ringmem##name};

#define RINGBUFFER_EXTERN(name) extern S_RingBuff name;

#pragma endregion
#pragma region 函数回调
typedef enum {
	eJustExit = 0x01,//没有必要传给下个函数了，此标志为独享
	eKeepPass = 0x02,//继续传递给以后的函数
}E_CallbackRes;

typedef E_CallbackRes(*PCbkFUNC)(UL32 Msk, PVOID pMsg);

typedef struct {
	/* 回调函数 */
	PCbkFUNC pCbkFUNC;
	/* 掩码 */
	UL32 Mask;
	/* 被占用 */
	EBOOL Occupied;
}S_CbkEntry, *S_PCbkEntry;

typedef struct {
	/* 回调项 */
	S_PCbkEntry Entry;
	/* 回调总个数 */
	UL32 EntryCount;
}S_CbkMgt, *PS_CbkMgt;

typedef struct {
	EBOOL(*Exe)(PS_CbkMgt pCbkMgt, UL32 Mask, UL32 pMsg);
	EBOOL(*Add)(PS_CbkMgt pCbkMgt, UL32 Mask, PCbkFUNC pCbkFUNC);
}S_CbkApi;

extern S_CbkApi CbkApi;
#pragma endregion
#pragma region 查表法
typedef struct {
	FT32 x;
	FT32 y;
}S_TableItem, *PS_TableItem;
typedef struct {
	BFT32(*Search)(PS_TableItem pS_TableItem, BUS16 MaxCount, BFT32 x);
}S_TableApi;
extern S_TableApi TableApi;
#pragma endregion
#pragma region 软件看门狗

typedef EBOOL(*DogCbkFunc)(BUB08 Typ);

typedef struct {
	EBOOL Enable;
	BUL32 SelfCnt;
	BUL32 MaxCnt;
	DogCbkFunc Cbkf;
}S_SoftDog, *PS_SoftDog;

typedef struct {
	EBOOL(*Init)(PS_SoftDog pS_SoftDog, BUL32 Max);
	EBOOL(*Loop)(PS_SoftDog pS_SoftDog, BUS16 Interval);
	EBOOL(*Feed)(PS_SoftDog pS_SoftDog);
}S_SoftDogApi;

extern S_SoftDogApi SoftDogApi;

#pragma endregion













#endif
