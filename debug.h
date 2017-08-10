#ifndef debug_h
#define debug_h

#define DBACT_COUNT 6

#define END      0
#define START    1
#define STOP     2
#define STEPF    3
#define PRINTM   4
#define PRINTS   5


#define build_dbtable()                  			    \
	static void* dbtable[DBACT_COUNT]= {&&dbact_end,    \
		                                &&dbact_start,  \
		                                &&dbact_stop,   \
		                                &&dbact_stepf,  \
		                                &&dbact_printm, \
		                                &&dbact_prints}

const char* dbmenu_str = "\n\t[{}]end [{}]start [{}]stop [{}]stepf [{}]printm [{}]prints";
const char* input_msg  = "\n --> ";
const char* invalid_input_msg = "\n\tinvalid choice input, try again.";

u8 dbmenu_input()
{
	u8 input;

	for (;;) {
		printf("%s%s", db_menu_str, input_msg);
		input = fgetc(stdin);
		switch (input) {
			case END:
            case START:
			case STOP:
			case STEPF:
			case PRINTM:
			case PRINTS:
				return input_msg;
			default:
				printf(invalid_input_msg);
				continue;
		}
	}
}

#endif
