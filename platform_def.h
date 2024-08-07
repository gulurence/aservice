#pragma once






#ifdef _WINDOWS

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

#else

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long                int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long       uint64;

#endif





#ifdef _WINDOWS

#pragma warning(disable:4099)//LNK4099
#pragma warning(disable:4996)
#pragma warning(disable:4200)
#pragma warning(disable:4355)

#endif






