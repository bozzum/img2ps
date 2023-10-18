// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include "dither.h"

using Coeff = struct Coeff_st { int num; int den; };

static inline void
adjustPx(Img<int>& img, const Pt& pt, const Coeff& c, int err)
{
	if(pt.x < 0 or pt.x >= img.w or pt.y < 0 or pt.y >= img.h)
		// pixel outside the image -- do nothing
		return;

	img.data[(pt.y * img.w) + pt.x] += (err * c.num) / c.den;
}

static inline void
calcPx(Img<int>& img, const Pt& pt, const Coeff m[3][5], int th)
{
	// calculate new pixel
	int i = (pt.y * img.w) + pt.x;
	int val = img.data[i];
	img.data[i] = val >= th ? 0xff : 0x00;
	int err = val - img.data[i];

	// distribute error
	for(int y = 0; y <= 2; y++)
		for(int x = 0; x <= 4; x++)
			if((m[y][x]).num != 0) {
				Pt np = { pt.x + x - 2, pt.y + y };
				adjustPx(img, np, m[y][x], err);
			}
}

int
dither(Img<int>& img, int th, const std::string& algo)
{
	// coefficients for various dither methods
	Coeff mTH[3][5] = { { {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1} },
						{ {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1} },
						{ {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1} } };
	Coeff mFS[3][5] = { { {0, 16}, {0, 16}, {0, 16}, {7, 16}, {0, 16} },
						{ {0, 16}, {3, 16}, {5, 16}, {1, 16}, {0, 16} },
						{ {0, 16}, {0, 16}, {0, 16}, {0, 16}, {0, 16} } };
	Coeff mJJN[3][5] = { { {0, 48}, {0, 48}, {0, 48}, {7, 48}, {5, 48} },
						 { {3, 48}, {5, 48}, {7, 48}, {5, 48}, {3, 48} },
						 { {1, 48}, {3, 48}, {5, 48}, {3, 48}, {1, 48} } };
	Coeff mSI[3][5] = { { {0, 32}, {0, 32}, {0, 32}, {5, 32}, {3, 32} },
						{ {2, 32}, {4, 32}, {5, 32}, {4, 32}, {2, 32} },
						{ {0, 32}, {2, 32}, {3, 32}, {2, 32}, {0, 32} } };

	Coeff m[3][5] {};
	if(algo == "TH")		// Threshold
		std::copy(&mTH[0][0], &mTH[2][4], &m[0][0]);
	else if(algo == "FS")	// Floyd-Steinberg
		std::copy(&mFS[0][0], &mFS[2][4], &m[0][0]);
	else if(algo == "JJN")	// Jarvis, Judice & Ninke
		std::copy(&mJJN[0][0], &mJJN[2][4], &m[0][0]);
	else if(algo == "SI")	// Sierra
		std::copy(&mSI[0][0], &mSI[2][4], &m[0][0]);
	else
		throw "Unrecognised dither algorithm " + algo;

	// just run over the entire image from top-left to bottom-right
	for(int y = 0; y < img.h; y++)
		for(int x = 0; x < img.w; x++) {
			Pt p{x, y};
			calcPx(img, p, m, th);
		}

	return 0;
}
