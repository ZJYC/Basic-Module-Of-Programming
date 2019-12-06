--------------------------------------------------------------------------
typedef __packed union BytesConvert
{
	FT32 fT32;
	UB08 uB08[4];
	US16 uS16[2];
	SS16 sS16[2];
	SL32 sL32;
	UL32 uL32;
}BytesConverts;
BytesConvert.ft32 = 0.1234;
UL32 = 0x12345678
UB08[0] = 0x78
UB08[1] = 0x56
UB08[2] = 0x34
UB08[3] = 0x12
US16[0] = 0x5678
US16[0] = 0x1234
