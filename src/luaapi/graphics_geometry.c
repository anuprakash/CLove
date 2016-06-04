/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_geometry.h"
#include "../graphics/geometry.h"
#include "tools.h"
#include "../tools/log.c"

#include <stdio.h>
#include <stdlib.h>

static int l_geometry_circle(lua_State* state) {
  const char* type = l_tools_toStringOrError(state, 1);
  float x = l_tools_toNumberOrError(state, 2);
  float y = l_tools_toNumberOrError(state, 3);
  float radius = l_tools_toNumberOrError(state, 4);
  float segments = l_tools_toNumberOrError(state, 5);

  if (strncmp(type,"line",4) == 0)
    graphics_geometry_drawCircle(x, y, radius, segments);
  else if(strncmp(type,"fill", 4) == 0)
    graphics_geometry_fillCircle(x, y, radius, segments);

  return 1;
}

static int l_geometry_rectangle(lua_State* state) {
  const char* type = l_tools_toStringOrError(state, 1);
  float x = l_tools_toNumberOrError(state, 2);
  float y = l_tools_toNumberOrError(state, 3);
  float w = l_tools_toNumberOrError(state, 4);
  float h = l_tools_toNumberOrError(state, 5);
  float r = 0; 
   if (lua_tonumber(state, 6)) r = luaL_checknumber(state, 6);
  float sx = 1; 
   if (lua_tonumber(state, 7)) sx = luaL_checknumber(state, 7);
  float sy = 1; 
   if (lua_tonumber(state, 8)) sy = luaL_checknumber(state, 8);
 float ox = 0; 
   if (lua_tonumber(state, 9)) ox = luaL_checknumber(state, 9);
 float oy = 0; 
   if (lua_tonumber(state, 10)) oy = luaL_checknumber(state, 10);

  if (strncmp(type,"line",4) == 0)
    graphics_geometry_fillRectangle(0, x, y, w, h, r, sx, sy, ox, oy);
  else if(strncmp(type,"fill", 4) == 0)
    graphics_geometry_fillRectangle(1, x, y, w, h, r, sx, sy, ox, oy);
  else 
    printf("%s \n", "Error: Only 'line' and 'fill' modes are correct");
  
  return 1;
}

static int l_geometry_points(lua_State* state) {
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);  
  
  graphics_geometry_points(x,y);

  return 1;
}

static int l_geometry_vertex(lua_State* state) {
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);
  int vertices[100] = {};
  int _i = 0;
  int count = 1;
  if (lua_tonumber(state, 4)) count = luaL_checknumber(state, 4);
    
  //Check if we need to draw points of lines
  if (lua_istable(state, 3)) { //in this case draw lines
    // Push another reference to the table on top of the stack (so we know
    // where it is, and this function can work for negative, positive and
    // pseudo indices
    lua_pushvalue(state, 3);
    // stack now contains: -1 => table
    lua_pushnil(state);
    // stack now contains: -1 => nil; -2 => table
    while (lua_next(state, -2))
    {
        // stack now contains: -1 => value; -2 => key; -3 => table
        // copy the key so that lua_tostring does not modify the original
        lua_pushvalue(state, -2);
        // stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
        const char *key = lua_tostring(state, -1);
        const char * value = lua_tostring(state, -2);
        int v = atoi(value);
        int i = atoi(key);
        
        //Put the key and the value of the table into an array
       // _i += 1;
       // vertices[_i] = i; // first insert the key 
        //_i += 1;
        vertices[_i] = v; // second insert the value of the key
          _i += 1;
   
        //printf("%d\n", px);
        // pop value + copy of key, leaving original key
        lua_pop(state, 2);
        // stack now contains: -1 => key; -2 => table
    }
    // stack now contains: -1 => table (when lua_next returns 0 it pops the key
    // but does not push anything.)
    // Pop table
    lua_pop(state, 1);
    // Stack is now the same as it was on entry to this function
    //
    graphics_geometry_vertex(x,y,vertices,count);

  } 
  
  graphics_geometry_vertex(x,y,vertices,count);
  
  return 1;
}

static int l_geometry_setLineWidth(lua_State* state) {
  float width = l_tools_toNumberOrError(state, 1);
  //graphics_geometry_setLineWidth(width);
  return 1;
}

static int l_geometry_getLineWidth(lua_State* state) {
  //lua_pushnumber(state, graphics_geometry_getLineWidth());
  return 1;
}

static int l_geometry_gcGeometry(lua_State* state) {
  graphics_geometry_free();
  return 0;
}

static luaL_Reg const geometryFuncs[] = {
  {"__gc",         l_geometry_gcGeometry},
  {"points",       l_geometry_points},
  {"point",       l_geometry_points},
  {"vertex",       l_geometry_vertex},
  {"rectangle",    l_geometry_rectangle},
  {"circle",       l_geometry_circle},
  {"setLineWidth", l_geometry_setLineWidth},
  {"getLineWidth", l_geometry_getLineWidth},
  {NULL,NULL}
};

void l_graphics_geometry_register(lua_State* state) {
  l_tools_registerFuncsInModule(state, "graphics", geometryFuncs);
}
