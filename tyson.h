#ifndef tyson_h
#define tyson_h

#include <stdio.h>
#include <stdint.h>


#define STACK_BYTES_RESERVED 100

/*
	Common Abbreviations Used:

		b    - u8
		u    - u64
		i    - s64
		r    - r64
		s    - string/0-terminated byte-table
		t    - byte-table
		c    - constant
		h    - heap
		st   - stack top
		rst  - return-stack top
		tdx  - table index
		fd   - foward
		bk   - backwards
		rsv  - reserved(used by vm itself only).
		sh   - short
		lkup - lookup
		tbl  - jump-table / address table.
		fc   - from constant.
		fh   - from heap
		fs   - from stack top.
		swch - switch
*/


// Important Constants.
#define METADATA_SEG_SIZE (120)
#define START_MARKER      ("main") 
#define STACK_SIZE        (120000)
#define RECUR_LIMIT       (200)
#define TEXT_MAXSIZE      (500000)
#define DATABUF_SIZE      (STACK_SIZE)
#define GCOL_THRESHOLD    (2400)
#define TEXT_BASE         (74)


// Native Datatype Declarations.
/*
	Important Notes On Native Datatypes:
		Please note that changing the native type declarations will most likely
		break the entire system! For the sake of better alignment, easier calculations
		and thus better speed, literally all instructions that don't operate on bytes
		can safely assume the data object in question is exactly 8 bytes long.

		The concept of the 64bit machine word is very important to tyson.
		The work stack can only push and pop entire words at a time, even single
		bytes pushed onto the stack will increment sp by 8.

		Again this is all for speed and simplicity. It basically allows internal vm
		pointers to be updated faster because of the fact that anything not a byte
		must be 8 bytes long.

		The downside to all this efficiency is that the Tyson VM can only see the world    
		in terms of either 1 or 8 bytes at a time. Tyson's current datatype set is likely
		to endure, perhaps the only change being the possible addition of a 2 word-long
		128 bit precision real datatype. This however would require at least 10-15 extra
		instructions, so not likely to happen. The other possibility is the complete
		replacement of r64 and her associated operations with their r128 equivilents.
*/

typedef uint8_t      u8;
typedef uint64_t    u64;
typedef int64_t     s64;
typedef double      r64;
typedef u64         w64;
typedef struct Word word;

struct Word {
	u8 byte0;
	u8 byte1;
	u8 byte2;
	u8 byte3;
	u8 byte4;
	u8 byte5;
	u8 byte6;
	u8 byte7;
} __packed__;

typedef struct {
	u64 size;
	u8* start_byte;
	u8* img;
} Process;

#define hwordsize  4
#define wordsize   8
#define dwordsize 16
#define qwordsize 32

#define NIL 0
#define WRD 10
#define U8  11
#define U64 12
#define S64 13
#define R64 14
#define STR 15
#define TBL 16

#define N   0  // nil
#define W   10 // word
#define B   11 // byte / u8
#define U   12 // unsigned / u64
#define I   13 // signed / s64
#define R   14 // real / r64
#define S   15 // string / 0 terminated u8 array
#define T   16 // table 

#define TRUE  1
#define FALSE 0

#define copyhword(d, s) \
	*d = *s;            \
	*(d+1) = *(s+1);    \
	*(d+2) = *(s+2);    \
	*(d+3) = *(s+3)

#define copyword(d, s) \
	*d = *s;           \
	*(d+1) = *(s+1);   \
	*(d+2) = *(s+2);   \
	*(d+3) = *(s+3);   \
	*(d+4) = *(s+4);   \
	*(d+5) = *(s+5);   \
	*(d+6) = *(s+6);   \
	*(d+7) = *(s+7)

#define copydword(d, s) \
	*d      = *s;       \
	*(d+1)  = *(s+1);   \
	*(d+2)  = *(s+2);   \
	*(d+3)  = *(s+3);   \
	*(d+4)  = *(s+4);   \
	*(d+5)  = *(s+5);   \
	*(d+6)  = *(s+6);   \
	*(d+7)  = *(s+7);   \
	*(d+8)  = *(s+8);   \
	*(d+9)  = *(s+9);   \
	*(d+10) = *(s+10);  \
	*(d+11) = *(s+11);  \
	*(d+12) = *(s+12);  \
	*(d+13) = *(s+13);  \
	*(d+14) = *(s+14);  \
	*(d+15) = *(s+15)

#define copyqword(d, s)  \
	*d       = *s;       \
	*(d+1)   = *(s+1);   \
	*(d+2)   = *(s+2);   \
	*(d+3)   = *(s+3);   \
	*(d+4)   = *(s+4);   \
	*(d+5)   = *(s+5);   \
	*(d+6)   = *(s+6);   \
	*(d+7)   = *(s+7);   \
	*(d+8)   = *(s+8);   \
	*(d+9)   = *(s+9);   \
	*(d+10)  = *(s+10);  \
	*(d+11)  = *(s+11);  \
	*(d+12)  = *(s+12);  \
	*(d+13)  = *(s+13);  \
	*(d+14)  = *(s+14);  \
	*(d+15)  = *(s+15);  \
	*(d+16)  = *(s+16);  \
	*(d+17)  = *(s+17);  \
	*(d+18)  = *(s+18);  \
	*(d+19)  = *(s+19);  \
	*(d+20)  = *(s+20);  \
	*(d+21)  = *(s+21);  \
	*(d+22)  = *(s+22);  \
	*(d+23)  = *(s+23);  \
	*(d+24)  = *(s+24);  \
	*(d+25)  = *(s+25);  \
	*(d+26)  = *(s+26);  \
	*(d+27)  = *(s+27);  \
	*(d+28)  = *(s+28);  \
	*(d+29)  = *(s+29);  \
	*(d+30)  = *(s+30);  \
	*(d+31)  = *(s+31)


void     execute_process(Process*);
Process* malloc_process();
void     free_process(Process*);
Process* build_process(const char*);
u64      write_process(Process*, const char*);

#endif
