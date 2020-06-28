#ifndef _lcthw_Hashmap_h
#define _lcthw_Hashmap_h

#include <stdint.h>
#include <lcthw/darray.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare) (void *a, void *b);
typedef uint32_t(*Hashmap_hash) (void *key);

typedef struct Hashmap {
	// dynamic array that will be set to a fixed size
	// each bucket will contain a DArray that holds HashmapNode pairs
	DArray *buckets;
	// find elements by their key
	Hashmap_compare compare;
	// hashing function, taking a key, process contents
	// produce a single uint32_t index number
	Hashmap_hash hash;
} Hashmap;

typedef struct HashmapNode {
	// key value pair
	void *key;
	void *data;
	// calculated hash
	uint32_t hash;
} HashmapNode;

typedef int(*Hashmap_traverse_cb)(HashmapNode *node);

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash);
void Hashmap_destroy(Hashmap *map);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap *map, void *key);

#endif
