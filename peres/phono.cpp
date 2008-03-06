#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include "tactics.h"
#include "en_phonology.h"
#include "misc.h"


#define ARRAYSIZE(a) (sizeof(a)/sizeof((a[0])))

typedef Distribution<Segment> SegmentDistribution;

void en_setupOnsets(SegmentDistribution &dist);
void en_setupNuclei(SegmentDistribution &dist);
void en_setupCodas(SegmentDistribution &dist);



class EnglishSyllableGenerator : public SeededGenerator {

protected:
	SegmentDistribution		codas;
	SegmentDistribution		onsets;
	SegmentDistribution		nuclei;

	// enforce 's'C1VC2 rule where V is a short vowel and C1/C2 must be different
	bool rule0(const Syllable &syllable) const {
		if (!syllable.hasOnset() || !syllable.hasCoda())
			return true;

		Phoneme s = syllable.onset.first();
		if (!s.hasProps( FRICATIVE | ALVEOLAR | VOICELESS ))
			return true;

		Phoneme c1 = syllable.onset.last();
		Phoneme c2 = syllable.coda.first();

		if (c1 != c2)
			return true;

		return !syllable.nucleus.isShortVowel();
	}

	bool validateSyllable(const Syllable &syllable) const {
		return rule0(syllable);
	}

	virtual void genSyllable(Syllable &s) = 0;

public:
	EnglishSyllableGenerator(Seed &seed) : SeededGenerator(seed) {
		en_setupCodas(codas);
		en_setupOnsets(onsets);
		en_setupNuclei(nuclei);
	}

};

class EnglishOpenSyllableGenerator : public EnglishSyllableGenerator {
public:
	EnglishOpenSyllableGenerator(Seed &seed) : EnglishSyllableGenerator(seed) {
	}
	virtual void genSyllable(Syllable& s) {
		do {
			s.onset = onsets.getItem(_seed.getBits(onsets.cumFreq()));
			s.nucleus = nuclei.getItem(_seed.getBits(nuclei.cumFreq()));
		} while (!validateSyllable(s));
	}
};

class EnglishClosedSyllableGenerator : public EnglishSyllableGenerator {
public:
	EnglishClosedSyllableGenerator(Seed &seed) : EnglishSyllableGenerator(seed) {
	}
	virtual void genSyllable(Syllable& s) {
		do {
			s.onset = onsets.getItem(_seed.getBits(onsets.cumFreq()));
			s.nucleus = nuclei.getItem(_seed.getBits(nuclei.cumFreq()));
			s.coda = codas.getItem(_seed.getBits(codas.cumFreq()));
		} while (!validateSyllable(s));

		return;
	}
};

enum type {
	kNone,
	kVowel,
	kConsonant
} ;

bool isVowel(char c) {
	return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool isConsonant(char c) {
	return !isVowel(c);
}

type getType(char c) {
	if (c == 0) return kNone;
	return isVowel(c) ? kVowel : kConsonant;
}

#define MAX_SEGS	10
extern Phoneme phonemes[];

class Word {
	Segment	segs[MAX_SEGS];
	int numSegs;

public:
	Word(Syllable *syllables, int numSyllables) : numSegs(0) {
		for (int i = 0; i < numSyllables; i++) {
			if (syllables[i].hasOnset()) {
				segs[numSegs++] = syllables[i].onset;
			}

			segs[numSegs++] = syllables[i].nucleus;

			if (syllables[i].hasCoda()) {
				segs[numSegs++] = syllables[i].coda;
			}
		}
	}

	bool validate() {
		int i;

		int complexClusters = 0;
		for (i = 0; i < numSegs; i++) {
			complexClusters += (segs[i].isComplexCluster() ? 1 : 0);
		}

		int freq[39];
		memset(freq, 0, 39*sizeof(int));

		int cacophony = 0, maxVowel = 0, maxConsonant = 0;
		for (i = 0; i < numSegs; i++) {
			for (int j = 0; j < segs[i]._numItems; j++) {
				freq[segs[i].set[j]._id]++;
			}
		}
		for (i = 0; i < 39; i++) {
			if (phonemes[i]._props & MASK_VOWEL) {
				if (freq[i] > maxVowel) maxVowel = freq[i];
			} else {
				if (freq[i] > maxConsonant) maxConsonant = freq[i];
			}
		}
		cacophony = ((maxConsonant > 2 || maxVowel > 3) ? 1 : 0);

		int repeats = 0;
		for (i = 0; i < numSegs - 1; i++) {
			repeats += (segs[i] == segs[i+1]) ? 1 : 0;
		}

		int middleGlottal = 0;
		for (i = 1; i < numSegs; i++) {
			middleGlottal += ((segs[i]._numItems == 1 && segs[i].first()._props & GLOTTAL) ? 1 : 0);
		}

		int complexity = 0;
		for (i = 0; i < numSegs; i++) {
			complexity += ((segs[i]._numItems >= 2) ? 1 : 0);
		}

		return (middleGlottal == 0) && (cacophony == 0) && (complexClusters < 3) && (complexity < 2) && (repeats == 0);
	}

	void render(char *buffer) {
		char *dst = buffer;

		int i;
		for (i = 0; i < numSegs; i++) {
			dst += sprintf(dst, "%s", segs[i]._spelling);
		}
	}

	void renderSegmented(char *buffer) {
		char *dst = buffer;

		int i;
		for (i = 0; i < numSegs-1; i++) {
			dst += sprintf(dst, "%s-", segs[i]._spelling);
		}
		sprintf(dst, "%s", segs[i]._spelling);
	}


	void render2(char *buffer) {

		char temp[100];
		char *dst = temp;

		int i;
		for (i = 0; i < numSegs; i++) {
			dst += sprintf(dst, "%s", segs[i]._spelling);
		}

		char temp2[100];
		type cl, cl2;
		int j = 0;

		i = 0;
		cl2 = getType(temp[i]);
		while (cl2 != kNone) {
			temp2[j++] = temp[i];
			i++;

			cl = getType(temp[i]);
			if (cl == 0) {
				temp2[j++] = '\0';
			} else
			if (cl != cl2) {
				temp2[j++] = '-';
			}
			cl2 = cl;
		}

		strcpy(buffer, temp2);
	}

};






RandSeed seed0(0);
RandSeed seed1(1);

EnglishOpenSyllableGenerator   openGen(seed0);
EnglishClosedSyllableGenerator closedGen(seed1);

bool generateWord() {

	Syllable syl[3];
	char buffer[100];
	bool r = false;

	openGen.genSyllable(syl[0]);
	closedGen.genSyllable(syl[1]);
//	openGen.genSyllable(syl[1]);
	//closedGen.genSyllable(syl[2]);

	Word word(syl, 2);
	word.render(buffer);

	if (word.validate()) {
		printf("%s\n", buffer);
		r = true;
	} else {
		printf("%s -> REJECTED\n", buffer);
	}

	return r;
}

int main(int argc, char *argv[]) {

	int len = 1;

	if (argc > 1) {
		len = atoi(argv[1]);
		if (len <= 0) {
			len = 1;
		}
	}

	int numRejected = 0;
	int numGenerated = 0;
	bool accepted;

	for (int i = 0; i < len; i++) {
		numGenerated++;
		accepted = generateWord();
		if (!accepted) numRejected++;
	}

//	printf("rejection ratio = %3.1f%%\n", 100.0f * numRejected / numGenerated);

	return 0;
}

