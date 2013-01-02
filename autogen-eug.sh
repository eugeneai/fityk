#!/bin/bash

LUA_VER=5.2
autoreconf -iv
CPPFLAGS="-I/usr/include/lua$LUA_VER" ./configure --with-lua-suffix=$LUA_VER
