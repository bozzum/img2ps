
#include "lodepng.h"
#include "jpgd.h"
#include "chromaproc.h"
#include "loadfile.h"

int* loadFile(std::string path, std::string algo, int& width, int& height)
{
	int* buf = nullptr;

	try {
		auto dot = path.rfind('.');
		if(dot == std::string::npos)
			return nullptr;
		auto ext = path.substr(dot);

		if(ext == ".png") {
			std::vector<unsigned char> image;
			unsigned w, h;

			auto rc = lodepng::decode(image, w, h, path);
			if(rc)
				throw;

			width = w;
			height = h;

			buf = (int*)malloc(width * height * sizeof(int));
			Img img = { buf, width, height };

			// Chroma to luma conversion
			if(chromaConv(image.data(), img, algo))
				throw;

		} else if(ext == ".jpg" or ext == ".jpeg") {
			int w, h, c;
			auto bbuf = jpgd::decompress_jpeg_image_from_file(path.c_str(), &w, &h, &c, 1);
			if(not bbuf)
				throw;

			width = w;
			height = h;

			buf = (int*)malloc(width * height * sizeof(int));
			for(int i = w * h; i; --i)
				buf[i] = (int)bbuf[i];

			free(bbuf);
		}
	} catch(...) {
		if(buf)
			free(buf);
		return nullptr;
	}

	return buf;
}
