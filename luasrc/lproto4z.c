/*
 * proto4z License
 * -----------
 * 
 * proto4z is licensed under the terms of the MIT license reproduced below.
 * This means that proto4z is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2013-2015 YaweiZhang <yawei.zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 */




#include "lua.h"
#include "lauxlib.h"
#include <string.h>
#include <stdbool.h>


int luaopen_protoz_bit(lua_State *L);



static const unsigned short __gc_localEndianType = 1;
static bool islocalLittleEndian()
{
	if (*(const unsigned char *)&__gc_localEndianType == 1)
	{
		return true;
	}
	return false;
}



static unsigned long long streamToInteger(const char stream[])
{
	unsigned long long integer = 0;
	size_t len = sizeof(unsigned long long);
	if (!islocalLittleEndian())
	{
		unsigned char *dst = (unsigned char*)&integer;
		unsigned char *src = (unsigned char*)stream + sizeof(unsigned long long);
		while (len > 0)
		{
			*dst++ = *--src;
			len--;
		}
	}
	else
	{
		memcpy(&integer, stream, len);
	}
	return integer;
}


static int streamToString(lua_State *L)
{
	const char * stream = luaL_checkstring(L, -1);
	unsigned long long integer = 0;
	size_t len = sizeof(unsigned long long);
	char buf[50];
	if (!islocalLittleEndian())
	{
		unsigned char *dst = (unsigned char*)&integer;
		unsigned char *src = (unsigned char*)stream + sizeof(unsigned long long);
		while (len > 0)
		{
			*dst++ = *--src;
			len--;
		}
	}
	else
	{
		memcpy(&integer, stream, len);
	}
	lua_settop(L, 0);
	
#ifdef WIN32  
	sprintf(buf, "%I64u", integer);
#else
	sprintf(buf, "%llu", integer);
#endif
	lua_pushstring(L, buf);
	return 1;
}

static int stringToStream(lua_State *L)
{
	const char * stream = luaL_checkstring(L, -1);
	unsigned long long integer = 0;
	unsigned long long org = 0;
	size_t len = sizeof(unsigned long long);
#ifdef WIN32  
	sscanf(stream, "%I64u", &org);
#else
	sscanf(stream, "%llu", &org);
#endif
	stream = (const char *)&org;
	if (!islocalLittleEndian())
	{
		unsigned char *dst = (unsigned char*)&integer;
		unsigned char *src = (unsigned char*)stream + len;
		while (len > 0)
		{
			*dst++ = *--src;
			len--;
		}
	}
	else
	{
		integer = org;
	}
	
	lua_settop(L, 0);
	lua_pushlstring(L, (const char *)&integer, 8);
	return 1;
}


//����һ�������Ķ�����λ�Ƿ�Ϊ1
//param1 ui64
//param2 0~63
static int checkBitTrue(lua_State *L)
{
	size_t len = 0;
	const char * log = luaL_checklstring(L, 1, &len);
	size_t pos = luaL_checkinteger(L, 2);
	if (len != 8)
	{
		return 0;
	}
	lua_settop(L, 0);
	unsigned long long val = streamToInteger(log);
	if (val & (1ULL << pos))
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;
}

static int checkStringToBit(lua_State *L)
{
	size_t i, len = 0;
	const char * log = luaL_checklstring(L, 1, &len);
	unsigned long long pos = 0;
	if (len > 64)
	{
		return 0;
	}
	
	for (i = 0; i < len; i++)
	{
		if (log[i] == '1')
		{
			pos |= (1ULL << i);
		}
		else
		{
			pos &= ~(unsigned long long)(1ULL << i);
		}
	}

	lua_pushlstring(L, (const char*)&pos, 8);
	return 1;
}


static luaL_Reg checkBit[] = {
	{ "__checkBitTrue", checkBitTrue },
	{ "__checkStringToBit", checkStringToBit },
	{ "stringToStream", stringToStream },
	{ "streamToString", streamToString },

	{ NULL, NULL }
};


int luaopen_protoz_bit(lua_State *L)
{
	lua_getglobal(L, "Proto4z");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, "Proto4z");
	}
	
	luaL_Reg *l;
	for (l = checkBit; l->name != NULL; l++) {
		lua_pushcclosure(L, l->func, 0);  /* closure with those upvalues */
		lua_setfield(L, -2, l->name);
	}
	return 0;
}

