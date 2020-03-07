
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

#pragma region 信号量操作
//信号量结构体
typedef struct {
	BOOL Occupied;
	US16 Cnt;
}Semaphore;
//设定信号量的初始值
static BOOL SemaphoreInit(Semaphore * pSemaphore, US16 SetValue) {
	CRITICAL_ENTER();

	if (pSemaphore == NULL || pSemaphore->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}
	pSemaphore->Occupied = eTRUE;
	pSemaphore->Cnt = SetValue;
	pSemaphore->Occupied = eFAIL;

	CRITICAL_EXITX();
	return eTRUE;
}
//获取信号量
static BOOL SemaphoreTake(Semaphore * pSemaphore) {
	UB08 Res = eFAIL;
	CRITICAL_ENTER();

	if (pSemaphore == NULL || pSemaphore->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}
	pSemaphore->Occupied = eTRUE;
	if (pSemaphore->Cnt > NULL) {
		pSemaphore->Cnt = pSemaphore->Cnt - 1;
		Res = eTRUE;
	}
	else {
		Res = eFAIL;
	}
	pSemaphore->Occupied = eFAIL;
	CRITICAL_EXITX();
	return Res;
}
//释放信号量
static BOOL SemaphoreGive(Semaphore * pSemaphore) {
	CRITICAL_ENTER();

	if (pSemaphore == NULL || pSemaphore->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}

	pSemaphore->Occupied = eTRUE;
	pSemaphore->Cnt = pSemaphore->Cnt + 1;
	pSemaphore->Occupied = eFAIL;

	CRITICAL_EXITX();
	return eTRUE;
}
//查看信号量
static BOOL SemaphorePeek(Semaphore * pSemaphore) {
	UB08 Res = eFAIL;
	CRITICAL_ENTER();

	if (pSemaphore == NULL || pSemaphore->Occupied == eTRUE) {
		CRITICAL_EXITX();
		return eFAIL;
	}

	pSemaphore->Occupied = eTRUE;
	Res = (pSemaphore->Cnt == NULL ? eFAIL : eTRUE);
	pSemaphore->Occupied = eFAIL;

	CRITICAL_EXITX();
	return Res;
}
//接口
typedef struct {
	BOOL(*Init)(Semaphore * pSemaphore, US16 SetValue);
	BOOL(*Take)(Semaphore * pSemaphore);
	BOOL(*Give)(Semaphore * pSemaphore);
	BOOL(*Peek)(Semaphore * pSemaphore);
}SemaphoreApi;

SemaphoreApi Sem = {
.Init = SemaphoreInit,
.Take = SemaphoreTake,
.Give = SemaphoreGive,
.Peek = SemaphorePeek,
};

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
}CallbackRes;

//typedef CallbackRes(*CallbackFunction)(UL32 Msk, VOID * pMsg);
//CallbackFunction Function;

typedef struct {
	CallbackRes(*Function)(UL32 Msk, VOID * pMsg);
	UL32 Mask;
	BOOL Occupied;
}CallbackItem;

typedef struct {
	UL32 Mask;
	UL32 Msg;
}Param;

typedef struct {
	CallbackItem *Items;
	UL32 ItemsLength;
}CallbackGroup;

static BOOL CallbackExe(CallbackGroup * pCallbackGroup, UL32 Mask, VOID * pMsg) {
	CallbackRes Res = eJustExit;
	if (pCallbackGroup == NULL)return eFAIL;
	for (UL32 i = 0; i < pCallbackGroup->ItemsLength; i++) {
		if (pCallbackGroup->Items[i].Mask & Mask) {
			if (pCallbackGroup->Items[i].Function != NULL && pCallbackGroup->Items[i].Occupied == eFAIL) {

				pCallbackGroup->Items[i].Occupied = eTRUE;
				Res = pCallbackGroup->Items[i].Function(Mask, pMsg);
				pCallbackGroup->Items[i].Occupied = eFAIL;

				if (Res == eJustExit)return eTRUE;
			}
			if (pCallbackGroup->Items[i].Function != NULL && pCallbackGroup->Items[i].Occupied == eTRUE) {
				//Debug information ...
			}
		}
	}
	return eTRUE;
}

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

#if 1/* 另类字符串转数 */
static UL32 S2B()
#endif







