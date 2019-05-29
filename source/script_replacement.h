#ifndef SCRIPT_REPLACEMENT_H
#define SCRIPT_REPLACEMENT_H

#include <stdint.h>

#include "l2c_imports.h"

void replace_scripts(lib::L2CAgent* l2c_agent, u8 category, int kind);

void* sv_get_status_func(u64 l2c_agentbase, int status_kind, u64 key);
void sv_replace_status_func(u64 l2c_agentbase, int status_kind, u64 key, void* func);
u64 clear_lua_stack_replace(u64 l2c_agent);

#endif // SCRIPT_REPLACEMENT_H
