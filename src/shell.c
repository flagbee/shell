
#include <ctype.h>
#include <shell.h>
#include <stdlib.h>
#include "global.h"
#include "shell.h"

static SH_INSTANCE_S s_inst;

const char shell_help_clear[] =
		"[cls]\r\n"
		" * clear the screen\r\n"
		"\r\n";

static const char help_help[] =
		"\r\n"
		"[help]\r\n"
		" * show commands\r\n"
		"\r\n";


static int hexToInt(char c)
{
	if (isdigit(c)) {
		return c - '0';
	} else {
		return tolower(c) - 'a' + 10;
	}
}

static uint32_t hexStrToInt(const char *_hex_str)
{
	uint32_t result = 0;
	char* hex_str = (char *)_hex_str;
    if (strncmp(hex_str, "0x", 2) == 0) {
        hex_str += 2;
    }
	for (int i = 0; hex_str[i] != '\0'; i++) {
		result = result * 16 + hexToInt(hex_str[i]);
	}
	return result;
}

int32_t shell_get_int_from_str(const uint8_t *p_str, int32_t *p_val)
{
	uint8_t ret = 0;

	*p_val = (int32_t)strtol((const char *)&p_str[0], NULL, 10);
	ret = 1;

	return ret;
}

int32_t shell_get_int_from_hexstr(const char *p_str, int32_t *p_val)
{
	uint8_t ret = 0;

	*p_val = hexStrToInt(p_str);
	ret = 1;

	return ret;
}

int32_t shell_parse_string_to_int_array(int32_t cmdcnt, const uint8_t args[][16], int32_t *p_array)
{
	int32_t cnt=0;

	while(0 < (cmdcnt-cnt)){
		if( shell_get_int_from_str(args[cnt], (int32_t *)&p_array[cnt]) ){
		}
		else{
			break;
		}
		cnt++;
	}

	return cnt;
}

float shell_parse_string_to_float(const uint8_t args[][16])
{
	return (float)atof((const char *)args[0]);
}

int32_t shell_parse_string_to_float_array(int32_t cmdcnt, const uint8_t args[][16], float *p_array)
{
	int32_t cnt=0;

	while(0 < (cmdcnt-cnt)){
		p_array[cnt] = (float)atof((const char *)args[cnt]);
		cnt++;
	}

	return cnt;
}

uint8_t shell_is_matched(const char *p_str1, const char *p_str2)
{
	if(strlen(p_str1) == strlen(p_str2) && 0 == strncmp(p_str1, p_str2, strlen(p_str1))){
		return 1;
	}
	else{
		return 0;
	}
}

int _write(int32_t file, uint8_t *ptr, int32_t len)
{
	return s_inst.output(ptr, (uint32_t)len);
}

static uint8_t cli_help(void *para, uint8_t len)
{
	uint8_t i=0;
	SH_COMMAND_S * const * section;

	section = s_inst.p_cmdlist;
	for(i=0; i<s_inst.cmdlist_size; i++){
		SH_COMMAND_S * const _cmd = *section;
		if( NULL != _cmd->p_cmd ) {
			if (NULL != _cmd->p_help) {
				PRINTF(_cmd->p_help);
			}
		}
		section++;
	}

	return TRUE;
}

uint8_t shell_cmd_clear(void *para, uint8_t len)
{
	TERMINAL_BACK_BLACK();
	TERMINAL_FONT_GREEN();
	TERMINAL_RESET_CURSOR();
	TERMINAL_DISPLAY_CLEAR();

	return TRUE;
}

static void cli_history_add(SH_INSTANCE_S *p_inst, char* buff)
{
	int32_t len=0;
	uint8_t index = p_inst->history.latest;

	if (NULL == buff) return;

	len = strlen((const char *)buff);
	if (len >= SH_MAX_BUFSIZE) return;  /* command too long */

	/* find the latest one */
	if (0 != index) {
		index--;
	} else {
		index = SH_HISTORY_MAX - 1;
	}

	if (0 != memcmp(p_inst->history.cmd[index], buff, len)) {
		/* if the new one is different with the latest one, the save */
		memset((void *)p_inst->history.cmd[p_inst->history.latest], 0x00, SH_MAX_BUFSIZE);
		memcpy((void *)p_inst->history.cmd[p_inst->history.latest], (const void *)buff, len);
		if (p_inst->history.count < SH_HISTORY_MAX) {
			p_inst->history.count++;
		}

		p_inst->history.latest++;
		if (p_inst->history.latest >= SH_HISTORY_MAX) {
			p_inst->history.latest = 0;
		}
	}

	p_inst->history.show = 0;
}

static uint8_t cli_history_show(SH_INSTANCE_S *p_inst, uint8_t mode, uint8_t** p_history)
{
	uint8_t err = TRUE;
	uint8_t num;
	uint8_t index;

	if (0 == p_inst->history.count) return err;

	if (TRUE == mode) {
		/* look up */
		if (p_inst->history.show < p_inst->history.count) {
			p_inst->history.show++;
		}
	} else {
		/* look down */
		if (1 < p_inst->history.show) {
			p_inst->history.show--;
		}
	}

	num = p_inst->history.show;
	index = p_inst->history.latest;
	while (num) {
		if (0 != index) {
			index--;
		} else {
			index = SH_HISTORY_MAX - 1;
		}
		num--;
	}

	err = FALSE;
	*p_history = p_inst->history.cmd[index];
	PRINTF("history: %s \r\n", p_inst->history.cmd[index]);

	return err;
}


static void _handle_cmd(SH_INSTANCE_S *p_inst, uint8_t *p_cmd, uint16_t cmdlen)
{
	int32_t i=0;
	uint8_t paramlen=0;
	uint8_t *p_param=NULL;
	SH_COMMAND_S * const * section;

	section = p_inst->p_cmdlist;
	for(i=0; i<p_inst->cmdlist_size; i++){
		SH_COMMAND_S * const _cmd = *section;
		//FXIME.baek.20220831.
		if(0 == strncmp((const char *)p_inst->cmdbuf, (void *)_cmd->p_cmd, strlen(_cmd->p_cmd))) {
			uint8_t _cmdlen = 0;

			_cmdlen = strlen(_cmd->p_cmd);
			paramlen = p_inst->cmdlen - _cmdlen - 1;
			if( 0 >= paramlen ){
				p_param = NULL;
			}
			else{
				p_param = &p_inst->cmdbuf[_cmdlen+1];
			}

			PRINTF("\r\n");
			if(NULL != _cmd->p_func) {
				if( TRUE == _cmd->p_func(p_param, paramlen) ) {
					cli_history_add(p_inst, (char *)p_inst->cmdbuf);
				} else {
					PRINTF("\r\n-> PARA. ERR\r\n");
					PRINTF(_cmd->p_help);
				}
			} else {
				PRINTF("\r\n-> FUNC. ERR\r\n");
			}
			break;
		}
		section++;
	}

	if( i == p_inst->cmdlist_size ){
		PRINTF("\r\n-> CMD ERR, try: help\r\n\r\n");
	}
}

static bool _is_key(const uint8_t *key, uint8_t *p_str)
{
	if( key[0] == p_str[0] ){
		if( key[1] == p_str[1] ){
			if( key[2] == p_str[2] ){
				return TRUE;
			}
		}
	}
	return FALSE;
}

static void _handle_key(SH_INSTANCE_S *p_inst, uint8_t rbyte)
{
	bool	is_key = FALSE;
	uint8_t *p_hist_cmd = NULL;

	if( 3 <= p_inst->cmdlen ){
		if( TRUE == _is_key((const uint8_t *)KEY_UP, &p_inst->cmdbuf[p_inst->cmdlen-3]) ){
			TERMINAL_MOVE_LEFT(p_inst->cmdlen);
			TERMINAL_CLEAR_END();
			cli_history_show(p_inst, TRUE, &p_hist_cmd);
			is_key = TRUE;
		}
		else if( TRUE == _is_key((const uint8_t *)KEY_DOWN, &p_inst->cmdbuf[p_inst->cmdlen-3]) ){
			TERMINAL_MOVE_LEFT(p_inst->cmdlen);
			TERMINAL_CLEAR_END();
			cli_history_show(p_inst, FALSE, &p_hist_cmd);
			is_key = TRUE;
		}
	}

	if( is_key ) {
		if( p_hist_cmd ){
			memcpy(p_inst->cmdbuf, p_hist_cmd, SH_MAX_BUFSIZE);
			p_inst->cmdlen = (uint16_t)strlen((const char *)p_hist_cmd);
			if( SH_MAX_BUFSIZE > (int32_t)p_inst->cmdlen ){
				p_inst->cmdbuf[p_inst->cmdlen] = '\0';
			}
			else{
				p_inst->cmdlen = (uint16_t)(SH_MAX_BUFSIZE-1);
				p_inst->cmdbuf[p_inst->cmdlen] = '\0';
			}
			PRINTF("%s", p_inst->cmdbuf);
		}
		else{
			TERMINAL_MOVE_LEFT(p_inst->cmdlen);
			TERMINAL_CLEAR_END();
			p_inst->cmdlen = 0;
		}
	}
}

static void linefeed_show_prompt(const char *p_name)
{
	PRINTF("\r\n%s> ", p_name);
	fflush(stdout);
}

static void return_show_prompt(const char *p_name)
{
	PRINTF("\r%s> ", p_name);
	fflush(stdout);
}

void sh_put_byte_impl(SH_INSTANCE_S *p_inst, uint8_t rbyte)
{
	if(p_inst->cmdlen < SH_MAX_BUFSIZE) {
		if ( KEY_BACKSPACE == rbyte ) {
			if (0 < p_inst->cmdlen) {
				PRINTF("%c", rbyte);
				TERMINAL_CLEAR_END();
				fflush(stdout);
				p_inst->cmdlen -= 1;
			}
			else{
				return_show_prompt(p_inst->name);
			}
		}
		else if ( KEY_ENTER == rbyte || KEY_ENTER_ALT == rbyte ) {
			if( 0 == p_inst->cmdlen ){
			}
			else{
				_handle_cmd(p_inst, p_inst->cmdbuf, p_inst->cmdlen);
			}
			p_inst->cmdlen = 0;
			memset(p_inst->cmdbuf, 0x0, SH_MAX_BUFSIZE);
			linefeed_show_prompt(p_inst->name);
		}
		else {
			PRINTF("%c", rbyte);
			fflush(stdout);

			p_inst->cmdbuf[p_inst->cmdlen] = rbyte;
			p_inst->cmdlen++;
			_handle_key(p_inst, rbyte);
		}
	}
	else {
		ERR("Command buffer full. reset.");
		p_inst->cmdlen = 0;
	}
}

int32_t shell_parse_arguments(const uint8_t *p_str, uint8_t str_len, uint8_t p_arg[][16])
{
	int32_t ret=0;
	uint8_t to_parse[128], *p=NULL;

	ASSERT(p_str);
	ASSERT(p_arg);

	memset(to_parse, 0x0, sizeof(uint8_t)*128);
	if( str_len >= 128 || str_len <= 0 ){
		ret = 0;
	}
	else{
		memcpy(to_parse, p_str, sizeof(uint8_t)*str_len);
		p = (uint8_t *)strtok((char *)to_parse, " ");
		do {
			if( NULL != p ){
				uint16_t _len = strlen((const char *)p);
				if( 16 <= _len ){
					ERRA("too long argument : %s(%d)", p, _len);
					ret = 0;
					break;
				}
				else if( 0 == _len ){
					ERR("null argument");
				}
				else{
					memcpy(&p_arg[ret][0], p, _len);
					p_arg[ret][_len] = '\0';
					ret += 1;
					if( ret >= 32 ){
						break;
					}
				}
			}
			else{
				break;
			}
			p = (uint8_t *)strtok((char *)NULL, " ");
		} while(NULL != p_arg);
	}

	return ret;
}

void sh_init_impl(SH_INSTANCE_S *p_inst, SH_COMMAND_S * const *p_cmdlist, uint8_t cmdlist_size)
{
	uint8_t i=0;

	p_inst->p_cmdlist = (SH_COMMAND_S * const *)p_cmdlist;
	p_inst->cmdlist_size = cmdlist_size;

	for(i=0; i<cmdlist_size; i++){
		SH_COMMAND_S * const _cmd = *p_cmdlist;
		if( NULL != _cmd->p_cmd ) {
			if( NULL != _cmd->p_init ) {
				if( FALSE == _cmd->p_init() ) {
					ERRA("\r\n-> FUN[%d] INIT WRONG\r\n", i);
				}
			}
		}
		p_cmdlist++;
	}

	PRINTF(" \r\n");
	TERMINAL_BACK_BLACK();
	TERMINAL_FONT_GREEN();
	TERMINAL_DISPLAY_CLEAR();
	TERMINAL_RESET_CURSOR();
	linefeed_show_prompt(p_inst->name);
}

#pragma section("_shell_section$0_start") 
#pragma section("_shell_section$a_content")
#pragma section("_shell_section$z_end")
SHELL_CMD_INIT(base);
SHELL_CMD(base,	help,	help_help,			NULL,	cli_help);
SHELL_CMD(base,	cls,	shell_help_clear,	NULL,	shell_cmd_clear);

void sh_init(SH_CONFIG *p_config)
{
#if __GNUC__ >= 3
	SH_COMMAND_S* const* p_start_of_section = (uint8_t *)&__start_shell_base_section;
	SH_COMMAND_S* const* p_end_of_section = (uint8_t *)&__stop_shell_base_section;
#else
	SH_COMMAND_S* const* p_start_of_section = (uint8_t *)&__start_shell_section + 0x108;
	SH_COMMAND_S* const* p_end_of_section = (uint8_t *)&__stop_shell_section - 0x104;
#endif

	ASSERT(p_config);
	memset(&s_inst, 0, sizeof(SH_INSTANCE_S));
	s_inst.init = p_config->init;
	s_inst.output = p_config->output;
	snprintf(s_inst.name, sizeof(char)*16, "%s", p_config->name);

	if( NULL != s_inst.init ){
		s_inst.init();
	}
	
	sh_init_impl(&s_inst, p_start_of_section, (p_end_of_section - p_start_of_section));
}

void sh_input(uint8_t *p_byte, uint32_t size)
{
	int i=0;

	for(i=0; i<size; i++){
		sh_put_byte_impl(&s_inst, p_byte[i]);
	}
}

int32_t sh_wait_for_input(const char *p_chars, uint32_t size, uint32_t timeout_ms)
{
	return 0;
}
