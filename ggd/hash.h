#ifndef _HASH_H
#define _HASH_H
#include <glib.h>

void initHashMap(GHashTable **table);
bool hashcheck(GHashTable *table,char* str);
unsigned long calc(char* str);

#endif
