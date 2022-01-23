
#include <cmath>
#include "chromaproc.h"

int
chromaConv(uint8_t* pSrc, Img& dst, std::string algo)
{
	if(algo == "I") {
		auto area = dst.w * dst.h;
		for(int n = 0; n < area; n++, pSrc += 4) {
			int val = (int)(round(pSrc[0] * 0.299 + pSrc[1] * 0.587 + pSrc[2] * 0.114));
			dst.data[n] = std::min(std::max(val, 0), 255);
		}
	} else
		return -1;

	return 0;
}
