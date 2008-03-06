#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include "tactics.h"
#include "en_phonology.h"
#include "misc.h"

const char *roots[] = {
	"*b", "*bb", "*bq", "*d", "*dm", "*dn", "*gr", "*hr", "*lp", "*mm", "*mn", "*mr", "*pd", "*pl", "*sd", "*wd", "*zr",
	"bd", "dr", "lm", "mm", "mr", "ms", "qb", "rb", "rp", "rq", "sr", "sd", "ttr", "tr", "wr", "zz", "b*r", "bl", "bdl",
	"bdw", "bhm", "bhw", "bhr", "bkr", "bn", "brk", "brr", "bsm", "dbq", "dbr", "dgn", "dl", "dm", "dpr", "drn", "drs",
	"dwd", "drwr", "dbb", "dhb", "dkr", "dnb", "drw", "gbb", "gbl", "gbr", "gdr", "ghd", "glb", "gll", "glm", "gmd", "gml",
	"gmr", "gnb", "gnn", "gpr", "grr", "gzr", "gdn", "gt", "gwl", "gwr", "gzw", "hbb", "hdg", "hdy", "hgr", "hlk", "hll",
	"hmz", "hnn", "hrr", "hrs", "hsm", "hdt", "hdr", "hgb", "hgg", "hkm", "hlb", "khl", "kll", "kmn", "kms", "kpp", "kpr",
	"krb", "kwl", "kwn", "kws", "l*", "l*k", "lb*", "lbn", "lmd", "lwq", "mlk", "mnn", "mnw", "mr*", "mrr", "msr", "mt",
	"mwt", "nbr", "n*m", "n*r", "ngd", "nhr", "nqb", "nqr", "ntn", "nwb", "nwn", "nwr", "pll", "pqr", "prd", "prh", "ptw",
	"q*d", "qbl", "qbw", "qdm", "qhr", "qhw", "qnw", "qpl", "qr", "qr*", "qrb", "qrm", "qry", "qsm", "qwd", "qww", "r*b",
	"r*s", "rb", "rbb", "rgb", "rgl", "rgm", "rp*", "rwm", "rzm", "rzz", "sdr", "sg", "skk", "smd", "smk", "sm", "spr",
	"sq", "swd", "srr", "skm", "sqq", "smq", "sr", "srb", "srp", "stt", "sb", "sbt", "swl", "sdd", "sg", "sgh", "shl",
	"skn", "skr", "slm", "slt", "sms", "smm", "smn", "sn", "snn", "spr", "sq", "sqm", "sqs", "sw*", "sw", "swr", "sbr",
	"sdq", "shb", "shq", "slh", "sn", "spn", "spp", "spr", "sq", "srp", "srr", "swp", "sb*", "sh", "srb", "srw", "thm",
	"tht", "tll", "tmm", "tmr", "twm", "tww", "tmn", "tn", "tpt", "tql", "tb", "thn", "thp", "trh", "trp", "twq", "tll",
	"wd", "wbl", "wdd", "wgn", "wkl", "wq", "wrh", "wew", "wsm", "wsp", "wsl", "wtp", "wtb", "wt", "wzr", "zbl", "zmr", "zqr", 0
};

const char *sonorants1[] = {
	"a", "e", "i", "o", "u", 0
};

const char *sonorants2[] = {
	      " a", " e", " i", " o", " u",
	"a ", "aa", "ae", "ai", "ao", "au",
	"e ", "ea", "ee", "ei", "eo", "eu",
	"i ", "ia", "ie", "ii", "io", "iu",
	"o ", "oa", "oe", "oi", "oo", "ou",
	"u ", "ua", "ue", "ui", "uo", "uu", 0
};

class Generator : public SeededGenerator {

	typedef Distribution<const char*> StringDistribution;

	StringDistribution	rootFreq, sono2Freq;
	int maxRoots, maxSonorants2;

public:
	Generator(Seed &seed) : SeededGenerator(seed) {
		int i = 0;
		maxRoots = 0;
		while (roots[i]) {
			rootFreq.addItem(roots[i++], 10);
			maxRoots += 10;
		}

		i = 0;
		maxSonorants2 = 0;
		while (sonorants2[i]) {
			sono2Freq.addItem(sonorants2[i++], 10);
			maxSonorants2 += 10;
		}
	}

	void generate(char *text) {
		const char *r[2];

		r[0] = rootFreq.getItem(_seed.getBits(maxRoots));
		r[1] = sono2Freq.getItem(_seed.getBits(maxSonorants2));

		sprintf(text, "%s-%s", r[0], r[1]);
	}

};

RandSeed seed(0);
/*
int main(int argc, char *argv[]) {

	char text[100];

	Generator gen(seed);

	for (int i = 0; i < 10; i++) {
		gen.generate(text);
		printf("%s\n", text);
	}

	return 0;
}
*/
