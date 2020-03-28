
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

#define TRUE        (0x01)
#define FALSE       (0x00)
#define NULL        (0x00)
#define T           (1)
#define F           (0)
#define FAIL        (0x00)
#define PASS        (0x01)
//进入临界区
#define CRITICAL_ENTER()    {;}
//退出临界区
#define CRITICAL_EXITX()    {;}

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
    BOOL(*Init)(S_PSem pSem, US16 SetValue);
    BOOL(*Take)(S_PSem pSem);
    BOOL(*Give)(S_PSem pSem);
    BOOL(*Peek)(S_PSem pSem);
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
    UL32 HashCode;
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

#pragma region 状态机
static UL32 Step = NULL;
#define STEP_SETX(x)    {Step =  (x);}
#define STEP_NEXT(x)    {Step += (x);}
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
#pragma endregion

#pragma region 字序转换
typedef enum {
    BigOrder = 0,
    SmlOrder = 1,
}E_ByteOrder;

static PUB08 US16_2_UB08(BUS16 Input, E_ByteOrder OutputOrder);
static PUB08 SS16_2_UB08(BSS16 Input, E_ByteOrder OutputOrder);
static PUB08 UL32_2_UB08(BUL32 Input, E_ByteOrder OutputOrder);
static PUB08 SL32_2_UB08(BSL32 Input, E_ByteOrder OutputOrder);
static PUB08 FT32_2_UB08(BFT32 Input, E_ByteOrder OutputOrder);
static BUS16 UB08_2_US16(PUB08 Input, E_ByteOrder InputOrderx);
static BSS16 UB08_2_SS16(PUB08 Input, E_ByteOrder InputOrderx);
static BUL32 UB08_2_UL32(PUB08 Input, E_ByteOrder InputOrderx);
static BSL32 UB08_2_SL32(PUB08 Input, E_ByteOrder InputOrderx);
static BFT32 UB08_2_FT32(PUB08 Input, E_ByteOrder InputOrderx);

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
    E_ByteOrder LocalCpuOrder;
}S_Convert;

S_Convert Convert = {
US16_2_UB08,SS16_2_UB08,
UL32_2_UB08,SL32_2_UB08,
FT32_2_UB08,
UB08_2_US16,UB08_2_SS16,
UB08_2_UL32,UB08_2_SL32,
UB08_2_FT32,
SmlOrder,
};

static PUB08 US16_2_UB08(BUS16 Input, E_ByteOrder OutputOrder) {
    //Input 0x1234,
    //OutputOrder:Big ==> [12] [34]
    //OutputOrder:Sml ==> [34] [12]
    UB08 Temp[2] = { NULL };

    if (OutputOrder == BigOrder) {
        Temp[0] = (Input & 0xFF00) >> 8;
        Temp[1] = (Input & 0x00FF) >> 0;
    }
    else {
        Temp[1] = (Input & 0xFF00) >> 8;
        Temp[0] = (Input & 0x00FF) >> 0;
    }

    return Temp;
}
static PUB08 SS16_2_UB08(BSS16 Input, E_ByteOrder OutputOrder) {
    //Input 0x1234,
    //OutputOrder:Big ==> [12] [34]
    //OutputOrder:Sml ==> [34] [12]

    UB08 Temp[2] = { NULL };

    if (OutputOrder == BigOrder) {
        Temp[0] = (Input & 0xFF00) >> 8;
        Temp[1] = (Input & 0x00FF) >> 0;
    }
    else {
        Temp[1] = (Input & 0xFF00) >> 8;
        Temp[0] = (Input & 0x00FF) >> 0;
    }

    return Temp;
}
static PUB08 UL32_2_UB08(BUL32 Input, E_ByteOrder OutputOrder) {
    //Input 0x12345678
    //OutputOrder:Big ==> [12] [34] [56] [78]
    //OutputOrder:Sml ==> [78] [56] [34] [12]

    UB08 Temp[4] = { NULL };

    if (OutputOrder == BigOrder) {
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
static PUB08 SL32_2_UB08(BSL32 Input, E_ByteOrder OutputOrder) {
    //Input 0x12345678
    //OutputOrder:Big ==> [12] [34] [56] [78]
    //OutputOrder:Sml ==> [78] [56] [34] [12]

    UB08 Temp[4] = { NULL };

    if (OutputOrder == BigOrder) {
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
static PUB08 FT32_2_UB08(BFT32 Input, E_ByteOrder OutputOrder) {
    //Input 1.0
    //LocalOrder:Big ==> [3F] [80] [00] [00] Output:Big ==> [3F] [80] [00] [00]
    //LocalOrder:Big ==> [3F] [80] [00] [00] Output:Sml ==> [00] [00] [80] [3F]
    //LocalOrder:Sml ==> [00] [00] [80] [3F] Output:Big ==> [3F] [80] [00] [00]
    //LocalOrder:Sml ==> [00] [00] [80] [3F] Output:Sml ==> [00] [00] [80] [3F]

    BUB08 Temp[4] = { NULL };
    PUB08 p = PUB08(&Input);

    if (OutputOrder == BigOrder && Convert.LocalCpuOrder == BigOrder) {
        Temp[0] = p[0];
        Temp[1] = p[1];
        Temp[2] = p[2];
        Temp[3] = p[3];
    }
    if (OutputOrder == BigOrder && Convert.LocalCpuOrder == SmlOrder) {
        Temp[0] = p[3];
        Temp[1] = p[2];
        Temp[2] = p[1];
        Temp[3] = p[0];
    }
    if (OutputOrder == SmlOrder && Convert.LocalCpuOrder == BigOrder) {
        Temp[0] = p[3];
        Temp[1] = p[2];
        Temp[2] = p[1];
        Temp[3] = p[0];
    }
    if (OutputOrder == SmlOrder && Convert.LocalCpuOrder == SmlOrder) {
        Temp[0] = p[0];
        Temp[1] = p[1];
        Temp[2] = p[2];
        Temp[3] = p[3];
    }

    return Temp;
}
static BUS16 UB08_2_US16(PUB08 Input, E_ByteOrder InputOrderx) {
    //Input [12][34]
    //OutputOrder:Big ==> [1234]
    //OutputOrder:Sml ==> [3412]

    if (Input == NULL)return NULL;
    BUS16 Temp = NULL;

    if (InputOrderx == BigOrder) {
        Temp |= (Input[0] << 0x08);
        Temp |= (Input[1] << 0x00);
    }
    if (InputOrderx == SmlOrder) {
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static BSS16 UB08_2_SS16(PUB08 Input, E_ByteOrder InputOrderx) {
    //Input [12][34]
    //OutputOrder:Big ==> [1234]
    //OutputOrder:Sml ==> [3412]

    if (Input == NULL)return NULL;
    SS16 Temp = NULL;

    if (InputOrderx == BigOrder) {
        Temp |= (Input[0] << 0x08);
        Temp |= (Input[1] << 0x00);
    }
    if (InputOrderx == SmlOrder){
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static BUL32 UB08_2_UL32(PUB08 Input, E_ByteOrder InputOrderx) {
    //Input [12][34][56][78]
    //OutputOrder:Big ==> [12345678]
    //OutputOrder:Sml ==> [78563412]

    if (Input == NULL)return NULL;
    UL32 Temp = NULL;

    if (InputOrderx == BigOrder) {
        Temp |= (Input[0] << 0x18);
        Temp |= (Input[1] << 0x10);
        Temp |= (Input[2] << 0x08);
        Temp |= (Input[3] << 0x00);
    }
    if (InputOrderx == SmlOrder) {
        Temp |= (Input[3] << 0x18);
        Temp |= (Input[2] << 0x10);
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static BSL32 UB08_2_SL32(PUB08 Input, E_ByteOrder InputOrderx) {
    //Input [12][34][56][78]
    //OutputOrder:Big ==> [12345678]
    //OutputOrder:Sml ==> [78563412]

    if (Input == NULL)return NULL;
    SL32 Temp = NULL;

    if (InputOrderx == BigOrder) {
        Temp |= (Input[0] << 0x18);
        Temp |= (Input[1] << 0x10);
        Temp |= (Input[2] << 0x08);
        Temp |= (Input[3] << 0x00);
    }
    if (InputOrderx == SmlOrder) {
        Temp |= (Input[3] << 0x18);
        Temp |= (Input[2] << 0x10);
        Temp |= (Input[1] << 0x08);
        Temp |= (Input[0] << 0x00);
    }

    return Temp;
}
static BFT32 UB08_2_FT32(PUB08 Input, E_ByteOrder InputOrderx) {
    //Input [3F][80][00][00]
    //Local:Sml ==> 
    if (Input == NULL)return NULL;
    BFT32 Temp = NULL;
    PUB08 p = PUB08(&Temp);

    if (InputOrderx == BigOrder && Convert.LocalCpuOrder == BigOrder) {
        p[0] = Input[0];
        p[1] = Input[1];
        p[2] = Input[2];
        p[3] = Input[3];
    }
    if (InputOrderx == BigOrder && Convert.LocalCpuOrder == SmlOrder) {
        p[0] = Input[3];
        p[1] = Input[2];
        p[2] = Input[1];
        p[3] = Input[0];
    }
    if (InputOrderx == SmlOrder && Convert.LocalCpuOrder == BigOrder) {
        p[0] = Input[3];
        p[1] = Input[2];
        p[2] = Input[1];
        p[3] = Input[0];
    }
    if (InputOrderx == SmlOrder && Convert.LocalCpuOrder == SmlOrder) {
        p[0] = Input[0];
        p[1] = Input[1];
        p[2] = Input[2];
        p[3] = Input[3];
    }

    return Temp;
}

#pragma endregion

#pragma region 环形缓冲区

#include <assert.h>
#include <string.h>

#define RINGBUFFER_NEW(name, BufferSize)        \
        static BUB08 ringmem##name[BufferSize]; \
        S_RingBuff name = {ringmem##name, (BufferSize), 0, ringmem##name, ringmem##name};

#define RINGBUFFER_EXTERN(name) extern RingBuff_t name;

typedef struct {
    PUB08 BufferPointer;
    BUS16 BufferSize;
    BUS16 FilledSize;
    PUB08 ReadPointer;
    PUB08 WritPointer;
}S_RingBuff,*S_PRingBuff;

static BUS16 RB_Init(S_PRingBuff pRingBuff) {
    assert(pRingBuff);
    pRingBuff->WritPointer = pRingBuff->BufferPointer;
    pRingBuff->ReadPointer = pRingBuff->BufferPointer;
    return 0;
}
static BUS16 RB_Read(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len) {
    assert(Len > 0);
    assert(pRingBuff);
    //__disable_irq();
    if (pRingBuff->FilledSize >= Len) {
        if (pRingBuff->WritPointer > pRingBuff->ReadPointer) {
            memcpy(Buff, pRingBuff->ReadPointer, Len);
            pRingBuff->ReadPointer += Len;
        }
        else if (pRingBuff->WritPointer < pRingBuff->ReadPointer) {
            BUS16 len1 = pRingBuff->BufferPointer + pRingBuff->BufferSize - 1 - pRingBuff->ReadPointer + 1;
            if (len1 >= Len) {
                memcpy(Buff, pRingBuff->ReadPointer, Len);
                pRingBuff->ReadPointer += Len;
            }
            else {
                BUS16 len2 = Len - len1;
                memcpy(Buff, pRingBuff->ReadPointer, len1);
                memcpy(Buff + len1, pRingBuff->BufferPointer, len2);
                pRingBuff->ReadPointer = pRingBuff->BufferPointer + len2; // Wrap around
            }
        }
        pRingBuff->FilledSize -= Len;
        //__enable_irq();
        return Len;
    }
    else {
        //__enable_irq();
        return 0;
    }
}
static BUS16 RB_Peek(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len) {
    assert(Len > 0);
    assert(pRingBuff);
    //__disable_irq();
    if (pRingBuff->FilledSize >= Len) {
        if (pRingBuff->WritPointer > pRingBuff->ReadPointer) {
            memcpy(Buff, pRingBuff->ReadPointer, Len);
        }
        else if (pRingBuff->WritPointer < pRingBuff->ReadPointer) {
            BUS16 len1 = pRingBuff->BufferPointer + pRingBuff->BufferSize - 1 - pRingBuff->ReadPointer + 1;
            if (len1 >= Len) {
                memcpy(Buff, pRingBuff->ReadPointer, Len);
            }
            else {
                BUS16 len2 = Len - len1;
                memcpy(Buff, pRingBuff->ReadPointer, len1);
                memcpy(Buff + len1, pRingBuff->BufferPointer, len2);
            }
        }
        //__enable_irq();
        return Len;
    }
    else {
        //__enable_irq();
        return 0;
    }
}
static BUS16 RB_Writ(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len) {
    assert(Len > 0);
    assert(pRingBuff);
    //__disable_irq();
    if (pRingBuff->BufferSize - pRingBuff->FilledSize < Len) {
        //__enable_irq();
        return 0;
    }
    else {
        if (pRingBuff->WritPointer >= pRingBuff->ReadPointer) {
            BUS16 len1 = pRingBuff->BufferPointer + pRingBuff->BufferSize - pRingBuff->WritPointer;
            if (len1 >= Len) {
                memcpy(pRingBuff->WritPointer, Buff, Len);
                pRingBuff->WritPointer += Len;
            }
            else {
                BUS16 len2 = Len - len1;
                memcpy(pRingBuff->WritPointer, Buff, len1);
                memcpy(pRingBuff->BufferPointer, Buff + len1, len2);
                pRingBuff->WritPointer = pRingBuff->BufferPointer + len2; // Wrap around
            }
        }
        else {
            memcpy(pRingBuff->WritPointer, Buff, Len);
            pRingBuff->WritPointer += Len;
        }
        pRingBuff->FilledSize += Len;
        //__enable_irq();
        return Len;
    }
}
static BUS16 RB_GetFree(S_PRingBuff pRingBuff) {
    return pRingBuff->BufferSize - pRingBuff->FilledSize;
}
static BUS16 RB_GetUsed(S_PRingBuff pRingBuff) {
    return pRingBuff->FilledSize;
}

typedef struct {
    BUS16(*Init)(S_PRingBuff pRingBuff);
    BUS16(*Read)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
    BUS16(*Peek)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
    BUS16(*Writ)(S_PRingBuff pRingBuff, PUB08 Buff, BUS16 Len);
    BUS16(*GetFree)(S_PRingBuff pRingBuff);
    BUS16(*GetUsed)(S_PRingBuff pRingBuff);
}S_RingBuffApi;

S_RingBuffApi Rb = {
.Init = RB_Init,
.Read = RB_Read,
.Peek = RB_Peek,
.Writ = RB_Writ,
.GetFree = RB_GetFree,
.GetUsed = RB_GetUsed,
};

//使用样例

RINGBUFFER_NEW(Tst, 1024);








#pragma endregion

#pragma region 数字操作

//GetValOfPos(123456,2,10) ==> 4
//GetValOfPos(0x123456,2,16) ==> 4
static BUB08 GetValOfPosSL32(BSL32 Input, BUB08 Pos, BUB08 Base) {
    if (Base == NULL)return NULL;

    BUB08 i = NULL, Res = NULL;
    BSL32 Temp = Input, Divider = 1;

    while ((i++) < Pos) {
        Divider *= Base;
    }

    Res = Temp / Divider % Base;

    return Res;

}
//SetValOfPos(123456,2,10,6) ==> 123656
static BSL32 SetValOfPosSL32(BSL32 Input, BUB08 Pos, BUB08 Base,BUB08 Val) {
    if (Base == NULL)return NULL;

    BUB08 MaxLen = 10, i = NULL, Res = NULL;
    BSL32 Temp = Input, Divider = 1;

    while ((i++) < Pos) {
        Divider *= Base;
    }

    Res = Temp / Divider % Base;
    Temp -= Res * Divider;
    Temp += Val * Divider;

    return Temp;
}
//GetValOfPosDB64(1234.5678, 2) ==> 3
//GetValOfPosDB64(1234.5678, -1) ==> 5
static BUB08 GetValOfPosDB64(BDB64 Input, BSB08 Pos) {
    BUB08 Base = 10;
    BDB64 Temp = Input;
    while (Pos < 0) {
        Temp *= Base;
        Pos += 1;
    }

    BUB08 i = NULL, Res = NULL;
    BSL32 Divider = 1;

    while ((i++) < Pos) {
        Divider *= Base;
    }

    Res = (BSL32)Temp / Divider % Base;

    return Res;
}
//SetValOfPosDB64(1234.5678, 1,4) ==> 1244.5678
//SetValOfPosDB64(1234.5678, -1,4) ==> 1234.4678
static BDB64 SetValOfPosDB64(BDB64 Input, BSB08 Pos, BUB08 Val) {
    BUB08 Base = 10,Amp = NULL;
    BDB64 Temp = Input;
    while (Pos < 0) {
        Temp *= Base;
        Pos += 1;
        Amp += 1;
    }

    BUB08 i = NULL, Res = NULL;
    BSL32 Divider = 1;

    while ((i++) < Pos) {
        Divider *= Base;
    }

    Res = (BSL32)Temp / Divider % Base;

    Temp -= Res * Divider;
    Temp += Val * Divider;

    while ((Amp--) > NULL) {
        Temp /= Base;
    }

    return Temp;
}

typedef struct {
    BUB08 (*GetValOfPosSL32)(BSL32 Input, BUB08 Pos, BUB08 Base);
    BSL32 (*SetValOfPosSL32)(BSL32 Input, BUB08 Pos, BUB08 Base,BUB08 Val);
    BUB08 (*GetValOfPosDB64)(BDB64 Input, BSB08 Pos);
    BDB64 (*SetValOfPosDB64)(BDB64 Input, BSB08 Pos, BUB08 Val);
}S_Num;

S_Num Num = {
GetValOfPosSL32,
SetValOfPosSL32,
GetValOfPosDB64,
SetValOfPosDB64,
};

#pragma endregion

#pragma region 内存复制

static BOOL MemCpy(PUB08 Src, PUB08 Dst, BUL32 Len, BOOL Reverse) {
    if (Src == NULL || Dst == NULL || Len == NULL)return eFAIL;

    Dst += (Reverse == eFAIL ? 0 : Len - 1);

    while (Len--) {
        if (Reverse == eFAIL) {
            *(Dst++) = *(Src++);
        }
        if (Reverse == eTRUE) {
            *(Dst--) = *(Src++);
        }
    }

    return eTRUE;
}
#pragma endregion