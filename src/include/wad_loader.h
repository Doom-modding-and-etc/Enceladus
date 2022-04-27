#include "structures/wad_structure.h"

void    W_InitMultipleFiles (char** filenames);
void    W_Reload (void);

int	W_CheckNumForName (char* name);
int	W_GetNumForName (char* name);

int	W_LumpLength (int lump);
void W_ReadLump (int lump, void *dest);

void* W_CacheLumpNum (int lump, int tag);
void* W_CacheLumpName (char* name, int tag);

