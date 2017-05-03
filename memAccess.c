/*
 * @author Matthew Spydell
 * MIPS Cache Access
 */

#include "main.h"

void memAccess(bool icache, bool read, uint32_t address) {

	uint32_t tag = address >> (CACHESIZE + BLOCKSIZE + 2);
	uint32_t blockIndex = (address >> (BLOCKSIZE + 2)) & (CACHESIZE - 1);
	uint32_t blockOffset = (address >> 2) & (BLOCKSIZE - 1);
	//uint32_t byteOffset = address & 0b11; unnecessary for my implementation of cache

	struct cache_element cashElement;

	if (icache) {
		cashElement = icash[blockIndex][blockOffset];

		if (cashElement.tag != tag || cashElement.v != true) {
			clockCycles = clockCycles + 8;		// pay initial load penalty
			for (int i=0; i<BLOCKSIZE; i++) {	// load
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

				for (int i=0; i<BLOCKSIZE; i++) {	// loop through words to write back
					if (dcash[blockIndex][i].d == true) {
						clockCycles++;	// pay write-back penalty for dirty data only
					}
				}	
			}

			clockCycles = clockCycles + 8;		// pay initial load penalty
			for (int i=0; i<BLOCKSIZE; i++) {	// load
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

