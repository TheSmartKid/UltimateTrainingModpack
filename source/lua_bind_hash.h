#ifndef LUA_BIND_HASH_H
#define LUA_BIND_HASH_H

#include <stdint.h>
#include <inttypes.h>

uint64_t lua_bind_hash(const void* data_, size_t len);
uint64_t lua_bind_hash_str(const char* str);

#endif // LUA_BIND_HASH_H
