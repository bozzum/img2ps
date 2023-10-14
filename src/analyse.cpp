
#include <cstdio>
#include "analyse.h"

int
analyse(const Img<int>& img, Stat& stat)
{
	try {
		stat = {};
		auto area = img.w * img.h;

		// analysis: quality
		for(int n = 0; n < area; n++) {
			auto val = img.data[n];
			stat.lCnt.at(val)++;
			stat.sumLuma += val;
			stat.minLuma = std::min(stat.minLuma, val);
			stat.maxLuma = std::max(stat.maxLuma, val);
		}
		stat.avrLuma = stat.sumLuma / area;

		// analysis: quantity
		for(int i = 0; i < 256; i++) {
			auto val = stat.lCnt.at(i);
			if(val)
				stat.unique++;
			stat.minCnt = std::min(stat.minCnt, val);
			stat.maxCnt = std::max(stat.maxCnt, val);
		}

		stat.sumCnt = area;

	} catch(std::exception& e) {
		fprintf(stderr, "Error whilst analyzing the image: %s\n", e.what());
	}

	return 0;
}
