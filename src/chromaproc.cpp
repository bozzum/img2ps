
#include <cmath>
#include "chromaproc.h"

int
chromaConv(uint8_t* pSrc, Img<int>& dst, std::string& algo)
{
	double fR, fG, fB;

	if(algo == "C") {
		fR = 0.299; fG = 0.587; fB = 0.114;
	} else if(algo == "B") {
		fR = 0.2126; fG = 0.7152; fB = 0.0722;
	} else if(algo == "S") {
		fR = 0.212; fG = 0.701; fB = 0.087;
	} else
		return -1;

	auto area = dst.w * dst.h;
	for(int n = 0; n < area; n++, pSrc += 4) {
		int val = (int)(round(pSrc[0] * fR + pSrc[1] * fG + pSrc[2] * fB));
		dst.data[n] = std::min(std::max(val, 0), 255);
	}

	return 0;
}
