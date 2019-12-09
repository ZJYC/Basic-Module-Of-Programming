
typedef 	unsigned short   US16;
typedef 	signed short     SS16;
typedef 	unsigned char    UB08;
typedef 	signed char      SB08;
typedef 	unsigned long    UL32;
typedef 	signed long      SL32;
typedef 	float            FT32;
typedef 	double           DB64;

static SL32 DigitalLoopAdd(SL32 Val, SL32 Add, SL32 Min, SL32 Max)
{
	if ((Val + Add) > Max)return Min;
	if ((Val + Add) < Min)return Max;
	return (Val + Add);
}

static SL32 Power(SL32 Val, SL32 Level)
{
	SL32 Sum = 1;
	while (Level--)Sum *= Val;
	return Sum;
}

static SL32 GetDigital(SL32 Val, SL32 Bit)
{
	while (Bit != 0)
	{
		Val /= 10;
		Bit--;
	}

	return Val % 10;
}
/* 带限位的数据增减 */
static SL32 Decimal(SL32 Val, SL32 Sel, SL32 Add, SL32 Min, SL32 Max)
{
	SL32 ValTemp = 0;
	SL32 DigitalSelected = 0;
	if (Sel < 0)return Val;
	DigitalSelected = GetDigital(Val, Sel);
	Val -= DigitalSelected * Power(10, Sel);
	ValTemp = Val + DigitalLoopAdd(DigitalSelected, Add, 0, 9) * Power(10, Sel);
	if (ValTemp > Max)
	{
		if (Add >= 0)
		{
			return Val;
		}
		if (Add < 0)
		{
			return Val + GetDigital(Max, Sel) * Power(10, Sel);;
		}
	}
	else
	{
		return ValTemp;
	}
	return Val;
}
