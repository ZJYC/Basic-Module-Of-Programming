--------------------------------------------------------------------------
UL32 GET_SIGNAL(UL32 * Signal,UL32 Wanted,UL32 Unwanted)
{
	if(*Signal == Wanted)
	{
		*Signal = Unwanted;
		return Wanted;
	}
	return Unwanted;
}
if(GET_SIGNAL(*Flg,TRUE,FALSE) == TRUE)
{
	......
}



