#ifndef debug_h
#define debug_h

#define DBACT_COUNT 7

#define END         0
#define RUN         1
#define STEP        2
#define STOP        3
#define RESET       4
#define PRINT_STK   5
#define PRINT_MEM   6

#define build_dbtable()                  			       \
	static void* dbtable[DBACT_COUNT]= {&&dbact_end,       \
		                                &&dbact_run,       \
		                                &&dbact_step,      \
		                                &&dbact_stop,      \
		                                &&dbact_reset,     \
		                                &&dbact_print_stk, \
		                                &&dbact_print_mem}

const char* dbmenu_str;
const char* input_msg;
const char* invalid_input_msg;

u8  is_int(const char*);
u8* get_stdin_str();
u8  dbmenu_input();

#endif
