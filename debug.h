#ifndef debug_h
#define debug_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DBACT_COUNT 4

#define END         0
#define RUN         1
#define STEP        2
#define STOP        3

#define build_dbtable()                  			     \
	static void* dbtable[DBACT_COUNT]= {&&dbact_end,     \
		                                &&dbact_run,     \
		                                &&dbact_step,    \
		                                &&dbact_stop}

const char* dbmenu_str = "\n\t[0]end [1]run [2]step [3]stop";
const char* input_msg  = "\n --> ";
const char* invalid_input_msg = "\n\tinvalid input, try again.";

u8
is_int(const char* str)
{
	u8 i, len = strlen(str);

	// Cant be valid if length less than one.
	if (len < 1)
		return 0;

	// If number begins with a zero it's
	// invalid unless it's a lone zero,
	// if we do detect a lone zero we return 1 now.
	if (*str == '0') {
		if (len == 1)
			return 1;
		else
			return 0;
	}

	// Inspect string char at a time, if we find
	// a non-digit character then is invalid integer.
	for (i=0; i < len; ++i) {
		if (!isdigit(str[i]))
			return 0;
	}

	// Return 1 to signal that str represents an integer.
	return 1;
}

u8* get_stdin_str() {
    u8  buf[256];
    u8  sz = 0;
    u8* r;
    int ch;

    while ((ch=fgetc(stdin)) != 10)
        buf[sz++] = (u8) ch;
        
    buf[sz] = 0;
    r = (u8*) malloc(sz+1);
    strcpy(r, buf);
    return r;
}


u8 dbmenu_input()
{
    int input;
	u8* str;

	for (;;) {
		printf("%s%s", dbmenu_str, input_msg);
		str = get_stdin_str();
		if (strlen(str) == 1) {
		    if (is_int(str)) {
		    	input = atoi(str);
		    	free(str);
			    switch (input) {
				    case END: case RUN: case STEP: case STOP:
					    return (u8) input;
				    default:
					    printf("%s", invalid_input_msg);
					    continue;
			    }
			} else {
			    printf("%s", invalid_input_msg);
			    continue;		
		    }
		} else {
		    printf("%s", invalid_input_msg);
			continue;	
		} 
	}
}
#endif
