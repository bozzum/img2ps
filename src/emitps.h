// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include "main.h"
#include "img.h"
#include <cstdio>

// Function: emitPng
// Renders the dithered image to an (Encapsulated) PostScript file.
//
// Parameters:
//   file - Handle to output file (can be stdout)
//   img - Dithered image buffer
//
// Returns:
//   Returns 0 on success, otherwise it throws an exception.
//
int emitPs(FILE* out, Img<int>& img);
