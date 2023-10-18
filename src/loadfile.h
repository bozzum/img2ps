// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "main.h"

// Function: loadFile
// Loads an image file (either PNG or Jpeg) and stores it as a gray scale image
// in a buffer. Alternatively, it can generate a test pattern, which allows to
// calibrate and fine-tweak the result at the troff/groff backend.
//
// Parameters:
//   path - Path and name of the image source file (or ::test for the internal
//          test pattern generator)
//   algo - Colour conversion algorithm (see <chromaproc>) for PNG sources
//   width - Will be set by the file readers (PNG and Jpeg) or will be read by
//           the test pattern generator
//   height - Will be set by the file readers (PNG and Jpeg) or will be read by
//            the test pattern generator
//   ppi -- Pixels per Inch, used by the test pattern generator
//
// Returns:
//   Returns a pointer to the image buffer on success, otherwise it throws an
//   exception.
//
int* loadFile(const std::string& path, std::string& algo, int& width, int& height, int ppi);
