//
//有些时候需要判断数据是否改变，用这种散列函数最好了
//
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

static UL32 JSHash(UB08 * Data,UL32 Length){
	UL32 hash = 1315423911;
	if (Data == NULL || Length == NULL)return NULL;
	while (Length--){
		hash ^= ((hash << 5) + (Data[Length]) + (hash >> 2));
	}
	return (hash & 0x7FFFFFFF);
}

static UL64 MurmurHash64B (UB08 * key, UL32 len){
	const UL32 m = 0x5bd1e995;
	const SL32 r = 24;
	//seed：种子，最好用一个质数 0xEE6B27EB  
	UL32 h1 = (0xEE6B27EB) ^ len;
	UL32 h2 = 0;
 
	if (key == NULL || len == NULL)return NULL;

	UL32 * data = (UL32 *)key;
 
	while(len >= 8){
		UL32 k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
 
		UL32 k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}
 
	if(len >= 4){
		UL32 k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}
 
	switch(len){
	case 3: h2 ^= ((UB08 *)data)[2] << 16;
	case 2: h2 ^= ((UB08 *)data)[1] << 8;
	case 1: h2 ^= ((UB08 *)data)[0];
			h2 *= m;
	};
 
	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;
 
	UL64 h = h1;
 
	h = (h << 32) | h2;
 
	return h;
}

typedef struct{
	UL64 (*Hash64)(UB08 * key, SL32 len);
	UL32 (*Hash32)(UB08 * key, SL32 len);
}HASH;

HASH Hash = {
	.Hash64 = MurmurHash64B,
	.Hash32 = JSHash,
};

static BOOL IsDataChanged(UB08 * pData, US16 Length) {
	static UL64 LastHash = NULL;
	UL64 CurtHash = NULL;
	BOOL Res = FAIL;
	
	Hash.Hash64(pData, Length);
	if (CurtHash == LastHash) {
		Res = FAIL;
	}
	else {
		Res = TRUE;
		LastHash = CurtHash;
	}

	return Res;

}



