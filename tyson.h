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
#define METADATA_SIZE     (96)
#define START_MARKER      ("main") 
#define STACK_SIZE        (120000)
#define RECUR_LIMIT       (200)
#define TEXT_MAXSIZE      (500000)
#define DATABUF_SIZE      (STACK_SIZE)
#define GCOL_THRESHOLD    (2400)
#define TEXT_BASE         (METADATA_SIZE)
#define ARGS_BUFFER_SIZE  (5000)

#define TIMG_SIZE_OFFS	  (0)
#define START_ADDR_OFFS   (8)
#define ARGS_BASE_OFFS    (16)
#define TEXT_BASE_OFFS	  (24)
#define POOL_BASE_OFFS    (32)
#define HEAP_BASE_OFFS    (40)
#define PIMG_SIZE_OFFS	  (48)
#define TEXT_SIZE_OFFS    (56)
#define POOL_SIZE_OFFS    (64)
#define HEAP_SIZE_OFFS    (72)
#define ARGS_COUNT_OFFS   (80)
#define ARGS_SIZE_OFFS    (88)

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

typedef struct {
	u64 argc;
	u64 argsz;
	u8* buf;
} ProcessArgs;

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

int      execute_process(Process*);
Process* malloc_process();
void     free_process(Process*);
Process* build_process(const char*, ProcessArgs*);
u64      write_process(Process*, const char*);

#endif
