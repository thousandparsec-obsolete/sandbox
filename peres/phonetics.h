#ifndef __PHONETICS__
#define __PHONETICS__


#include <assert.h>
#include "tactics.h"

enum {
	FRICATIVE		= 1,
	PLOSIVE			= 2,
	AFFRICATE		= 4,
	NASAL			= 8,
	APPROXIMANT		= 0x10,
	LATERAL			= 0x20,
	MASK_MANNER		= 0x3f,

	BILABIAL		= 0x40,
	LABIODENTAL		= 0x80,
	DENTAL			= 0x100,
	GLOTTAL			= 0x200,
	PALATAL			= 0x400,
	ALVEOLAR		= 0x800,
	POSTALVEOLAR 	= 0x1000,
	VELAR			= 0x2000,
	LABIOVELAR		= 0x4000,
	MASK_PLACE		= 0x7fc0,

	VOICED			= 0x10000,
	VOICELESS   	= 0x20000,
	MASK_VOICE		= 0x30000,

	SHORT_VOWEL		= 0x100000,
	LONG_VOWEL 		= 0x200000,
	MASK_VOWEL		= 0x300000

};

struct Phoneme {
	int				_id;
	unsigned int 	_props;

	Phoneme() : _id(0), _props(0) { }
	Phoneme(int id, unsigned int props) : _id(id), _props(props) { }

	bool hasProps(unsigned int props) const {
		return (_props & props) == props;
	}

	bool operator==(const Phoneme &p) const {
		return _id == p._id;
	}

	bool operator!=(const Phoneme &p) const {
		return _id != p._id;
	}
};

#define MAX_PHONEMES_PER_SEGMENT	3

struct Segment {
	Phoneme set[MAX_PHONEMES_PER_SEGMENT];
	int _numItems;
	const char *_spelling;

	Segment() : _numItems(0) {
		_spelling = "";
		set[0]._id = 0;
	}

	Segment(const char spelling[], const Phoneme &p0) {
		_numItems = 1;
		set[0] = p0;
		_spelling = spelling;
	}

	Segment(const char spelling[], Phoneme p0, Phoneme p1) {
		_numItems = 2;
		set[0] = p0;
		set[1] = p1;
		_spelling = spelling;
	}

	Segment(const char spelling[], Phoneme p0, Phoneme p1, Phoneme p2) {
		_numItems = 3;
		set[0] = p0;
		set[1] = p1;
		set[2] = p2;
		_spelling = spelling;
	}

	bool operator==(const Segment &s) const {
		if (s._numItems != _numItems)
			return false;

		for (int i = 0; i < _numItems; i++) {
			if (s.set[i] != set[i])
				return false;
		}

		return true;
	}

	Segment& operator=(const Segment& seg) {
		_numItems = seg._numItems;
		set[0] = seg.set[0];
		set[1] = seg.set[1];
		set[2] = seg.set[2];
		_spelling = seg._spelling;
		return *this;
	}

	Phoneme item(int index) const {
		if (_numItems == 0) {
			return set[0];
		}

		assert(index >= 0 && index < _numItems);
		return set[index];
	}

	Phoneme first() const {
//		assert(_numItems > 0);
		return item(0);
	}

	Phoneme last() const {
//		assert(_numItems > 0);
		return item(_numItems - 1);
	}

	bool isShortVowel() const {
		if (_numItems > 1)
			return false;

		return set[0].hasProps( SHORT_VOWEL );
	}

	bool isComplexCluster() const {
		return (_numItems > 1);
	}
};

struct Syllable {
	Segment onset;
	Segment nucleus;
	Segment coda;


	bool hasCoda() const {
		return coda._numItems > 0;
	}
	bool hasOnset() const {
		return onset._numItems > 0;
	}
	int numSegments() const {
		return 1 + (hasCoda() ? 1 : 0) + (hasOnset() ? 1 : 0);
	}
};

#endif
