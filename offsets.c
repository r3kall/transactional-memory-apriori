#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int i, j, err, nprocs, num_trans, maxitem, avg_trans_sz, numitem;
    int  *count, *buf, *buf_ptr;
    long fsize;
    FILE *fptr;
    FILE *optr;

    if (argc != 3) {
        printf("Usage: %s filename nprocs\n",argv[0]);
        exit(1);
    }
    nprocs = atoi(argv[2]);
    fptr = fopen(argv[1], "r");
    if (fptr < 0){
        printf("Error: open file %s (%d)\n", argv[1],strerror(errno));
        exit(-1);
    }
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
    fread(&num_trans,    sizeof(int), 1, fptr);
    fread(&maxitem,      sizeof(int), 1, fptr);
    fread(&avg_trans_sz, sizeof(int), 1, fptr);

    fseek(fptr, 0, SEEK_END);
    fsize = ftell(fptr);
    fsize -= 3 * sizeof(int);  /* header of 3 int */
    buf = (int*) malloc(fsize);

    fseek(fptr, 3 * sizeof(int), SEEK_SET);
    fread(buf, 1, fsize, fptr);
    fclose(fptr);

    count = (int*) calloc(nprocs, sizeof(int));
    for (i=0; i<nprocs; i++) {
        count[i] = num_trans / nprocs;
        if (i < num_trans % nprocs)
            count[i]++;
    }

    char* outfile = (char*) malloc(32);
    strcpy(outfile, "offset_P");
    strcat(outfile, argv[2]);
    strcat(outfile, ".txt");

    if ((optr = fopen (outfile, "w")) == NULL){
      printf("can't open %s\n", outfile);
      exit(-1);
   }

    fprintf(optr, "0\n");
    buf_ptr = buf;
    for (i=0; i<nprocs; i++) {
        for (j=0; j<count[i]; j++) {
            buf_ptr++;   /* transaction ID */
            numitem = *buf_ptr;  /* number of items */
            buf_ptr += numitem + 1;
        }
        fprintf(optr, "%d\n", buf_ptr-buf);
    }

    free (buf);
    free (count);

    fclose(optr);

    return 0;
}

