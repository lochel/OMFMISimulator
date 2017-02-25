#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <OMSimulator.h>

void push_pointer(lua_State *L, void *p)
{
  void **bp = lua_newuserdata(L, sizeof(p));
  *bp = p;
}

void* pop_pointer(lua_State *L)
{
  void **bp = lua_touserdata(L, -1);
  return *bp;
}

//void* oms_loadModel(const char* filename);
static int loadModel(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TSTRING);

  const char* filename = lua_tostring(L, -1);
  void *pModel = oms_loadModel(filename);
  push_pointer(L, pModel);
  return 1;
}

//void* oms_loadComposite(const char* filename);

//void oms_simulate(void* model);
static int simulate(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *pModel = pop_pointer(L);
  oms_simulate(pModel);
  return 0;
}

//void oms_describe(void* model);
static int describe(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *pModel = pop_pointer(L);
  oms_describe(pModel);
  return 0;
}

//void oms_unload(void* model);
static int unload(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *pModel = pop_pointer(L);
  oms_unload(pModel);
  return 0;
}
    
//void oms_setWorkingDirectory(const char* filename);
static int setWorkingDirectory(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TSTRING);

  const char* tempDir = lua_tostring(L, -1);
  oms_setWorkingDirectory(tempDir);
  return 0;
}
  
//void oms_setResultFile(const char* filename);
//void oms_setStartTime(double startTime);
//void oms_setStopTime(double stopTime)
static int setStopTime(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TNUMBER);

  double stopTime = lua_tonumber(L, -1);
  oms_setStopTime(stopTime);
  return 0;
}

//void oms_setTolerance(double tolerance);
static int setTolerance(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TNUMBER);

  double tolerance = lua_tonumber(L, -1);
  oms_setTolerance(tolerance);
  return 0;
}

//const char* oms_getVersion();
static int getVersion(lua_State *L)
{
  if (lua_gettop(L) != 0)
    return luaL_error(L, "expecting no arguments");

  const char* version = oms_getVersion();
  lua_pushstring(L, version);
  return 1;
}

int luaopen_libOMSimulatorLua(lua_State *L)
{
  lua_register(L, "loadModel", loadModel);
  lua_register(L, "simulate", simulate);
  lua_register(L, "describe", describe);
  lua_register(L, "unload", unload);
  lua_register(L, "setWorkingDirectory", setWorkingDirectory);
  lua_register(L, "setStopTime", setStopTime);
  lua_register(L, "setTolerance", setTolerance);
  lua_register(L, "getVersion", getVersion);
  return 0;
}
