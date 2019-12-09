--------------------------------------------------------------------------
/* printf:"Header      Ender" */
static UB08 * PrintfSelfChkTitle(UB08 * Buff,US16 BuffLen,UB08 * Header, UB08 * Ender,UB08 Length) {
	UB08 LenOfHeader = strlen((const char *)Header);
	UB08 LenOfUnit = strlen((const char *)Ender);
	if (LenOfHeader + LenOfUnit > Length)return NULL;
	if (LenOfHeader + LenOfUnit > BuffLen)return NULL;
	UB08 SpaceNum = Length - LenOfHeader - LenOfUnit;
	Buff[0] = NULL;
	strcat(Buff, Header);
	for (US16 i = 0; i < SpaceNum; i++)strcat(Buff, " ");
	strcat(Buff, Ender);
	return Buff;
}
static void PrintValItem(TEXT * pTEXT, UB08 * Header, FT32 Val, UB08 Point) {
	UB08 Format[10] = { NULL };
	UB08 Ender[10] = { 0 };
	sprintf_s(Format, 10, "%%0%d.%df", pTEXT->Flg.Alone.NUMLEN, Point);
	sprintf_s(Ender, 10, Format, Val);
	UB08 LenOfHeader = strlen((const char *)Header);
	UB08 LenOfEnder = strlen((const char *)Ender);
	if (LenOfHeader + LenOfEnder > pTEXT->MaxLen)return;
	UB08 SpaceNum = pTEXT->MaxLen - LenOfHeader - LenOfEnder;
	pTEXT->Text[0] = NULL;
	strcat(pTEXT->Text, Header);
	for (US16 i = 0; i < SpaceNum; i++)strcat(pTEXT->Text, " ");
	strcat(pTEXT->Text, Ender);
}