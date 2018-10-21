#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static lua_State *L;

void c_toLuaInit(void)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfile(L,"meta.lua");
    lua_pcall(L,0,0,0);
    luaL_loadfile(L,"run.lua");
    lua_pcall(L,0,0,0);
    luaL_loadfile(L,"cfg.lua");
    lua_pcall(L,0,0,0);
}

void c_toLuaReload(void)
{
    luaL_loadfile(L,"run.lua");
    lua_pcall(L,0,0,0);
    luaL_loadfile(L,"cfg.lua");
    lua_pcall(L,0,0,0);
}

int c_toLuaGetCfg(void* data,unsigned int len,...)
{
    va_list args;
    char *ag;
    int type,cnt=0,retCode=0;
    va_start(args,len);
    do{
        ag = va_arg(args,char *);
        if(ag == 0)
            break;
        type = cnt==0?lua_getglobal(L,ag):lua_getfield(L,-1,ag);
        cnt++;
        switch(type){
            case LUA_TNUMBER:
                *(unsigned int*)data=lua_tonumber(L,-1);
                break;
            case LUA_TSTRING:
                strncpy(data,lua_tostring(L,-1),len);
                break;
            case LUA_TTABLE:
                continue;
            default:
                retCode=-1;
                break;
        }
        lua_pop(L,cnt);
    }while(1);
    va_end(args);
    return retCode;
} 

int c_toLuaCall(char* fname,void *out,char *argType,...)
{
    va_list args;
    int type,cnt=0;
    type=lua_getglobal(L,fname);
    if(type != LUA_TFUNCTION){
        lua_pop(L,1);
        return -1;
    }
    va_start(args,argType);
    for(;*argType;argType++){
        luaL_checkstack(L,1,"too many arguments");
        cnt++;
        switch(*argType){
            case 'd':
                lua_pushnumber(L,va_arg(args,double));
                break;
            case 'i':
                lua_pushinteger(L,va_arg(args,unsigned int));
                break;
            case 's':
                lua_pushstring(L,va_arg(args,char*));
                break;
            default:
                cnt--;
                break;
        }
    }
    va_end(args);
    if(LUA_OK != lua_pcall(L,cnt,1,0))
        return -2;

    if(out != 0)
        *(lua_Integer*)out = lua_tointeger(L,-1);

    lua_pop(L,1);
    return 0;
}
                
int main(int argc,char* args[])
{
    lua_Integer ret;
    c_toLuaInit();
    c_toLuaCall("show",&ret,"i",10);
    printf("ret from lua : %lld\n",ret);
    c_toLuaGetCfg(&ret,sizeof(ret),"cfgTable","name","sname",0);
    printf("ret from lua : %lld\n",ret);
    lua_close(L);
    return 0;
}

