// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include "img.h"

// Function: drawBorder
// Draws a border around the image. The border grows 'inwards', in the sense that
// the original dimensions of the image remain unchanged, but the border overwrites
// the outer parts.
//
// Parameters:
//   img - Source image buffer
//   border - Width of the border in pixel, defaults to 0
//
// Returns:
//   Returns 0
//
int drawBorder(Img<int>& img, int border = 0);
