/*
** Lua BitOp -- a bit operations library for Lua 5.1/5.2.
** http://bitop.luajit.org/
**
** Copyright (C) 2008-2012 Mike Pall. All rights reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/


#define LUA_LIB
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

#include <iostream>
#include <string.h>
static const unsigned short __gc_localEndianType = 1;
static inline bool islocalLittleEndian()
{
	if (*(const unsigned char *)&__gc_localEndianType == 1)
	{
		return true;
	}
	return false;
}


template<class Integer>
static Integer streamToInteger(const char stream[sizeof(Integer)])
{
	unsigned short integerLen = sizeof(Integer);
	Integer integer = 0;
	if (integerLen == 1)
	{
		integer = (Integer)stream[0];
	}
	else
	{
		if (!islocalLittleEndian())
		{
			unsigned char *dst = (unsigned char*)&integer;
			unsigned char *src = (unsigned char*)stream + integerLen;
			while (integerLen > 0)
			{
				*dst++ = *--src;
				integerLen--;
			}
		}
		else
		{
			memcpy(&integer, stream, integerLen);
		}
	}
	return integer;
}


static int checkBitTrue(lua_State *L)
{
	size_t len = 0;
	const char * log = luaL_checklstring(L, 1, &len);
	size_t pos = luaL_checkinteger(L, 1);
	if (len != 8)
	{
		return 0;
	}
	unsigned long long val = streamToInteger<unsigned long long>(log);
	if (val | (1ULL << pos))
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
	size_t len = 0;
	const char * log = luaL_checklstring(L, 1, &len);
	unsigned long long pos = 0;
	if (len > 64)
	{
		return 0;
	}
	
	for (size_t i = 0; i < len; i++)
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


luaL_Reg protoz_bit[] = {
	{ "checkBitTrue", checkBitTrue },
	{ "checkStringToBit", checkStringToBit },

	{ NULL, NULL }
};


int luaopen_protoz_bit(lua_State *L)
{
	lua_newtable(L);
	for (luaL_Reg *l = protoz_bit; l->name != NULL; l++) {
		lua_pushcclosure(L, l->func, 0);  /* closure with those upvalues */
		lua_setfield(L, -2, l->name);
	}
	lua_setglobal(L, "Protoz_bit");
	return 0;
}

