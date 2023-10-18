// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include "border.h"

int
drawBorder(Img<int>& img, int border)
{
	// Frame
	if(border) {
		Pt tl = {0, 0}, br = {img.w -1, img.h -1};

		while(border-- > 0) {
			img.rect(tl, br);

			tl.x++; tl.y++;
			br.x--; br.y--;
		}
	}

	return 0;
}
