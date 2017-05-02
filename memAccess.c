struct cache_element {
	bool v;
	bool d;
	uint32_t tag;
}

struct cache_element icash[blockSize][offsetSize];
struct cache_element dcash[blockSize][offsetSize];


memAccess(bool icache, bool read, uint32_t address) {

	uint32_t tag = address >> (blockSize + offsetSize + 2);
	uint32_t blockIndex = (address >> (offsetSize + 2)) & (blockSize - 1));
	uint32_t blockOffset = (address >> 2) & (offsetSize - 1);
	uint32_t byteOffset = address & 0b11;

	struct cache_element cashElement;

	if (icache) {
		cashElement = icash[blockIndex][blockOffset];

		if (cashElement.tag != tag || cashElement.v != true) {
			clockCycles = clockCycles + 8;		// pay initial load penalty
			for (int i=0; i<offsetSize; i++) {	// load
				icash[blockIndex][i].tag = tag;
				icash[blockIndex][i].v = true;
				if (i<=blockOffset) {	// early start once the block desired is in cache
					clockCycles += 2;
				} else {
					clockCycles ++;
				}
			}
		}
	} else {
		cashElement = dcash[blockIndex][blockOffset];

		if (cashElement.tag != tag || cashElement.v != true) {
			if (cashElement.v == true && cashElement.d == true) {

				for (int i=0; i<offsetSize; i++) {	// loop through words to write back
					if (dcash[blockIndex][i].d == true) {
						clockCycles++;	// pay write-back penalty for dirty data only
					}
				}	
			}

			clockCycles = clockCycles + 8;		// pay initial load penalty
			for (int i=0; i<offsetSize; i++) {	// load
				dcash[blockIndex][i].tag = tag;
				dcash[blockIndex][i].v = true;
				dcash[blockIndex][i].d = false;
				if (i<=blockOffset) {	// early start once the block desired is in cache
					clockCycles += 2;
				} else {
					clockCycles++;
				}
			}
		}
		if (!read) {
			cashElement.d = true;	// if write mark data as dirty
		}
	}
}




/* This is old code
//////////////////// I-cache hit or miss logic
if (icache[blockIndex][blockOffset].tag == tag && icache[blockIndex][blockOffset].v == true) {
	clockCycles++;
} else {
	clockCycles = (clockCycles + 8) + (2*offsetSize);
	icache[blockIndex][blockOffset].tag = tag;
	icache[blockIndex][blockOffset].v == true;
}

//////////////////// D-cache hit or miss logic
// D-cache read
if (dcache[blockIndex][blockOffset].tag == tag && dcache[blockIndex][blockOffset].v == true) {
	clockCycles++;
} else {
	clockCycles = (clockCycles + 8) + (2*offsetSize);
	if (cache[blockIndex][blockOffset].d == true) {
		dcache[blockIndex][blockOffset].d == false;
		clockCycles = (clockCycles + 8) + (2*offsetSize);
	}
}

// D-cache write
if (dcache[blockIndex][blockOffset].tag == tag && dcache[blockIndex][blockOffset].d == false) {
	clockCycles++;
	dcache[blockIndex][blockOffset].d = true;
} else {
	if (dcache[blockIndex][blockOffset].d == true) {	
		clockCycles = (clockCycles + 8) + (2*blockOffset);
	} else {
	clockCycles
*/
