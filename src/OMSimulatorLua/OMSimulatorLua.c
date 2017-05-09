#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <OMSimulator.h>

#define REGISTER_LUA_CALL(name) lua_register(L, #name, name)

void push_pointer(lua_State *L, void *p)
{
  void **bp = lua_newuserdata(L, sizeof(p));
  *bp = p;
}

void* topointer(lua_State *L, int index)
{
  void **bp = lua_touserdata(L, index);
  return *bp;
}

//void* oms_newModel();
static int newModel(lua_State *L)
{
  if (lua_gettop(L) != 0)
    return luaL_error(L, "expecting no arguments");

  void *pModel = oms_newModel();
  push_pointer(L, pModel);
  return 1;
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

//void oms_unload(void* model);
static int unload(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);
  oms_unload(model);
  return 0;
}

//void oms_instantiateFMU(void* model, const char* filename, const char* instanceName);
static int instantiateFMU(lua_State *L)
{
  if (lua_gettop(L) != 3)
    return luaL_error(L, "expecting exactly 3 arguments");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);
  luaL_checktype(L, 3, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char* filename = lua_tostring(L, 2);
  const char* instanceName = lua_tostring(L, 3);
  oms_instantiateFMU(model, filename, instanceName);
  return 0;
}

//void oms_setReal(void* model, const char* var, double value);
static int setReal(lua_State *L)
{
  if (lua_gettop(L) != 3)
    return luaL_error(L, "expecting exactly 3 arguments");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);
  luaL_checktype(L, 3, LUA_TNUMBER);

  void *model = topointer(L, 1);
  const char *var = lua_tostring(L, 2);
  double value = lua_tonumber(L, 3);
  oms_setReal(model, var, value);
  return 0;
}

// TODO: setInteger
// TODO: setBoolean
// TODO: setString

//double oms_getReal(void* model, const char* var);
static int getReal(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 arguments");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char *var = lua_tostring(L, 2);

  double value = oms_getReal(model, var);
  lua_pushnumber(L, value);
  return 1;
}
// TODO: getInteger
// TODO: getBoolean
// TODO: getString

//void oms_addConnection(void* model, const char* from, const char* to);
static int addConnection(lua_State *L)
{
  if (lua_gettop(L) != 3)
    return luaL_error(L, "expecting exactly 3 arguments");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);
  luaL_checktype(L, 3, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char *from = lua_tostring(L, 2);
  const char *to = lua_tostring(L, 3);
  oms_addConnection(model, from, to);
  return 0;
}

//void oms_simulate(void* model);
static int simulate(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);
  oms_simulate(model);
  return 0;
}

//oms_status_t oms_doSteps(const void* model, const int numberOfSteps);
static int doSteps(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TNUMBER);

  void *model = topointer(L, 1);
  int numberOfSteps = lua_tointeger(L, 2);
  oms_doSteps(model, numberOfSteps);
  return 0;
}

//oms_status_t oms_stepUntil(const void* model, const double timeValue);
static int stepUntil(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TNUMBER);

  void *model = topointer(L, 1);
  double timeValue = lua_tonumber(L, 2);
  oms_stepUntil(model, timeValue);
  return 0;
}

//void oms_describe(void* model);
static int describe(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);
  oms_describe(model);
  return 0;
}

//void oms_exportDependencyGraph(void* model, const char* prefix);
static int exportDependencyGraph(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char* prefix = lua_tostring(L, 2);
  oms_exportDependencyGraph(model, prefix);
  return 0;
}

//oms_status_t oms_initialize(void* model);
static int initialize(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);
  oms_initialize(model);
  return 0;
}

//oms_status_t oms_terminate(void* model);
static int terminate(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);
  oms_terminate(model);
  return 0;
}

//oms_status_t oms_getCurrentTime(const void* model, double* time);
static int getCurrentTime(lua_State *L)
{
  if (lua_gettop(L) != 1)
    return luaL_error(L, "expecting exactly 1 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);

  void *model = topointer(L, 1);

  double time = 0.0;
  oms_status_t status = oms_getCurrentTime(model, &time);
  lua_pushnumber(L, time);
  return 1;
}

//void oms_setStartTime(void* model, double startTime);
static int setStartTime(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TNUMBER);

  void *model = topointer(L, 1);
  double startTime = lua_tonumber(L, 2);
  oms_setStartTime(model, startTime);
  return 0;
}

//void oms_setStopTime(void* model, double stopTime);
static int setStopTime(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TNUMBER);

  void *model = topointer(L, 1);
  double stopTime = lua_tonumber(L, 2);
  oms_setStopTime(model, stopTime);
  return 0;
}

//void oms_setTolerance(void* model, double tolerance);
static int setTolerance(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TNUMBER);

  void *model = topointer(L, 1);
  double tolerance = lua_tonumber(L, 2);
  oms_setTolerance(model, tolerance);
  return 0;
}

//void oms_setWorkingDirectory(const char* filename);
static int setWorkingDirectory(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char* filename = lua_tostring(L, 2);
  oms_setWorkingDirectory(model, filename);
  return 0;
}

//void oms_setResultFile(void* model, const char* filename);
static int setResultFile(lua_State *L)
{
  if (lua_gettop(L) != 2)
    return luaL_error(L, "expecting exactly 2 argument");
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checktype(L, 2, LUA_TSTRING);

  void *model = topointer(L, 1);
  const char* filename = lua_tostring(L, 2);
  oms_setResultFile(model, filename);
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
  REGISTER_LUA_CALL(newModel);
  REGISTER_LUA_CALL(loadModel);
  REGISTER_LUA_CALL(unload);
  REGISTER_LUA_CALL(instantiateFMU);
  REGISTER_LUA_CALL(setReal);
  REGISTER_LUA_CALL(getReal);
  REGISTER_LUA_CALL(addConnection);
  REGISTER_LUA_CALL(simulate);
  REGISTER_LUA_CALL(doSteps);
  REGISTER_LUA_CALL(stepUntil);
  REGISTER_LUA_CALL(describe);
  REGISTER_LUA_CALL(exportDependencyGraph);
  REGISTER_LUA_CALL(initialize);
  REGISTER_LUA_CALL(terminate);
  REGISTER_LUA_CALL(getCurrentTime);
  REGISTER_LUA_CALL(setStartTime);
  REGISTER_LUA_CALL(setStopTime);
  REGISTER_LUA_CALL(setTolerance);
  REGISTER_LUA_CALL(setWorkingDirectory);
  REGISTER_LUA_CALL(setResultFile);
  REGISTER_LUA_CALL(getVersion);
  return 0;
}
