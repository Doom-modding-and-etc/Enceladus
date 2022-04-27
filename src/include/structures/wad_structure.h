#ifndef __WAD_STRUCTURE_H
#define __WAD_STRUCTURE_H

//
// TYPES
//
typedef struct
{ // Should be "IWAD" or "PWAD". 
  char identification[4];
  
  int numlumps; 
  int infotableofs; 
} wadinfo_t;

typedef struct
{ 
 int filepos; 
 int size; 
 char name[8]; 
} filelump_t;

//
//WADFILE I/O related stuff.
//
typedef struct
{ 
 char name[8]; 
 int handle; 
 int position; 
 int size;
} lumpinfo_t;

extern	void**		lumpcache;
extern	lumpinfo_t*	lumpinfo;
extern	int		numlumps;

static struct
{ 
  void *cache;
  #ifdef TIMEDIAG 
  int locktic;
  #endif 
  int locks;
} *cachelump;


#endif

