struct sWinCheat {
	unsigned __int8  Type;
	unsigned __int8  Code[16];
	unsigned __int8  Description[200];

	unsigned __int32 Address;
	unsigned __int8  NoBytes;
	unsigned __int8  SavedBytes[4];
	unsigned __int8  CheatBytes[4];
};

struct sWinCheats{
	unsigned __int32 NoCheats;
	struct sWinCheat *Cheats;
} WinCheats;