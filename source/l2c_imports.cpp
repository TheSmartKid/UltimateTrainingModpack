#include <switch.h>

#include <math.h>

#include "lua_bind_hash.h"
#include "useful.h"

#include "l2c_imports.h"

lib::L2CValue::L2CValue() {
	type = L2C_void;
}

lib::L2CValue::L2CValue(bool val) {
	type = L2C_bool;
	raw = val;
}

lib::L2CValue::L2CValue(int val) {
	type = L2C_integer;
	raw = val;
}

lib::L2CValue::L2CValue(u64 val) {
	type = L2C_integer;
	raw = val;
}

lib::L2CValue::L2CValue(float val) {
	if (isnan(val)) {
		type = L2C_void;
	} else {
		type = L2C_number;
		raw_float = val;
	}
}

lib::L2CValue::L2CValue(double val) {
	if (isnan(val)) {
		type = L2C_void;
	} else {
		type = L2C_number;
		raw_float = val;
	}
}

void lib::L2CAgent::get_lua_stack(int index, lib::L2CValue* l2c_val) {
	asm("mov x8, %x0" : : "r"(l2c_val) : "x8" );
	pop_lua_stack(index);
}

int lib::lua_const(const char* str) {
	int val;
	if (lua_bind_get_value(lua_bind_hash_str(str), &val))
		return val;
	else
		return -1;
}
