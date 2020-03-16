#include "bitset.h"

int word_size = sizeof(uint64_t);

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
	struct bitset * bitset = calloc(1, sizeof(struct bitset));
	bitset->universe_size = size;
	bitset->size_in_words = (size + sizeof(uint64_t)-1)/sizeof(uint64_t);
	bitset->bits = calloc(bitset->size_in_words, sizeof(uint64_t));
	return bitset;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
	return this->universe_size;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this){
	int count = 0;
	for (int i=0; i<this->size_in_words; i++)
	{
		uint64_t number = this->bits[i];
		if(number&1)
		{
			count++;
			number >>= 1;
		}
	}
	return count;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item){
	int number_index = item/word_size;
	int bit_index_from_right = word_size - (item % word_size);
	uint64_t number = this->bits[number_index];
	number >>= bit_index_from_right;
	if (number & 1)
	{
		return 1;
	} else return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
void bitset_add(struct bitset * this, int item) {
	uint64_t temp_item = 0;
	uint64_t temp_word = 0;
	uint64_t word;
       
	word = this->bits[item/word_size];
	temp_item = item % word_size;
	temp_word = word;
	word = word >> temp_word;
	if(!(word & 1))
	{
		word = 1;
		//word = word << tempItem;
		word = word << temp_item;
		word = temp_word | word;
		this->bits[(int)item/word_size] = word;
	}
		
}
  
// remove an item with number 'item' from the set
void bitset_remove(struct bitset * this, int item) {
	int number_index = (item/word_size);
	item %= word_size;
	uint64_t tmp = ~(1 << item);
	this->bits[number_index] &= 1;
}
  
// place the union of src1 and src2 into dest;
// all of src1, src2, and dest must have the same size universe
void bitset_union(struct bitset * dest, struct bitset * src1,
		  struct bitset * src2) {
	for (int i=0; i<(dest->universe_size); i++)
		dest->bits[i] = src1->bits[i] | src2->bits[i];
}

// place the intersection of src1 and src2 into dest
// all of src1, src2, and dest must have the same size universe
void bitset_intersect(struct bitset * dest, struct bitset * src1,
		      struct bitset * src2) {
	for (int i=0; i<(dest->universe_size); i++)
		dest->bits[i] = src1->bits[i] & src2->bits[i];
}
