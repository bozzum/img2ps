// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include "border.h"

int
drawBorder(Img<int>& img, int border)
{
	// Frame
	if(border) {
		Pt tl = {0, 0}, tr = {img.w -1, 0};
		Pt bl = {0, img.h -1}, br = {img.w -1, img.h -1};

		while(border-- > 0) {
			img.line(tl, tr);
			img.line(tr, br);
			img.line(br, bl);
			img.line(bl, tl);

			tl.x++; tl.y++;
			tr.x--; tr.y++;
			bl.x++; bl.y--;
			br.x--; br.y--;
		}
	}

	return 0;
}
