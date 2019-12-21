

#if 1 /* 仿操作系统信号量 */
static UB08 Semaphore_Get(UB08 * pSemaphore,UB08 Num){
	if(pSemaphore == NULL)return FAIL;
	if(*pSemaphore != NULL){
		*pSemaphore -= (*pSemaphore >= Num ? Num : *pSemaphore);
		return TRUE;
	}
	return FAIL;
}
static UB08 Semaphore_Set(UB08 * pSemaphore,UB08 New){
	if(pSemaphore == NULL)return FAIL;
	*pSemaphore = New;
	return TRUE;
}
typedef struct{
	UB08 (*Get)(UB08 * pSemaphore,UB08 Num);
	UB08 (*Set)(UB08 * pSemaphore,UB08 Num);
}Semaphore;
Semaphore Sem = {
	.Get = Semaphore_Get,
	.Set = Semaphore_Set,
};
#endif

#if 1/* 模块结构 */
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
#endif

#if 1/* 回调注册 */
typedef CBKR (*CBKF)(UL32 Tag, VOID * pMsg);
#define BIT(x)		(UL32)(1 << (x))
typedef enum{
	Fail = 0x00,
	True = 0x01,
	Full = 0x02,
	
	Stop = 0x64,
}CBKR
typedef struct {
	CBKF Cbk;
	UL32 Msk;
}CBKI;
typedef struct {
	CBKI Cbks[0];
	UL32 Lens;
}CBKS;
static CBKR Init(CBKS * pCBKS,CBKI * pCBKI,UL32 Lens){
	if(pCBKS == NULL || pCBKI == NULL)return Fail;
	pCBKS->Cbks = pCBI;
	pCBKS->Lens = Lens;
	return True;
}
static CBKR Addx(CBKS * pCBKS,UL32 Msk,CBKF pNew){
	if((pCBKS == NULL)||(pNew == NULL))return Fail;
    /* 已经存在就无需再次添加了 */
    for (UL32 i = NULL; i < pCBM->Lens; i++) {
        if (pCBM->Cbks[i].Cbk == pNew)return True;
    }
	/* 尝试加入 */
	for (UL32 i = NULL; i < pCBM->Lens; i++) {
		if(pCBM->Cbks[i].Cbk == UL32_NULL){
			pCBM->Cbks[i].Msk = Msk;
			pCBM->Cbks[i].Cbk = pNew;
			return True;
		}
	}
	if(i >= pCBM->Lens){
		return Full;
	}
	return Fail;
}
static CBKR Delx(CBKS * pCBKS,CBKF pDel){
	if((pCBKS == NULL)||(pDel == NULL))return Fail;
	for(UL32 i = UL32_NULL;i < pCBKS->Lens;i++){
		if(pCBKS->CBIS[i].Cbk == pDel){
			pCBKS->CBIS[i].Cbk = NULL;
			return True;
		}
	}
	return Fail;
}
static CBKR Exex(CBKS * pCBKS,UL32 Msk,VOID * pMsg){
	if(pCBKS == UL32_NULL)return Fail;
	for(UL32 i = UL32_NULL;i < pCBKS->Lens;i++){
		if ((pCBKS->Cbks[i].Cbk != NULL) && 
            (pCBKS->Cbks[i].Msk & Msk)) {
			if(pCBKS->Cbks[i].Fuc(Msk, pMsg) == Stop)return Stop;
		}
	}
	return True;
}
typedef struct{/* 驱动模块 */
	CBKR (*Init)(CBKS * pCBKS,CBKI * pCBKI,UL32 Lens);
	CBKR (*Addx)(CBKS * pCBKS,UL32 Msk,CBKF pNew);
	CBKR (*Delx)(CBKS * pCBKS,CBKF pDel);
	CBKR (*Exex)(CBKS * pCBKS,UL32 Msk,VOID * pMsg)
}CBK_ModuleExport;
typedef struct{/* 模块结构 */
	//XX_ModuleImport Import;
	CBK_ModuleExport Export;
	//XX_ModuleTmpdat Tmpdat;
}CBK_Module;
#endif

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







