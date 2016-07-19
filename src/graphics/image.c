/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "image.h"
#include "../image/imagedata.h"
#include "../math/vector.h"
#include "graphics.h"
#include "vertex.h"
#include "shader.h"
#include <stdio.h>

static struct {
  GLuint vbo;
  GLuint ibo;
  GLuint vao;
} moduleData;

static graphics_Vertex const imageVertices[] = {
  {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};
static unsigned char const imageIndices[] = { 0, 1, 2, 3 };

void graphics_image_init(void) {  

}

static const graphics_Wrap defaultWrap = {
  .verMode = graphics_WrapMode_clamp,
  .horMode = graphics_WrapMode_clamp
};

static const graphics_Filter defaultFilter = {
  .maxAnisotropy = 1.0f,
  .mipmapLodBias = 1.0f,
  .minMode = graphics_FilterMode_linear,
  .magMode = graphics_FilterMode_linear,
  .mipmapMode = graphics_FilterMode_none
};

void graphics_Image_new_with_ImageData(graphics_Image *dst, image_ImageData *data) {
  glGenBuffers(1, &moduleData.vbo);
  glGenBuffers(1, &moduleData.ibo);
  
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageIndices), imageIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));

  graphics_Image_refresh(dst,data);
}

void graphics_Image_refresh(graphics_Image *img, image_ImageData const *data) {
  

  // Create the OpenGL texture
  glGenTextures(1, &img->texID);
  glBindTexture(GL_TEXTURE_2D, img->texID);
  
  graphics_Image_setFilter(img, &defaultFilter);
  graphics_Image_setWrap(img, &defaultWrap);
  
  img->width = data->w;
  img->height = data->h;
 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->w, data->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->surface);
}

void graphics_Image_free(graphics_Image *obj) {
  glDeleteTextures(1, &obj->texID);
  glDeleteBuffers(1, &moduleData.ibo);
  glDeleteBuffers(1, &moduleData.vbo);
}

void graphics_Image_setFilter(graphics_Image *img, graphics_Filter const* filter) {
  graphics_Texture_setFilter(img->texID, filter);
}

void graphics_Image_getFilter(graphics_Image *img, graphics_Filter *filter) {
  graphics_Texture_getFilter(img->texID, filter);
}

void graphics_Image_setWrap(graphics_Image *img, graphics_Wrap const* wrap) {
  glBindTexture(GL_TEXTURE_2D, img->texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap->horMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap->verMode);
}

void graphics_Image_getWrap(graphics_Image *img, graphics_Wrap *wrap) {
  glBindTexture(GL_TEXTURE_2D, img->texID);
  glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int*)&wrap->horMode);
  glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int*)&wrap->verMode);
}

void graphics_Image_draw(graphics_Image const* image, graphics_Quad const* quad,
                         float x, float y, float r, float sx, float sy,
                         float ox, float oy, float kx, float ky) {
  
  mat4x4 tr2d; 
  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STREAM_DRAW);
  m4x4_newTransform2d(&tr2d, x, y, r, sx, sy, ox, oy, kx, ky);
  
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, image->texID);
  
 graphics_drawArray(quad, &tr2d,  moduleData.ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                    graphics_getColor(), image->width * quad->w, image->height * quad->h);

}
