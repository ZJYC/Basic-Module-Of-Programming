static UL32 JSHash(UB08 * Data,US16 Len)
{
	UL32 hash = 1315423911;

	while (Len --)
	{
		hash ^= ((hash << 5) + (Data[Len]) + (hash >> 2));
	}

	return (hash & 0x7FFFFFFF);
}
