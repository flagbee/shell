
#include <ctype.h>
#include <shell.h>
#include <stdlib.h>
#include "config.h"

const char shell_help_test[] =
		"[test]\r\n"
		" * test command\r\n"
		"\r\n";

uint8_t shell_cmd_test(void *para, uint8_t len)
{
	PRINTF("test command\r\n");
	return TRUE;
}

SHELL_CMD(base,	test,	shell_help_test,	NULL,	shell_cmd_test);
