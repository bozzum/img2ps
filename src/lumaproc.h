// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include "main.h"
#include "img.h"
#include "analyse.h"

// Function: stretch
// Stretches the luma range of the image, effectively increasing the contrast
// range of the image. As a future feature we could/should get a bit more
// sophisticated and use the histogram (in the stat buffer) in a better way.
//
// Parameters:
//   img - Source image buffer
//   stat - Buffer with statistic data (from <analyse>)
//
// Returns:
//   Returns 0.
//
int stretch(Img<int>& img, const Stat& stat);
