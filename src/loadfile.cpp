// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include <stdexcept>
#include "lodepng.h"
#include "jpgd.h"
#include "chromaproc.h"
#include "loadfile.h"

int* loadFile(const std::string& path, std::string& algo, int& width, int& height, int ppi)
{
	int* buf = nullptr;

	try {
		if(path == "::test") {
			width = 5 * ppi;
			height = 5 * ppi;

			buf = new int[width * height];
			Img<int> img = Img<int>(buf, width, height);

			img.set();
			img.rect(Pt(0, 0), Pt(width -1, height -1));

			img.line(Pt(0, 1*ppi), Pt(width -1, 1*ppi));
			img.line(Pt(0, 2*ppi), Pt(width -1, 2*ppi));
			img.line(Pt(0, 3*ppi), Pt(width -1, 3*ppi));
			img.line(Pt(0, 4*ppi), Pt(width -1, 4*ppi));

			img.line(Pt(1*ppi, 0), Pt(1*ppi, height -1));
			img.line(Pt(2*ppi, 0), Pt(2*ppi, height -1));
			img.line(Pt(3*ppi, 0), Pt(3*ppi, height -1));
			img.line(Pt(4*ppi, 0), Pt(4*ppi, height -1));

			img.rect(Pt(0, 0), Pt(1*ppi, 1*ppi), ColBlack, true);
			img.rect(Pt(4*ppi, 0), Pt(width -1, 1*ppi), ColBlack, true);
			img.rect(Pt(0, 4*ppi), Pt(1*ppi, height -1), ColBlack, true);
			img.rect(Pt(4*ppi, 4*ppi), Pt(width -1, height -1), ColBlack, true);

			img.ellipse(Pt(width/2, height/2), width/4, height/4, ColGrey, true);

			return buf;
		}

		auto dot = path.rfind('.');
		if(dot == std::string::npos)
			return nullptr;
		auto ext = path.substr(dot);

		if(ext == ".png") {
			std::vector<unsigned char> image;
			unsigned w, h;

			auto rc = lodepng::decode(image, w, h, path);
			if(rc)
				throw std::string("Failed to load PNG file: ") + lodepng_error_text(rc);

			width = w;
			height = h;

			buf = new int[width * height];
			Img<int> img = Img<int>(buf, width, height);

			// Chroma to luma conversion
			chromaConv(image.data(), img, algo);

		} else if(ext == ".jpg" or ext == ".jpeg") {
			int w, h, c;
			auto bbuf = jpgd::decompress_jpeg_image_from_file(path.c_str(), &w, &h, &c, 1);
			if(not bbuf)
				throw std::string("Failed to load JPEG file");

			width = w;
			height = h;

			buf = new int[width * height];
			for(int i = 0; i < w * h; i++)
				buf[i] = (int)(unsigned)bbuf[i];
			free(bbuf);
		}
	} catch(...) {
		if(buf)
			delete[] buf;
		throw;
	}

	return buf;
}
