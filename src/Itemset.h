#ifndef __ITEMSET_H
#define __ITEMSET_H
#include <malloc.h>
#include <iostream>
using namespace std;

#if defined CCPD 
#include "llsc.h"
#endif

#include "pardhp.h"
#include "tm.h"

#define DEFAULT -1

class Itemset {
public:
   
  TM_SAFE
  Itemset (int = 1);

  TM_SAFE
  ~Itemset();

  TM_SAFE
  void clear();

  TM_SAFE
  void copy(Itemset*);

  int subsequence(Itemset&, int*);
  int subsequence(Itemset&);
  int subsequence(char*, int);
  int compare(Itemset&);
  int compare(Itemset&, int);
 
  friend ostream& operator << (ostream&, Itemset&);

  TM_SAFE
  inline int item(int pos) { return theItemset[pos]; }

  TM_SAFE
  inline int numitems(){ return theNumel; }

  inline void add_item(int pos, int val) { theItemset[pos] = val; }  
   
  inline void set_numitems(int val) { theNumel = val; }
   
  inline int maxsize() { return theSize; }
  
  inline int sup() { return support; }
  
  inline void incr_sup() { support++; }
  
  inline void set_sup(int val) { support = val; }
  
  inline int tid() { return Tid; }
  
  inline void set_tid(int val) { Tid = val; }
   
private:
  int* theItemset;
  unsigned int theNumel;
  unsigned int theSize;
  int support;
  int Tid;
};

#endif //__ITEMSET_H
