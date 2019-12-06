--------------------------------------------------------------------------
int value __attribute__((section(".ARM.__at_0x20000000"))) = 0x33;
const char ziku[] __attribute__((section(".ARM.__at_0x00001000")))   = {0x1, 0x2, 0x3};
void func (void) __attribute__((section(".ARM.__at_0x00000100")));
--------------------------------------------------------------------------
/* 数字循环 */
static SL32 DigitalLoopAdd(SL32 Val, SL32 Add, SL32 Min, SL32 Max)
{
	if ((Val + Add) > Max)return Min;
	if ((Val + Add) < Min)return Max;
	return (Val + Add);
}
/* 幂指数 */
static SL32 Power(SL32 Val, SL32 Level)
{
	SL32 Sum = 1;
	while (Level--)Sum *= Val;
	return Sum;
}
/* 获得数字的某一位 */
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




