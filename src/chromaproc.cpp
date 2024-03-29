// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include <cmath>
#include "chromaproc.h"

int
chromaConv(const uint8_t* pSrc, Img<int>& dst, const std::string& algo)
{
	double fR, fG, fB;

	if(algo == "C") {
		// CCIR 601 (https://www.itu.int/rec/R-REC-BT.601)
		fR = 0.299; fG = 0.587; fB = 0.114;
	} else if(algo == "B") {
		// BT709 (https://www.itu.int/rec/R-REC-BT.709)
		fR = 0.2126; fG = 0.7152; fB = 0.0722;
	} else if(algo == "S") {
		// SMPTE 240M
		fR = 0.212; fG = 0.701; fB = 0.087;
	} else
		throw "Unknown Chroma-Conversion " + algo;

	auto area = dst.w * dst.h;
	for(int n = 0; n < area; n++, pSrc += 4) {
		int val = (int)(round(pSrc[0] * fR + pSrc[1] * fG + pSrc[2] * fB));
		dst.data[n] = std::min(std::max(val, 0), 255);
	}

	return 0;
}
