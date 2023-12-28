#include<stdlib.h>
#include<assert.h>
#include <string.h>

// DYNAMIC ARRAY
#define DA_INIT_CAPACITY 10

#define da_append(da, item)	\
	do{			\
		if((da)->count >= (da)->capacity) { \
			if((da)->capacity == 0) (da)->capacity = DA_INIT_CAPACITY; else  (da)->capacity = (da)->capacity*2; \
			(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
			assert(errno != ENOMEM);\
		}	\
		(da)->items[(da)->count++] = (item); \
	} while(0)		

#define da_append_many(da, new_items, new_items_count) \
	do{\
		if((da)->count+new_items_count > (da)->capacity){\
			if((da)->capacity == 0) (da)->capacity = DA_INIT_CAPACITY;\
			while((da)->count+new_items_count > (da)->capacity){\
				(da)->capacity *= 2;\
			}\
			(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
			assert((da)->items == NULL && "Not enough RAM"); \
		}\
		memcpy((da)->items+(da)->count, new_items, new_items_count*(sizeof(*(da)->items)));\
		(da)->count += new_items_count;\
	} while(0)

// da_foreach(const char*, arg, da)
#define da_foreach(type, item, da) for(type item=(da)->items; item<(da)->items+(da)->count;item+=(sizeof(da->(items)))

// STRING BUILDER
#define sb_append_cstr(sb, cstr)\
	do{\
		const char s* = (cstr);\
		da_append_many(sb, s, strlen(cstr));\
	} while(0)
#define sb_append_null(sb) da_append(sb, '\0')

typedef struct {
	char* items;
	int count;
	int capacity;
} StringBuilder;

