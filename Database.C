#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <strings.h>
#include <omp.h>

#include "Database.h"

int DBASE_NUM_TRANS;
int DBASE_MAXITEM;
int DBASE_AVG_TRANS_SZ;
int DBASE_BUFFER_SIZE = 8192;

extern double *t_io;

void init_DCB(Dbase_Ctrl_Blk &DCB, int infile)
{
   DCB.fd = infile;
   DCB.buf_size = DBASE_BUFFER_SIZE;
   DCB.buf = new int [DCB.buf_size];
   DCB.cur_buf_pos = 0;
   DCB.cur_blk_size = 0;
}

void close_DCB(Dbase_Ctrl_Blk &DCB)
{
   delete [] DCB.buf;
}

void reset_database(Dbase_Ctrl_Blk& DCB)
{
   lseek(DCB.fd, 3*ITSZ, SEEK_SET);
   DCB.cur_buf_pos = 0;
   DCB.cur_blk_size = 0;
}

void get_next_trans_ext(Dbase_Ctrl_Blk& DCB, int &numitem, int &tid)
{
   // Need to get more items from file
   int res = DCB.cur_blk_size - DCB.cur_buf_pos+2;
   if (res > 0)
   {
      // First copy partial transaction to beginning of buffer
      //DCB.cur_blk_size -= DCB.cur_buf_pos;
      memcpy(DCB.buf,
              (char *)(DCB.buf + DCB.cur_buf_pos - 2),
              res * ITSZ);
      DCB.cur_blk_size = res;
   }
   else
   {
      // No partial transaction in buffer
      DCB.cur_blk_size = 0;
   }

   DCB.cur_blk_size +=
      (read(DCB.fd,
            (char *)(DCB.buf + DCB.cur_blk_size),
            ((DCB.buf_size - DCB.cur_blk_size)*ITSZ)))/ITSZ;
   
   if (DCB.cur_blk_size > 0)
   {
      tid = DCB.buf[0];
      numitem = DCB.buf[1];
      DCB.cur_buf_pos = 2;   
   }   
}

int Database_readfrom(char *infile, int * &items, int * & offsets, int pid, int file_offset, int data_size)
{
   int i,j,k,m;
   
   int num_trans;
   int maxitem;
   int avg_trans_sz;
   int max_trans_sz = 1;
   int tid, numitem;
   Dbase_Ctrl_Blk DCB;
   int *buf, *buffer;
   FILE *Dbase_file; 
   int idx, lb,ub,blk, offt = 0;
   double t_io_1, t_io_2; 

   seconds(t_io_1); 
   Dbase_file = fopen(infile,"r");
   if (Dbase_file < 0){
      printf("Error couldn't open file %s\n", infile);
      exit(-1);
   }
   
   fread(&num_trans,sizeof(int), 1, Dbase_file);
   fread(&maxitem,sizeof(int), 1, Dbase_file);
   fread(&avg_trans_sz,sizeof(int), 1, Dbase_file);
   if(pid==0) {
     DBASE_NUM_TRANS = num_trans;
     DBASE_MAXITEM = maxitem;
     DBASE_AVG_TRANS_SZ = avg_trans_sz;
     MINSUPPORT = (int)(MINSUP_PER*DBASE_NUM_TRANS+0.5);
      //ensure that support is at least 2
     if (MINSUPPORT < 2) MINSUPPORT = 2;

     data_set = (transaction *) calloc (num_trans, sizeof(transaction));
   }
  
   #pragma omp barrier
   offsets = new int [maxitem];
   for (i=maxitem-1; i >= 0; i--){
      offsets[maxitem-i-1] = offt;
      offt += i;
   }
   items = new int [(maxitem * (maxitem - 1))/2];
   for (i=0; i<(maxitem * (maxitem - 1))/2; i++)
       items[i]=0;
  
   blk = (num_trans+nthreads - 1)/nthreads;
   lb = pid*blk;
   ub = min((pid+1)*blk, num_trans);
   
   buffer = (int *) malloc(sizeof(int)*data_size); 
   fseek(Dbase_file, file_offset*sizeof(int), SEEK_CUR); 
   fread(buffer, sizeof(int), data_size, Dbase_file);

   seconds(t_io_2);
   t_io[pid]=t_io_2 - t_io_1;

/* input file format is in binary
 * First  4-byte integer is the number of total transactions
 * Second 4-byte integer is the number of unique items
 * Third  4-byte integer is the average size of transactions
 * The rest are transactions (one transaction appended after another)
 * Each transaction contains
 *     a transaction ID (a 4-byte integer),
 *     the number of items (a 4-byte integer),
 *     the list of item IDs (each is a 4-byte integer)
 */
   fread(&num_trans,sizeof(int), 1, Dbase_file);
   fread(&maxitem,sizeof(int), 1, Dbase_file);
   fread(&avg_trans_sz,sizeof(int), 1, Dbase_file);
   m=0; 
         
   for (i=lb; i < ub; i++){
     data_set[i].tid = buffer[m++];
     data_set[i].numitem = buffer[m++];

     if (data_set[i].numitem > max_trans_sz) max_trans_sz =  data_set[i].numitem;

    data_set[i].item_list = (int *) calloc (data_set[i].numitem, sizeof(int));
     for (j=0; j < data_set[i].numitem; j++)
       data_set[i].item_list[j] = buffer[m++];
     
     for (j=0; j < data_set[i].numitem -1; j++){
       idx = offsets[data_set[i].item_list[j]]-data_set[i].item_list[j]-1;
       for (k=j+1; k < data_set[i].numitem; k++){
         items[idx+data_set[i].item_list[k]]++;
       }
     }
   }

   free(buffer);
   return max_trans_sz;
}








