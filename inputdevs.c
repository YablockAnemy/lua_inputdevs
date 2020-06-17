#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lua.h>
#include <linux/input.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int inputdevs_get_devs(lua_State *L) {
	char *dir_path;
	DIR *dir_stream;
	struct dirent *dirent;
	
	if(lua_gettop(L) == 1) {
		luaL_checktype(L, 1, LUA_TSTRING);
		dir_path = (char*)lua_tostring(L, 1);
		
		if(!strcmp(dir_path, "default")) dir_path = "/dev/input/by-id/";
		else if(!strcmp(dir_path, "by-id")) dir_path = "/dev/input/by-id/";
		else if(!strcmp(dir_path, "by-path")) dir_path = "/dev/input/by-path/";
		else return luaL_argerror(L, 1, "invalid value");
		
	} else dir_path = "/dev/input/";
	
	if(!(dir_stream = opendir(dir_path))) luaL_error(L, strerror(errno));
	
	lua_newtable(L);
	for(int i = 1; (dirent = readdir(dir_stream)); i++)
		if(!strcmp(dirent->d_name, ".")) i--;
		else if(!strcmp(dirent->d_name, "..")) i--;
		else if(!strcmp(dirent->d_name, "by-id")) i--;
		else if(!strcmp(dirent->d_name, "by-path")) i--;
		else {
			lua_pushinteger(L, i);
			lua_pushfstring(L, "%s%s", dir_path, dirent->d_name);
			lua_settable(L, -3);
	}
	
	if(closedir(dir_stream)) luaL_error(L, strerror(errno));
	
	return 1;
}

int inputdevs_open(lua_State *L) {
	char *path;
	char *mode;
	FILE *stream;
	
	luaL_checktype(L, 1, LUA_TSTRING);
	path = (char*)lua_tostring(L, 1);
	
	if(lua_gettop(L) > 1){
		luaL_checktype(L, 2, LUA_TSTRING);
		mode = (char*)lua_tostring(L, 2);
		
		if(strcmp(mode, "r"))
		if(strcmp(mode, "r+"))
		if(strcmp(mode, "w"))
		if(strcmp(mode, "w+"))
		if(strcmp(mode, "a"))
		if(strcmp(mode, "a+"))
			return luaL_argerror(L, 2, "invalid value");
		
	} else mode = "a+";
	
	if(!(stream = fopen(path, mode))) luaL_error(L, strerror(errno));
	
	lua_pushlightuserdata(L, stream);
	
	return 1;
}

int inputdevs_close(lua_State *L) {
	
	luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
	if(fclose((FILE*)lua_touserdata(L, 1))) luaL_error(L, strerror(errno));
	
	return 0;
}

int luaopen_inputdevs(lua_State *L) {
	luaL_checkversion(L);
	
	luaL_Reg inputdevs_reg[] = {
		{"get_devs", inputdevs_get_devs},
		{"open", inputdevs_open},
		{"close", inputdevs_close},
		{NULL, NULL}
	};
	
	luaL_newlib(L, inputdevs_reg);
	return 1;
}
