
#pragma once

#include "main.h"
#include "img.h"
#include <cstdio>

int emitPs(FILE* out, Img<int> img, int pxPerIn = 96, int border = 0);
