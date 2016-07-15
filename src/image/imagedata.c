/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb/stb_image.c"
#include "imagedata.h"


int image_ImageData_new_with_filename(image_ImageData *dst, char const* filename) {
  int n;
  dst->surface = stbi_load(filename, &dst->w, &dst->h, &n, STBI_rgb_alpha);
 
  if(dst->surface == 0 || dst->surface == NULL) //image could not be loaded
    return 0;
  return 1;
}

int image_ImageData_getPixel(image_ImageData *dst, int x, int y) {
	return dst->surface[x+y*dst->w];
}

int image_ImageData_getWidth(image_ImageData *dst) {
	return dst->w;
}

int image_ImageData_getHeight(image_ImageData *dst) {
	return dst->h;
}

const char* image_error(void){
  return stbi_failure_reason();
}

void image_ImageData_new_with_size(image_ImageData *dst, int width, int height) {
  dst->surface = malloc(width*height*4);
  memset(dst->surface, 0, width*height*4);
}

void image_ImageData_free(image_ImageData *data) {
  free(data->surface);
}

void image_init() {
}
