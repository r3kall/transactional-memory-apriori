#include <stddef.h>
#include "Itemset.h"


TM_SAFE
Itemset::Itemset (int sz) {
  theNumel = 0;
  if (sz <= 0) sz = 1;
  theSize = sz;
  // theItemset = new int[theSize];
  theItemset = (int*) calloc(theSize, sizeof(int));
  for(int i=0; i < theSize; i++) theItemset[i] = DEFAULT;
  support =0;
  Tid = -1;
}


TM_SAFE
Itemset::~Itemset() { 
  clear();
}


TM_SAFE
void Itemset::copy(Itemset* it) {
  theNumel = it->theNumel;
  for (int i=0;i < theNumel; i++)
    theItemset[i] = it->theItemset[i];
  support = it->support;
}


int Itemset::subsequence(Itemset& ar) {
  int i, j;

  if (theNumel > ar.theNumel) return 0;
  int start = 0;
  for(i=0; i < theNumel; i++){
    for(j=start; j < ar.theNumel; j++)
      if (theItemset[i] == ar.theItemset[j]){
        start = j+1;
        break;
      }
    if (j >= ar.theNumel) return 0;
  }
  return 1;
}


int Itemset::subsequence(char* indx, int len) {
  if (theNumel > len) return 0;
  for (int i=0; i < theNumel; i++)
    if (indx[theItemset[i]] != 1) return 0;
  return 1;
}


int Itemset::subsequence(Itemset& ar, int* pos_arr) {
  int i, j;

  if (theNumel > ar.theNumel) return 0;
  int start = 0;
  for(i=0; i < theNumel; i++){
    for(j=start; j < ar.theNumel; j++)
      if (theItemset[i] == ar.theItemset[j]){
        pos_arr[i] = j;
        start = j+1;
        break;
      }
    if (j >= ar.theNumel) return 0;
  }
  return 1;
}


int Itemset::compare(Itemset& ar2) {
  int len = (theNumel > ar2.theNumel) ? ar2.theNumel:theNumel;
  for(int i=0; i < len; i++){
    if (theItemset[i] > ar2.theItemset[i]) return 1;
    else if (theItemset[i] < ar2.theItemset[i]) return -1;
  }
  if (theNumel > ar2.theNumel) return 1;
  else if (theNumel < ar2.theNumel) return -1;
  else return 0;
}


//len must be less than length of both Itemsets
int Itemset::compare(Itemset& ar2, int len) {
  for(int i=0; i < len; i++){
    if (theItemset[i] > ar2.theItemset[i]) return 1;
    else if (theItemset[i] < ar2.theItemset[i]) return -1;
  }
  return 0;
}


TM_SAFE
void Itemset::clear() {
  delete [] theItemset;
  theNumel = 0;
  theSize = 0;
  support = 0;
}


ostream& operator << (ostream& outputStream, Itemset& arr) {
  outputStream << "ITEM: ";
  for (int i=0; i < arr.theNumel; i++)
    outputStream << i << ":" << arr.theItemset[i] << " ";
  outputStream << " N:" << arr.theNumel << "(" << arr.theSize << ")"
               << " S:" << arr.support
               << " T:" << arr.Tid << "\n";
  return outputStream;
}
