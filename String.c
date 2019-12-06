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
