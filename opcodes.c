#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "opcodes.h"
#include "tyson.h"


char* opcode_strmap[OPCOUNT] = {"die",
                                 "nop",
								     "jmp"};

/*
	Lookup Opcode:
		Takes a u8 value and a pointer to a buffer then checks if the
		u8 value is an opcode, if so the opcode is looked up in the
		opcode-string-map and the corresponding mneumonic string is
		copied to the buffer then 1 is returned to signal opcode matched.
		
		If opcode doesn't match 0 is returned and nothing else happens.

	Usage:
		Ensure the buffer is long enough to fit any mneumonic.
*/
u8
lookup_opcode(const u64 opcode, char* buf)
{
	if (is_opcode(opcode)) {
		strcpy(buf, opcode_strmap[opcode]);
		return 1;
	} else {
		return 0;
	}
}

/*
	Lookup Mneumonic:
		Takes a string and checks it against each opcode mneumonic.
		If the string matches one the corresponding opcode is returned.

	Usage:
		To use this function simply pass the string to it. Check the
		return value if it is -1 the string failed to match anything.
		If not, it matched something and the value must be cast to u8
		before being a valid opcode.
*/
s64
lookup_mneumonic(const char* str)
{
	u8 i;

	for (i=0; i < OPCOUNT; ++i) {
		if (strcmp(str, opcode_strmap[i]) == 0)
			return (s64) i;
	}

	return (s64) -1;
}


// Predicate that tests whether string is a valid mneumonic.
u8
is_mneumonic(const char* str)
{
	u8 i = 0;

	for (; i < OPCOUNT; ++i) {
		if (strcmp(str, opcode_strmap[i]) == 0)
			return 1;
	}
	
	return 0;
}


// Predicate that tests whether u8 value is a valid opcode.
u8
is_opcode(const u64 opcode)
{
	return ((opcode >= 0) && (opcode <= OPCOUNT));
}
