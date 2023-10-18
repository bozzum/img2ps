// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include <stdexcept>
#include "lodepng.h"
#include "emitpng.h"

int
emitPng(const std::string& filename, const Img<int>& img)
{
	const uint8_t WHITE = 0xff;
	const uint8_t BLACK = 0x00;

	// first, we need to convert the integer-based dither buffer to an 8bit
	// RGBA buffer, which can be fed to the PNG encoder
	auto area = img.w * img.h;
	auto dst = new uint8_t[area * 4];
	memset(dst, WHITE, area * 4);

	for(int n = 0; n < area; n++) {
		if(not img.data[n]) {
			auto i = n * 4;
			dst[i + 0] = dst[i + 1] = dst[i + 2] = BLACK;
		}
	}

	// now we just let the encoder do the work
	auto rc = lodepng::encode(filename, dst, img.w, img.h);
	if(rc) {
		delete[] dst;
		throw std::string("Failed to emit PNG file: ") + lodepng_error_text(rc);
	}

	delete[] dst;

	return 0;
}
