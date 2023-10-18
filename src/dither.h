// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "img.h"
#include "main.h"

// Function: dither
//
//
//
//
// Parameters:
//   img - Source image buffer
//   th - Threshold to determine if a pixel is black or white. This parameter has
//        a rather dynamic characteristic.
//   algo - Dither algorithm (TH, FS, JJN or SI)
//
// Returns:
//   Returns 0
//
int dither(Img<int>& img, int th, const std::string& algo);
