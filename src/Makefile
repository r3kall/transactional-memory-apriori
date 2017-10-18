CC  = g++
OMPFLAG = -fopenmp
TMFLAG = -fgnu-tm

OPTS	= -g -O3 -DBALT -std=c++11
CFLAGS  = $(OPTS) $(OMPFLAG) $(TMFLAG)
LIBS =  -lm

OBJS	= ListItemset.o
OBJS2   = Database.o Itemset.o HashTree.o

TARGET  = stm-apriori

default: $(TARGET)

clean:
	rm -rf *~ *.o ../$(TARGET)

stm-apriori: apriori.C $(OBJS) $(OBJS2) 
	$(CC) $(CFLAGS) \
	-o ../stm-apriori apriori.C $(OBJS) $(OBJS2) $(LIBS)

Database.o: Database.C Database.h
	$(CC) $(CFLAGS) -c Database.C

Itemset.o: Itemset.C Itemset.h 
	$(CC) $(CFLAGS) -c Itemset.C

ListItemset.o: ListItemset.C ListItemset.h
	$(CC) $(CFLAGS) -c ListItemset.C

HashTree.o: HashTree.C HashTree.h
	$(CC) $(CFLAGS) -c HashTree.C
