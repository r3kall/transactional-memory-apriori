#ifndef __HASHTREE_H
#define __HASHTREE_H
// #include <omp.h>
#include "ListItemset.h"
#define CCPD
#define YES 1 
#define NO 0 

class HashTree {
public:

  TM_SAFE
  HashTree(int, int, int);

  TM_SAFE
  ~HashTree();

  TM_SAFE
  void rehash(); // procedure for converting leaf node to a interior node
   
  TM_SAFE
  int hash(int); // procedure to find out which node item hashes to
   
  TM_SAFE
  int add_element(Itemset&);
  
  TM_SAFE 
  void clear();
  
  TM_SAFE 
  int is_root() { return (Depth == 0); };

  friend ostream& operator << (ostream&, HashTree&);

  inline int is_leaf() { return (Leaf == YES); }

  inline ListItemset* list() { return List_of_itemsets; }

  inline int hash_function() { return Hash_function; }

  inline int depth() { return Depth; }

  inline int hash_table_exists() { return (Hash_table != NULL); }
   
  inline HashTree* hash_table(int pos) { return Hash_table[pos]; }
   
private:
  int Leaf;
  HashTree** Hash_table;
  int Hash_function;
  int Depth;
  ListItemset* List_of_itemsets;
  int Count;
  int Threshold;

/* ----------------------------------------------------------------------------   
#if defined CCPD
   omp_lock_t nlck; //node lock
   omp_lock_t clck; //count lock
#endif
---------------------------------------------------------------------------- */
};

typedef HashTree* HashTree_Ptr;

TM_SAFE
HashTree** cdpHashTree(size_t);

#endif //__HASHTREE_H
