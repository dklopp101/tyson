#include <stdio.h>
#include <stdlib.h>

int tyasm(const char* in, const char* out, unsigned report)
{
	char command[256];
	sprintf(command, "python3 tyasm.py %s %s %u", in, out, report);
	return system(command);	
}

int main(int argc, char *argv[])
{
	if (argc == 3) {
		tyasm(argv[1], argv[2], 1);
	} else {
		printf("\n\tinvalid input to tyasm.");
	}
	return 0;
}
