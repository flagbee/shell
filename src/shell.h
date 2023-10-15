
#ifndef __SHELL_H
#define __SHELL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "global.h"

#define SH_VERSION	"v1.0"
#define SYSTEM_REBOOT()     
#define SH_MAX_BUFSIZE 	(512)
#define SH_HISTORY_MAX	(5)
#define SH_ARGS_MAX_STR_LEN	(16)

typedef int32_t (*init_func)(void);
typedef int32_t (*output_func)(uint8_t *p_bytes, uint32_t size);

typedef PACK(struct _SH_COMMAND_S{
    const char *p_cmd;
    const char *p_help;
    uint8_t (*p_init)(void);
    uint8_t (*p_func)(void *, uint8_t);
}) SH_COMMAND_S;

typedef PACK(struct _SH_HISTORY_S{
	uint8_t	cmd[SH_HISTORY_MAX][SH_MAX_BUFSIZE];
	uint8_t	count;
	uint8_t	latest;
	uint8_t	show;
}) SH_HISTORY_S;

typedef PACK(struct _SH_INSTANCE_S{
	char				name[16];
	SH_COMMAND_S * const *p_cmdlist;
	uint8_t				cmdlist_size;
	uint16_t				cmdlen;
	uint8_t				cmdbuf[SH_MAX_BUFSIZE];
	SH_HISTORY_S		history;
	uint8_t				wait_flag;

	init_func		init;
	output_func		output;
}) SH_INSTANCE_S;

#if __GNUC__ >= 3
#define SHELL_CMD_INIT(_basename) \
	extern SH_COMMAND_S * const __weak __start_shell_##_basename##_section; \
	extern SH_COMMAND_S * const __weak __stop_shell_##_basename##_section
#define SHELL_CMD(_basename, _cmd, _help, _init, _handle) \
	SH_COMMAND_S _basename##_cmd = {#_cmd, _help, _init, _handle}; \
	SH_COMMAND_S * const p##_basename##_cmd __attribute((__section__("shell_"#_basename"_section"))) __attribute((__used__)) = (SH_COMMAND_S *)&_basename##_cmd
#else
#define SHELL_CMD_INIT(_basename) \
	__declspec(allocate("_shell_section$0_start")) int __start_shell_section = 0; \
	__declspec(allocate("_shell_section$z_end")) int __stop_shell_section = 0
#define SHELL_CMD(_basename, _cmd, _help, _init, _handle) \
	SH_COMMAND_S _cmd = {#_cmd, _help, _init, _handle}; \
	__declspec(allocate("_shell_section$a_content")) SH_COMMAND_S * const p##_cmd = (SH_COMMAND_S *)&_cmd
#endif

typedef struct {
	uint8_t	type;
	union {
		struct _uart {
			uint8_t		channel;
			uint32_t	baudrate;
		} uart;
	};
} SH_DEV_CONFIG_S;

int32_t sh_parse_arguments(const uint8_t *p_str, uint8_t str_len, uint8_t p_arg[][16]);
int32_t sh_get_int_from_str(const uint8_t *p_str, int32_t *p_val);
int32_t sh_get_int_from_hexstr(const char *p_str, int32_t *p_val);
int32_t sh_parse_string_to_int_array(int32_t cmdcnt, const uint8_t args[][16], int32_t *p_array);
float sh_parse_string_to_float(const uint8_t args[][16]);
int32_t sh_parse_string_to_float_array(int32_t cmdcnt, const uint8_t args[][16], float *p_array);
uint8_t sh_is_matched(const char *p_str1, const char *p_str2);

typedef struct _SH_CONFIG {
	char		name[16];
	init_func	init;
	output_func	output;
} SH_CONFIG;

void sh_init(SH_CONFIG *p_config);
void sh_input(uint8_t *p_byte, uint32_t size);
int32_t sh_wait_for_input(const char *p_chars, uint32_t size, uint32_t timeout_ms);

#endif /* __SHELL_H */

