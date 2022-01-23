
#include "emitstats.h"

int
emitstats(FILE* out, Stat stat)
{
	fprintf(out, ".G1\n");
	fprintf(out, "frame invis left solid bot solid\n");
	fprintf(out, "label left \"Samples\"\n");
	fprintf(out, "label bot \"Colour Value\"\n");
	fprintf(out, "draw solid\n");
	for(int i = 0; i < 256; i ++)
		fprintf(out, "%i\n", stat.lCnt[i]);
	fprintf(out, ".G2\n");

	fprintf(out, ".EQ\ndelim $$\n.EN\n");
	fprintf(out, "$Luma sub min = %i$\n.br\n", stat.minLuma);
	fprintf(out, "$Luma sub max = %i$\n.br\n", stat.maxLuma);
	fprintf(out, "$Luma sub avr = %i$\n.br\n", stat.avrLuma);
	fprintf(out, "$Count sub total = %i$\n.br\n", stat.sumCnt);
	fprintf(out, "$Count sub min = %i$\n.br\n", stat.minCnt);
	fprintf(out, "$Count sub max = %i$\n.br\n", stat.maxCnt);
	fprintf(out, "$Count sub unique = %i$\n.br\n", stat.unique);

	return 0;
}
