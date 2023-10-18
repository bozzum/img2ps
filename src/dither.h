// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "img.h"
#include "main.h"

// Function: dither
// Dithers the source image from a grayscale image to a b/w one. Various algorithms
// are supported. Each one has their own strengths and weaknesses. Play with
// them for each individual image. The threshold level is not an absolute value
// (except for the Threshold method), and shows some interesting dynamic properties.
//
// Parameters:
//   img - Source image buffer
//   th - Threshold to determine if a pixel is black or white. This parameter has
//        a rather dynamic characteristic.
//   algo - Dither algorithm (TH, FS, JJN or SI)
//
// Returns:
//   Returns 0 on success, otherwise it throws an exception.
//
int dither(Img<int>& img, int th, const std::string& algo);
