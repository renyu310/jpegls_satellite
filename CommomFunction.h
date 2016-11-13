#ifndef COMM_HEADER
#define COMM_HEADER

#include <iostream>
#include <math.h>
#include <cassert>
#include <string>

using namespace std;

typedef enum { NoEndian, LittleEndian, BigEndian } Endian;

typedef unsigned char		Uint8;
typedef unsigned short		Uint16;
typedef unsigned int		Uint32;
typedef unsigned long long	Uint64;
typedef signed char			Int8;
typedef signed short		Int16;
typedef signed long			Int32;
typedef signed long long	Int64;
typedef float				Float32;
typedef double				Float64;

#define	Uint8_MAX	 0xFF
#define	Uint16_MAX	 0xFFFF
#define	Uint32_MAX	 0xFFFFFFFF
#define	Int8_MIN	-0x80
#define	Int8_MAX	 0x7F
#define	Int16_MIN	-0x8000
#define	Int16_MAX	 0x7FFF
#define	Int32_MIN	-0x80000000
#define	Int32_MAX	 0x7FFFFFFF

#define Assert(expr) assert(expr)


static inline double ourlog2(double x) { return log(x) / log(2); }

static inline Uint32 Maximum(Uint32 a, Uint32 b) { return (a > b) ? a : b; }
static inline Uint16 Maximum(Uint16 a, Uint16 b) { return (a > b) ? a : b; }
static inline Uint32 Minimum(Uint32 a, Uint32 b) { return (a > b) ? b : a; }
static inline Uint16 Minimum(Uint16 a, Uint16 b) { return (a > b) ? b : a; }

static inline Uint32 Abs(Int32 x) { return (Uint32)((x < 0) ? -x : x); }
static inline Uint16 Abs(Int16 x) { return (Uint16)((x < 0) ? -x : x); }

static inline double Log2(double x)	{ return (ourlog2(x)); }

static inline Uint32 Floor(double x)	{ return Uint32(floor(x)); }
static inline Uint32 Ceiling(double x)	{ return Uint32(ceil(x)); }
static inline Uint32 FloorDivision(Uint32 n, Uint32 d)	{ return Uint32(floor(double(n) / double(d))); }
static inline Uint32 CeilingDivision(Uint32 n, Uint32 d)	{ return Uint32(ceil(double(n) / double(d))); }

static inline string uperTolown(string str){
	int i = 0;
	while (str[i]){
		str[i] = tolower(str[i]);
		i++;
	}
	return str;
}

static inline string lownTouper(string str){
	int i = 0;
	while (str[i]){
		str[i] = toupper(str[i]);
		i++;
	}
	return str;
}


static const Uint16 J[32] = {		// Order of run length codes
	0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

static const Uint16 J_rm[32] = {	// Length of run length codes (ie. 1<<J[n])
	1u << 0, 1u << 0, 1u << 0, 1u << 0,
	1u << 1, 1u << 1, 1u << 1, 1u << 1,
	1u << 2, 1u << 2, 1u << 2, 1u << 2,
	1u << 3, 1u << 3, 1u << 3, 1u << 3,
	1u << 4, 1u << 4,
	1u << 5, 1u << 5,
	1u << 6, 1u << 6,
	1u << 7, 1u << 7,
	1u << 8,
	1u << 9,
	1u << 10,
	1u << 11,
	1u << 12,
	1u << 13,
	1u << 14,
	1u << 15
};





#endif