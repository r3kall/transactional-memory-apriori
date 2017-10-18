#ifndef __DATABASE_H
#define __DATABASE_H

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "ListItemset.h"
#include "HashTree.h"
#include "pardhp.h"


#define ITSZ sizeof(int)

struct Dbase_Ctrl_Blk 
{
   int fd;     
   int buf_size;
   int * buf;
   int cur_blk_size; 
   int cur_buf_pos;  
};

extern int *Dbase_file;
int Database_readfrom(char *, int *&, int *&, int, int,int);
void reset_database(Dbase_Ctrl_Blk&);
void get_next_trans_ext(Dbase_Ctrl_Blk& DCB, int &numitem, int &tid);
void close_DCB(Dbase_Ctrl_Blk &DCB);
void init_DCB(Dbase_Ctrl_Blk &DCB, int infile);

inline void get_first_blk(Dbase_Ctrl_Blk& DCB)
{
   DCB.cur_blk_size = (read(DCB.fd,(char *)DCB.buf,
                            (DCB.buf_size*ITSZ)))/ITSZ;
   DCB.cur_buf_pos = 2;
}

inline void get_next_trans(Dbase_Ctrl_Blk& DCB, int * &buf, int &numitem, int &tid)
{
   numitem = DCB.buf[DCB.cur_buf_pos-1];
   tid = DCB.buf[DCB.cur_buf_pos-2];
   if ((DCB.cur_buf_pos + numitem + 2) > DCB.cur_blk_size)
   {
      // Need to get more items from file
      get_next_trans_ext(DCB, numitem, tid);
   }
   buf = DCB.buf + DCB.cur_buf_pos;

   DCB.cur_buf_pos += numitem + 2;
}

inline void make_Itemset(Itemset *it, int *buf, int numitem, int tid)
{
   int j;
   
   it->set_tid(tid);
   it->set_numitems(numitem);
   for (j=0; j < numitem; j++){
      it->add_item(j, (int) buf[j]);
   }
}


extern int DBASE_NUM_TRANS;
extern int DBASE_MAXITEM;
extern int DBASE_AVG_TRANS_SZ;
extern int MINSUPPORT;
extern int DBASE_BUFFER_SIZE;
extern double MINSUP_PER;

#endif //__DATABASE_H
