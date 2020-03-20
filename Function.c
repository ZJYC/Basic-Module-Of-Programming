
typedef unsigned short   			US16;
typedef signed short     			SS16;
typedef unsigned char    			UB08;
typedef signed char      			SB08;
typedef unsigned long    			UL32;
typedef signed long      			SL32;
typedef float            			FT32;
typedef double           			DB64;
typedef unsigned long long int      UL64;
typedef signed long long int        SL64;

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

//代替 void
#define VOID        void
//代替 void *
#define PVOD        void*
//代替 void **
#define PPVD        void**

#define PNT0(x)        (& (x))
#define PNT1(x)        (* (x))
#define PNT2(x)        (**(x))

#define BIT(x)          (1 << (x))

#define TRUE		(0x01)
#define FALSE		(0x00)
#define NULL		(0x00)
#define T			(1)
#define F			(0)
#define FAIL		(0x00)
#define PASS		(0x01)
//进入临界区
#define CRITICAL_ENTER()	{;}
//退出临界区
#define CRITICAL_EXITX()	{;}

typedef enum {
	eFAIL = 0x00,
	eTRUE = 0x01,
}BOOL;

#pragma region 信号量操作 || 常用在主任务中
//信号量结构体
typedef struct {
	BOOL Occupied;
	US16 Cnt;
}S_Sem,*S_PSem;
//设定信号量的初始值
static BOOL SemInit(S_PSem pSem, US16 SetValue) {
	CRITICAL_ENTER();

	if (pSem == NULL || pSem->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}
    pSem->Occupied = eTRUE;
    pSem->Cnt = SetValue;
    pSem->Occupied = eFAIL;

	CRITICAL_EXITX();
	return eTRUE;
}
//获取信号量
static BOOL SemTake(S_PSem pSem) {
	UB08 Res = eFAIL;
	CRITICAL_ENTER();

	if (pSem == NULL || pSem->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}
    pSem->Occupied = eTRUE;
	if (pSem->Cnt > NULL) {
        pSem->Cnt = pSem->Cnt - 1;
		Res = eTRUE;
	}
	else {
		Res = eFAIL;
	}
    pSem->Occupied = eFAIL;
	CRITICAL_EXITX();
	return Res;
}
//释放信号量
static BOOL SemGive(S_PSem pSem) {
	CRITICAL_ENTER();

	if (pSem == NULL || pSem->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}

    pSem->Occupied = eTRUE;
    pSem->Cnt = pSem->Cnt + 1;
    pSem->Occupied = eFAIL;

	CRITICAL_EXITX();
	return eTRUE;
}
//查看信号量
static BOOL SemPeek(S_PSem pSem) {
	UB08 Res = eFAIL;
	CRITICAL_ENTER();

	if (pSem == NULL || pSem->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}

    pSem->Occupied = eTRUE;
	Res = (pSem->Cnt == NULL ? eFAIL : eTRUE);
    pSem->Occupied = eFAIL;

	CRITICAL_EXITX();
	return Res;
}
//接口
typedef struct {
    BOOL(PNT1(Init))(S_PSem pSem, US16 SetValue);
    BOOL(PNT1(Take))(S_PSem pSem);
    BOOL(PNT1(Give))(S_PSem pSem);
    BOOL(PNT1(Peek))(S_PSem pSem);
}S_SemApi;

S_SemApi SemApi = {
.Init = SemInit,
.Take = SemTake,
.Give = SemGive,
.Peek = SemPeek,
};

#pragma endregion

#pragma region 邮箱操作 || 常用在中断中

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
    PVOD MailContent;   
}S_Mail,*S_PMail;

//获取邮箱:返回邮箱内容
static BOOL MailTake(S_PMail pMail,PPVD MailContent) {
    CRITICAL_ENTER();

    if (pMail == NULL || pMail->State == eEmpty || MailContent == NULL) {
        CRITICAL_EXITX();
        return eFAIL;
    }
    
    *MailContent = pMail->MailContent;
    pMail->State = eEmpty;

    CRITICAL_EXITX();
    return eTRUE;
}
//发送邮箱:发送邮箱内容
static BOOL MailGive(S_PMail pMail,PVOD MailContent) {
    CRITICAL_ENTER();

    if (pMail == NULL || pMail->State == eFully) {
        CRITICAL_EXITX();
        return eFAIL;
    }

    pMail->MailContent = MailContent;
    pMail->State = eFully;

    CRITICAL_EXITX();
    return eTRUE;
}

typedef struct {
    BOOL(*Take)(S_PMail pMail, PPVD MailContent);
    BOOL(*Give)(S_PMail pMail, PVOD MailContent)
}S_MailApi;
S_MailApi MailApi = {
.Take = MailTake,
.Give = MailGive,
};

/* 使用样例 */
//全局声明：
Mail MailUsartRecvdByte = { NULL };
//在中断中：
static UB08 RecvdByte = NULL;
RecvdByte = UsartGetChar();
MailApi.Give(PNT0 MailUsartRecvdByte, (VOID PNT1)(PNT0 RecvdByte));
//在主体函数中：
UB08 RecvdByte = NULL;
if (MailApi.Take(PNT0 MailUsartRecvdByte, (PPVD)(PNT0 RecvdByte)) == eTRUE) {

}


#pragma endregion

#pragma region 函数模块

typedef struct{/* 底桩实现 */
	VOID (*Init)(VOID);
}XX_ModuleImport;
typedef struct{/* 驱动模块 */
	VOID (*Init)(VOID);
}XX_ModuleExport;
typedef struct{/* 临时数据 */
	UL32 Reserve;
}XX_ModuleTmpdat;
typedef struct{/* 模块结构 */
	XX_ModuleImport Import;
	XX_ModuleExport Export;
	XX_ModuleTmpdat Tmpdat;
}XX_Module;

#pragma endregion

#pragma region 函数回调

typedef enum {
	eJustExit = 0x01,//没有必要传给下个函数了，此标志为独享
	eKeepPass = 0x02,//继续传递给以后的函数
}E_CallbackRes;

typedef E_CallbackRes(*PCbkFUNC)(UL32 Msk, PVOD pMsg);

typedef struct {
    /* 回调函数 */
    PCbkFUNC pCbkFUNC;
    /* 掩码 */
	UL32 Mask;
    /* 被占用 */
	BOOL Occupied;
}S_CbkEntry,*S_PCbkEntry;

typedef struct {
    /* 回调项 */
    S_PCbkEntry Entry;
    /* 回调总个数 */
	UL32 EntryCount;  
}S_CbkMgt,*S_PCbkMgt;

static BOOL CallbackExe(S_PCbkMgt pCbkMgt, UL32 Mask, UL32 pMsg) {
    E_CallbackRes Res = eJustExit;
	if (pCbkMgt == NULL)return eFAIL;
	for (UL32 i = 0; i < pCbkMgt->EntryCount; i++) {
		if (pCbkMgt->Entry[i].Mask & Mask) {
			if (pCbkMgt->Entry[i].pCbkFUNC != NULL && pCbkMgt->Entry[i].Occupied == eFAIL) {
                pCbkMgt->Entry[i].Occupied = eTRUE;
				Res = pCbkMgt->Entry[i].pCbkFUNC(Mask, pMsg);
                pCbkMgt->Entry[i].Occupied = eFAIL;
				if (Res == eJustExit)return eTRUE;
			}
			if (pCbkMgt->Entry[i].pCbkFUNC != NULL && pCbkMgt->Entry[i].Occupied == eTRUE) {
				//不应该调用自己
			}
		}
	}
	return eTRUE;
}

static BOOL CallbackAdd(S_PCbkMgt pCbkMgt, UL32 Mask, PCbkFUNC pCbkFUNC) {
    if (pCbkMgt == NULL)return eFAIL;

    for (UL32 i = 0; i < pCbkMgt->EntryCount; i++) {
        if (pCbkMgt->Entry[i].pCbkFUNC == NULL) {
            pCbkMgt->Entry[i].pCbkFUNC = pCbkFUNC;
            pCbkMgt->Entry[i].Mask = Mask;
            return eTRUE;
        }
    }

    return eFAIL;
}

typedef struct {
    BOOL (*Exe)(S_PCbkMgt pCbkMgt, UL32 Mask, UL32 pMsg);
    BOOL (*Add)(S_PCbkMgt pCbkMgt, UL32 Mask, PCbkFUNC pCbkFUNC)
}S_CbkApi;

S_CbkApi CbkApi = {
.Exe = CallbackExe,
.Add = CallbackAdd,
};

/* 使用样例 */

S_CbkEntry TimerEntry[10] = { NULL };
S_CbkMgt Timer1BkGroup = { TimerEntry ,10 };
#define MASK_FUNC   (BIT(1))
static E_CallbackRes FUNC(UL32 Msk, PVOD pMsg) {

}
CallbackAdd(PNT0(Timer1BkGroup), MASK_FUNC,FUNC);
//调用者
CallbackExe(PNT0(Timer1BkGroup), MASK_FUNC, NULL);


#pragma endregion

#if 1/* 状态机 */
static UL32 Step = NULL;
#define STEP_SETX(x)	{Step =  (x);}
#define STEP_NEXT(x)	{Step += (x);}
switch(Step){
	case 0x00:
	STEP_SETX(0x01);
	break;
	case 0x01:
	STEP_NEXT(0x01)
	break;
	case 0x02:
	break;
	default:
	break;
}
#endif

#pragma region Convert
typedef struct {
    UB08*(*US16_2_UB08)(US16 Input, BOOL Reverse);
    UB08*(*SS16_2_UB08)(SS16 Input, BOOL Reverse);
    UB08*(*UL32_2_UB08)(UL32 Input, BOOL Reverse);
    UB08*(*SL32_2_UB08)(SL32 Input, BOOL Reverse);
    UB08*(*FT32_2_UB08)(FT32 Input, BOOL Reverse);
    US16(*UB08_2_US16)(UB08* Input, BOOL Reverse);
    SS16(*UB08_2_SS16)(UB08* Input, BOOL Reverse);
    UL32(*UB08_2_UL32)(UB08* Input, BOOL Reverse);
    SL32(*UB08_2_SL32)(UB08* Input, BOOL Reverse);
    FT32(*UB08_2_FT32)(UB08* Input, BOOL Reverse);
}S_Convert;

static UB08* US16_2_UB08(US16 Input, BOOL Reverse) {
    UB08 Temp[2] = { NULL };

    if (Reverse == eFAIL) {
        Temp[0] = (Input & 0xFF00) >> 8;
        Temp[1] = (Input & 0x00FF) >> 0;
    }
    else {
        Temp[1] = (Input & 0xFF00) >> 8;
        Temp[0] = (Input & 0x00FF) >> 0;
    }

    return Temp;
}
static UB08* SS16_2_UB08(SS16 Input, BOOL Reverse) {
    UB08 Temp[2] = { NULL };

    if (Reverse == eFAIL) {
        Temp[0] = (Input & 0xFF00) >> 8;
        Temp[1] = (Input & 0x00FF) >> 0;
    }
    else {
        Temp[1] = (Input & 0xFF00) >> 8;
        Temp[0] = (Input & 0x00FF) >> 0;
    }

    return Temp;
}
static UB08* UL32_2_UB08(UL32 Input, BOOL Reverse) {
    UB08 Temp[4] = { NULL };

    if (Reverse == eFAIL) {
        Temp[0] = (Input & 0xFF000000) >> 0x18;
        Temp[1] = (Input & 0x00FF0000) >> 0x10;
        Temp[2] = (Input & 0x0000FF00) >> 0x08;
        Temp[3] = (Input & 0x000000FF) >> 0x00;
    }
    else {
        Temp[3] = (Input & 0xFF000000) >> 0x18;
        Temp[2] = (Input & 0x00FF0000) >> 0x10;
        Temp[1] = (Input & 0x0000FF00) >> 0x08;
        Temp[0] = (Input & 0x000000FF) >> 0x00;
    }

    return Temp;
}
static UB08* SL32_2_UB08(SL32 Input, BOOL Reverse) {
    UB08 Temp[4] = { NULL };

    if (Reverse == eFAIL) {
        Temp[0] = (Input & 0xFF000000) >> 0x18;
        Temp[1] = (Input & 0x00FF0000) >> 0x10;
        Temp[2] = (Input & 0x0000FF00) >> 0x08;
        Temp[3] = (Input & 0x000000FF) >> 0x00;
    }
    else {
        Temp[3] = (Input & 0xFF000000) >> 0x18;
        Temp[2] = (Input & 0x00FF0000) >> 0x10;
        Temp[1] = (Input & 0x0000FF00) >> 0x08;
        Temp[0] = (Input & 0x000000FF) >> 0x00;
    }

    return Temp;
}
static UB08* FT32_2_UB08(FT32 Input, BOOL Reverse) {
    UB08 Temp[4] = { NULL };
    UB08* p = (UB08*)&Input;

    if (Reverse == eFAIL) {
        Temp[0] = p[3];
        Temp[1] = p[2];
        Temp[2] = p[1];
        Temp[3] = p[0];
    }
    else {
        Temp[0] = p[0];
        Temp[1] = p[1];
        Temp[2] = p[2];
        Temp[3] = p[3];
    }

    return Temp;
}
static US16  UB08_2_US16(UB08* Input, BOOL Reverse) {
    if (Input == NULL)return NULL;
    US16 Temp = NULL;

    if (Reverse == eTRUE) {
        Temp |= (Input[0] << 0x08);
        Temp |= (Input[1] << 0x00);
    }
    else {
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static SS16  UB08_2_SS16(UB08* Input, BOOL Reverse) {
    if (Input == NULL)return NULL;
    SS16 Temp = NULL;

    if (Reverse == eTRUE) {
        Temp |= (Input[0] << 0x08);
        Temp |= (Input[1] << 0x00);
    }
    else {
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static UL32  UB08_2_UL32(UB08* Input, BOOL Reverse) {
    if (Input == NULL)return NULL;
    UL32 Temp = NULL;

    if (Reverse == eTRUE) {
        Temp |= (Input[0] << 0x18);
        Temp |= (Input[1] << 0x10);
        Temp |= (Input[2] << 0x08);
        Temp |= (Input[3] << 0x00);
    }
    else {
        Temp |= (Input[3] << 0x18);
        Temp |= (Input[2] << 0x10);
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static SL32  UB08_2_SL32(UB08* Input, BOOL Reverse) {
    if (Input == NULL)return NULL;
    SL32 Temp = NULL;

    if (Reverse == eTRUE) {
        Temp |= (Input[0] << 0x18);
        Temp |= (Input[1] << 0x10);
        Temp |= (Input[2] << 0x08);
        Temp |= (Input[3] << 0x00);
    }
    else {
        Temp |= (Input[3] << 0x18);
        Temp |= (Input[2] << 0x10);
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static FT32  UB08_2_FT32(UB08* Input, BOOL Reverse) {
    if (Input == NULL)return NULL;
    FT32 Temp = NULL;
    UB08* p = (UB08*)&Temp;

    if (Reverse == eTRUE) {
        p[0] = Input[0];
        p[1] = Input[1];
        p[2] = Input[2];
        p[3] = Input[3];
    }
    else {
        p[0] = Input[3];
        p[1] = Input[2];
        p[2] = Input[1];
        p[3] = Input[0];
    }

    return Temp;
}

S_Convert Convert = {
US16_2_UB08,
SS16_2_UB08,
UL32_2_UB08,
SL32_2_UB08,
FT32_2_UB08,
UB08_2_US16,
UB08_2_SS16,
UB08_2_UL32,
UB08_2_SL32,
UB08_2_FT32,
};

#pragma endregion



#if 1/* 另类字符串转数 */
static UL32 S2B()
#endif







