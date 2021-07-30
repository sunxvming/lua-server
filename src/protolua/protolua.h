#ifndef _JINJIAZHANG_PROTOLUA_H_
#define _JINJIAZHANG_PROTOLUA_H_

#include "lua.hpp"

#if LUA_VERSION_NUM == 501
#include "lua51ext.h"
#endif

#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/compiler/importer.h"

#ifdef _JINJIAZHANG_PROTOLOG_H_
#define proto_trace(fmt, ...)  log_trace(fmt, __VA_ARGS__)
#define proto_debug(fmt, ...)  log_debug(fmt, __VA_ARGS__)
#define proto_info(fmt, ...)   log_info(fmt, __VA_ARGS__)
#define proto_warn(fmt, ...)   log_warn(fmt, __VA_ARGS__)
#define proto_error(fmt, ...)  log_error(fmt, __VA_ARGS__)
#define proto_fatal(fmt, ...)  log_fatal(fmt, __VA_ARGS__)
#else
#define proto_trace(fmt, ...)  printf("[trace]" fmt "\n", __VA_ARGS__)
#define proto_debug(fmt, ...)  printf("[debug]" fmt "\n", __VA_ARGS__)
#define proto_info(fmt, ...)   printf("[info]" fmt "\n", __VA_ARGS__)
#define proto_warn(fmt, ...)   printf("[warn]" fmt "\n", __VA_ARGS__)
#define proto_error(fmt, ...)  printf("[error]" fmt "\n", __VA_ARGS__)
#define proto_fatal(fmt, ...)  printf("[fatal]" fmt "\n", __VA_ARGS__)
#endif

#ifndef lua_toint64
#define lua_toint64 lua_tointeger
#endif

#ifndef lua_pushint64
#define lua_pushint64 lua_pushinteger
#endif

#ifdef _WIN32
#define PROTO_API extern "C" __declspec(dllexport)
#else
#define PROTO_API extern "C"
#endif

#define PROTO_DO(exp) { if(!(exp)) return false; }
#define PROTO_ASSERT(exp) { if(!(exp)) return false; }

bool proto_parse(const char* file, lua_State* L);
bool proto_create(const char* proto, lua_State* L);
bool proto_encode(const char* proto, lua_State* L, int index, char* output, size_t* size);
bool proto_decode(const char* proto, lua_State* L, const char* input, size_t size);
bool proto_pack(const char* proto, lua_State* L, int start, int end, char* output, size_t* size);
bool proto_unpack(const char* proto, lua_State* L, const char* input, size_t size);

extern google::protobuf::compiler::Importer* g_importer;
extern google::protobuf::DynamicMessageFactory* g_factory;

#endif