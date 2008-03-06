#ifndef __MISC__
#define __MISC__

typedef unsigned int uint32;

class Seed {
public:
	virtual uint32 getBits(uint32 num) = 0;
};

class NullSeed : public Seed {

public:
	NullSeed() { }
	uint32 getBits(uint32 num) {
		return 0;
	}
};

class RandSeed : public Seed {

	uint32 _seed;

public:
	RandSeed(uint32 seed) : _seed(seed) {
	}
	uint32 getBits(uint32 num) {
		_seed = 0xDEADBF03 * (_seed + 1);
		_seed = (_seed >> 13) | (_seed << 19);
		return _seed % num;
	}
};

class SeededGenerator {
protected:
	Seed &_seed;
public:
	SeededGenerator(Seed &seed) : _seed(seed) { }
	virtual ~SeededGenerator() { }
};

template <class T>
class Distribution {

	std::vector<T>		_items;
	std::vector<int> 	_cumFreqs;

	int			_numItems;
	int			_cumFreq;

public:
	Distribution() : _numItems(0), _cumFreq(0) { }

	void addItem(int frequency, const T& item) {
		addItem(item, frequency);
	}

	void addItem(const T& item, int frequency) {
		if (frequency == 0)
			return;

		_cumFreq += frequency;

		_items.push_back(item);
		_cumFreqs.push_back(_cumFreq);

		_numItems++;
	}

	T getItem(int value) const {

		int i;
		for (i = 0; i < _numItems; i++)
			if (value <= _cumFreqs[i])
				break;

		return _items[i];
	}

	int size() {
		return _numItems;
	}

	int cumFreq() {
		return _cumFreq;
	}

};

#endif
