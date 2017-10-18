#ifndef __PARDHP_H
#define __PARDHP_H

#define TM

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct TRANSACTION {
  int tid;
  int numitem;
  int *item_list;
} transaction;

using namespace std;
extern transaction *data_set;
   
#define CACHE_LNSIZ 128 //128 bytes
   extern int nthreads;
   extern int *hash_indx;
   extern struct timeval tp;

#define seconds(tm) gettimeofday(&tp,(struct timezone *)0);\
   tm=tp.tv_sec+tp.tv_usec/1000000.0
      
#define min(a, b) ((a) < (b) ? (a) : (b))
#define mix(a, b) ((a) > (b) ? (a) : (b))
      
      class prof{
       public:
         double gen;
         double subset;
         double reduce;
         double large;
      };
#ifdef __cplusplus   
   }
#endif

#endif //__PARDHP_H
