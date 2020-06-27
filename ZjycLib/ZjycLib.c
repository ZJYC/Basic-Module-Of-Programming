#include "ZjycLib.h"

#pragma region 内存相关
static EBOOL MemCpy(PUB08 Src, PUB08 Dst, BUL32 Len, EBOOL Reverse) {
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
S_Mem Mem = {
    .MemCpy = MemCpy,
};
#pragma endregion
#pragma region 字序转换相关
static PUB08 US16_2_UB08(BUS16 Input, E_ByteOrder OutputOrder) {
    //Input 0x1234,
    //OutputOrder:Big ==> [12] [34]
    //OutputOrder:Sml ==> [34] [12]
    static UB08 Temp[2] = { NULL };

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

	static UB08 Temp[2] = { NULL };

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

	static UB08 Temp[4] = { NULL };

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

	static UB08 Temp[4] = { NULL };

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

	static BUB08 Temp[4] = { NULL };
    PUB08 p = (PUB08)(&Input);

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
    if (InputOrderx == SmlOrder) {
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
    PUB08 p = (PUB08)(&Temp);

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
static PUB08 US16_2_UB08X(BUS16 Input, PUB08 Output, E_ByteOrder OutputOrder) {
	if (Output == NULL)return NULL;
	PUB08 Temp = US16_2_UB08(Input, OutputOrder);
	Mem.MemCpy(Temp, Output, 0x02, eFAIL);
	return Temp;
}
static PUB08 SS16_2_UB08X(BSS16 Input, PUB08 Output, E_ByteOrder OutputOrder) {
	if (Output == NULL)return NULL;
	PUB08 Temp = SS16_2_UB08(Input, OutputOrder);
	Mem.MemCpy(Temp, Output, 0x02, eFAIL);
	return Temp;
}
static PUB08 UL32_2_UB08X(BUL32 Input, PUB08 Output, E_ByteOrder OutputOrder) {
	if (Output == NULL)return NULL;
	PUB08 Temp = UL32_2_UB08(Input, OutputOrder);
	Mem.MemCpy(Temp, Output, 0x04, eFAIL);
	return Temp;
}
static PUB08 SL32_2_UB08X(BSL32 Input, PUB08 Output, E_ByteOrder OutputOrder) {
	if (Output == NULL)return NULL;
	PUB08 Temp = SL32_2_UB08(Input, OutputOrder);
	Mem.MemCpy(Temp, Output, 0x04, eFAIL);
	return Temp;
}
static PUB08 FT32_2_UB08X(BFT32 Input, PUB08 Output, E_ByteOrder OutputOrder) {
	if (Output == NULL)return NULL;
	PUB08 Temp = FT32_2_UB08(Input, OutputOrder);
	Mem.MemCpy(Temp, Output, 0x04, eFAIL);
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
US16_2_UB08X,
SS16_2_UB08X,
UL32_2_UB08X,
SL32_2_UB08X,
FT32_2_UB08X,
SmlOrder,
};
#pragma endregion
#pragma region 哈希表
static BUL32 JSHash(PUB08 Data, BUL32 Length) {
    BUL32 hash = 1315423911;
    if (Data == NULL || Length == NULL)return NULL;
    while (Length--) {
        hash ^= ((hash << 5) + (Data[Length]) + (hash >> 2));
    }
    return (hash & 0x7FFFFFFF);
}

S_JsHashApi JsHashApi = {
    JSHash,
};
#pragma endregion
#pragma region CRC
static const BUB08 aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};
static const BUB08 aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};
static BUS16 usMBCRC16(PUB08 pucFrame, BUS16 usLen) {
	BUB08 ucCRCHi = 0xFF;
	BUB08 ucCRCLo = 0xFF;
	BUS16 iIndex;
	while (usLen--) {
		iIndex = ucCRCLo ^ *(pucFrame++);
		ucCRCLo = (BUB08)(ucCRCHi ^ aucCRCHi[iIndex]);
		ucCRCHi = aucCRCLo[iIndex];
	}
	return (BUS16)((ucCRCHi << 8) | ucCRCLo);
}
S_CrcApi CrcApi = {
    usMBCRC16,
};
#pragma endregion
#pragma region Sem
//设定信号量的初始值
static EBOOL SemInit(PS_Sem pSem, US16 SetValue) {
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
static EBOOL SemTake(PS_Sem pSem) {
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
static EBOOL SemGive(PS_Sem pSem) {
    CRITICAL_ENTER();

    if (pSem == NULL || pSem->Occupied == eTRUE) {
        CRITICAL_EXITX();
        return eFAIL;
    }

    pSem->Occupied = eTRUE;
    pSem->Cnt = pSem->Cnt + 1;
	pSem->Cnt = pSem->Cnt > pSem->Max ? pSem->Max : pSem->Cnt;
    pSem->Occupied = eFAIL;

    CRITICAL_EXITX();
    return eTRUE;
}
//查看信号量
static EBOOL SemPeek(PS_Sem pSem) {
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

S_SemApi SemApi = {
.Init = SemInit,
.Take = SemTake,
.Give = SemGive,
.Peek = SemPeek,
};

#pragma endregion
#pragma region Mail

//获取邮箱:返回邮箱内容
static EBOOL MailTake(S_PMail pMail, PUL32 MailContent) {
    CRITICAL_ENTER();

    if (pMail == NULL || pMail->State == eEmpty) {
        CRITICAL_EXITX();
        return eFAIL;
    }
	if(MailContent != NULL)*MailContent = pMail->MailContent;
    pMail->State = eEmpty;

    CRITICAL_EXITX();
    return eTRUE;
}
//发送邮箱:发送邮箱内容
static EBOOL MailGive(S_PMail pMail, BUL32 MailContent) {
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

S_MailApi MailApi = {
.Take = MailTake,
.Give = MailGive,
};

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
static BSL32 SetValOfPosSL32(BSL32 Input, BUB08 Pos, BUB08 Base, BUB08 Val) {
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
	BUB08 Base = 10, Amp = NULL;
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

static BDB64 Amplify(BDB64 Input, BSB08 Factor_Log10) {
	if (Factor_Log10 == -4)return Input / 10000.0;
	if (Factor_Log10 == -3)return Input / 1000.0;
	if (Factor_Log10 == -2)return Input / 100.0;
	if (Factor_Log10 == -1)return Input / 10.0;
	if (Factor_Log10 == 0)return Input * 1.0;
	if (Factor_Log10 == 1)return Input * 10.0;
	if (Factor_Log10 == 2)return Input * 100.0;
	if (Factor_Log10 == 3)return Input * 1000.0;
	if (Factor_Log10 == 4)return Input * 10000.0;
}

static BUL32 Distance(BUL32 Smaller, BUL32 Bigger, BUL32 Max) {
	if (Bigger >= Smaller) {
		return (Bigger - Smaller);
	}
	else {
		return (Bigger + Max - Smaller);
	}
}
static BUL32 CyclicAddOpend(BUL32 Input, BSL32 Add, BUL32 Min, BUL32 Max) {
	Input += Add;
	Input = (Input >= Max ? Min : Input);
	Input = (Input <= Min ? Max : Input);
	return Input;
}
static BUL32 CyclicAddClose(BUL32 Input, BSL32 Add, BUL32 Min, BUL32 Max) {
	Input += Add;
	Input = (Input > Max ? Min : Input);
	Input = (Input < Min ? Max : Input);
	return Input;
}
S_Num Num = {
GetValOfPosSL32,
SetValOfPosSL32,
GetValOfPosDB64,
SetValOfPosDB64,
Distance,
Amplify,
};

#pragma endregion
#pragma region 环形缓冲区

#include <assert.h>
#include <string.h>

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

S_RingBuffApi RingBuffApi = {
.Init = RB_Init,
.Read = RB_Read,
.Peek = RB_Peek,
.Writ = RB_Writ,
.GetFree = RB_GetFree,
.GetUsed = RB_GetUsed,
};

//使用样例
//RINGBUFFER_NEW(Tst, 1024);

#pragma endregion
#pragma region 函数回调
static EBOOL CallbackExe(PS_CbkMgt pCbkMgt, UL32 Mask, UL32 pMsg) {
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
static EBOOL CallbackAdd(PS_CbkMgt pCbkMgt, UL32 Mask, PCbkFUNC pCbkFUNC) {
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
S_CbkApi CbkApi = {
.Exe = CallbackExe,
.Add = CallbackAdd,
};
#pragma endregion
#pragma region 查表法
static BFT32 SearchTable(PS_TableItem pS_TableItem, BUS16 MaxCount, BFT32 x) {
	BUL32 i = 0, X1 = 0, X2 = 0;
	BFT32 x1, x2, y1, y2, r, A, B;
	/* 表长度小于2就不叫查表了 */
	if (pS_TableItem == 0 || MaxCount < 2)return 1.0;
	if (x < pS_TableItem[0].x) {
		X1 = 0;
		X2 = 1;
	}
	else if (x > pS_TableItem[MaxCount - 1].x) {
		X1 = MaxCount - 2;
		X2 = MaxCount - 1;
	}
	else {
		for (i = 0; i < MaxCount - 1; i++) {
			if ((pS_TableItem[i].x < x) && (x < pS_TableItem[i + 1].x)) {
				X1 = i;
				X2 = i + 1;
				break;
			}
		}
	}

	x1 = pS_TableItem[X1].x;
	x2 = pS_TableItem[X2].x;
	y1 = pS_TableItem[X1].y;
	y2 = pS_TableItem[X2].y;

	if (x2 == x1)return 1.0;
	A = (y2 - y1) / (x2 - x1);
	B = y2 - A * x2;

	r = A * x + B;

	return r;
}
//static BFT32 GetLine(BFT32 X1, BFT32 Y1, BFT32 X2, BFT32 Y2,PFT32 A,PFT32 B) {
//	if()
//}


S_TableApi TableApi = {
	SearchTable,
};
#pragma endregion







