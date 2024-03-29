// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include "lumaproc.h"

int
stretch(Img<int>& img, const Stat& stat)
{
	auto area = img.w * img.h;
	auto off = -stat.minLuma;
	auto den = stat.maxLuma + off;

	// stretch the luma range to fill the full range
	for(int i = 0; i < area; i++)
		img.data[i] = ((img.data[i] + off) * 255) / den;

	return 0;
}
