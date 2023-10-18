// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "main.h"
#include "img.h"

// Function: chromaConv
//
//
//
//
// Parameters:
//   pSrc - Source image buffer (RGBA, 8bit)
//   dst - Destination image buffer
//   algo - Conversion algorithm (C, B or S)
//
// Returns:
//   Returns 0
//
int chromaConv(const uint8_t* pSrc, Img<int>& dst, const std::string& algo);
