#include <stdio.h>
#include <climits>
#include <glib.h>
#include "hash.h"

//GHash_Table *table;
//bool *hashMap;

void initHashMap(GHashTable **table)
{
  //table=g_hash_table_new(g_int_hash,g_int_equal);
  *table=g_hash_table_new(g_str_hash,g_str_equal);
  //hashMap = new bool[USHRT_MAX];
  //for(int i = 0; i < USHRT_MAX; i++)
  //  hashMap[i] = false;
}

bool hashcheck(GHashTable *table,char* str){
  //unsigned short key = calc(str);
  gpointer r;
  r=g_hash_table_lookup(table, str);
  //r=g_hash_table_lookup(table, GINT_TO_POINTER(key));
  if(r != NULL){
    //printf("hash %d match : %s", key, str);
    return false;
  }
  else{
    g_hash_table_insert(table, str, str);
    //g_hash_table_insert(table, GINT_TO_POINTER(key), str);
    //printf("hash %d reseverd : %s", key, str);
    return true;
  }
}

unsigned long calc(char* str)
{
  unsigned long hv = 0;
  while(*str != '\n'){
    hv = ((hv << 5) - hv) + (unsigned char)*str++;
  }
  return hv;
}

