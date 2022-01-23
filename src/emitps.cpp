
#include <ctime>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include "emitps.h"

int
emitPs(FILE* out, Img img, int pxPerIn, int border)
{
	// pack image into a bit-buffer; lines are 8-bit aligned
	auto w8 = (img.w + 7) /8;
	auto bitbuf = new uint8_t[w8 * img.h];
	memset(bitbuf, 0, w8 * img.h);

	int i = 0, j = 0;
	for(int y = 0; y < img.h; y++) {
		for(int x = 0; x < img.w; j++) {
			if(img.data[j])
				bitbuf[i] |= 0x80 >> (x & 0x07);
			if((++x & 0x07) == 0x00)
				i++;
		}
		if(img.w & 0x07)
			i++;
	}

	double w = (double)img.w;
	double h = (double)img.h;
	double l = 0.0;
	double t = -24.0;
	auto tm = time(nullptr);
	char* now = asctime(localtime(&tm));

	fprintf(out, "%%!PS-Adobe-3.0\n");
	fprintf(out, "%%%%BoundingBox: %.4f %.4f %.4f %.4f\n", l, t, w, h);
	fprintf(out, "%%%%Creator: Pat's png2ps dither tool\n");
	fprintf(out, "%%%%CreationDate: %s", now);
	fprintf(out, "%%%%DocumentData: Clean7Bit\n");
	fprintf(out, "%%%%LanguageLevel: 2\n");
	fprintf(out, "%%%%Pages: 0\n");
	fprintf(out, "%%%%EndComments\n");
	fprintf(out, "%%%%BeginProlog\n");
	fprintf(out, "1 dict begin\n");
	fprintf(out, "%%EndProlog\n");
	fprintf(out, "gsave\n");
	fprintf(out, "%.4f %.4f translate\n", l, h);
	fprintf(out, "%.4f %.4f scale\n", w, -h);
	fprintf(out, "%.4f %.4f 1\n", w, h);
	fprintf(out, "[ %.4f 0 0 %.4f 0 0 ]\n", w, h);
	fprintf(out, "/bitstr %i string def\n", ((img.w + 7) /8) * 2);
	fprintf(out, "{currentfile bitstr readhexstring pop}\n");
	fprintf(out, "image\n");

	uint8_t* buf = bitbuf;
	for(int y = img.h; y; --y) {
		int c = w8;
		while(c) {
			int n = std::min(c, 40);
			for(int x = n; x ; --x)
				fprintf(out, "%02x", *buf++);
			c -= n;
			fprintf(out, "\n");
		}
	}

	if(border) {
		fprintf(out, "grestore\n0 0 translate\n1 1 scale\n");
		fprintf(out, "%f setlinewidth\n", border * 72.0 / pxPerIn);
		fprintf(out, "newpath\n");
		fprintf(out, "0 0 moveto\n");
		fprintf(out, "0 %i lineto\n", img.h);
		fprintf(out, "%i %i lineto\n", img.w, img.h);
		fprintf(out, "%i 0 lineto\n", img.w);
		fprintf(out, "0 0 lineto\n");
		fprintf(out, "stroke\n");
	}

	fprintf(out, "%%%%Trailer\n");
	fprintf(out, "end\n");
	fprintf(out, "%%%%EOF\n");

	delete[] bitbuf;

	return 0;
}
