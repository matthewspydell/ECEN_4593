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
		if (icash[blockIndex][blockOffset].tag != tag || icash[blockIndex][blockOffset].v != true) {
			clockCycles = (clockCycles + 8) + (2*blockSize);	// pay load penalty
			icash[blockIndex][blockOffset].tag = tag;
			icash[blockIndex][blockOffset].v = true;
		}
	} else {
		if (dcash[blockIndex][blockOffset].tag != tag || dcash[blockIndex][blockOffset].v != true) {
			if (dcash[blockIndex][blockOffset].v == true && dcash[blockIndex][blockOffset].d == true) {
				// pay write-back penalty
			}
		}
		clockCycles = (clockCycles + 8) + (2*blockSize);	// pay load penalty
		dcash[blockIndex][blockOffset].tag = tag;
		dcash[blockIndex][blockOffset].v = true;
		dcash[blockIndex][blockOffset].d = false;
		if(!read) {
			dcash[blockIndex][blockOffset].d = true;	// if write mark data as dirty
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
