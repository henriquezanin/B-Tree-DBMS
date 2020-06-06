#ifndef __BTREE__
#define __BTREE__

#include <utils.h>

#define PAGESIZE 4096
#define TREE_HEADER (1*PAGESIZE)
#define MAXKEYS 200
#define AUX_FIELDS_SIZE_ON_PAGE (2+1) /*number of keys and "is leaf" bool*/
#define FREE_SPACE_ON_PAGE (PAGESIZE - ((MAXKEYS*4)+(MAXKEYS*8)+((MAXKEYS+1)*8)+3))

typedef struct record{
    int key;
    long recordRRN;
}record;

typedef struct page{
    record *records;
    long *childs;
    short numberOfKeys;
    BOOL isLeaf;
    long pageRRN;
} btPage;

typedef struct promotedkey{
    int key;
    long recordRRN;
    long childs[2];
} promotedKey;

btPage *getOrCreateRoot(FILE *);
btPage *getRoot(FILE *fp);
Errors bTreeInsert(PrimaryIndex *, btPage *, FILE *);
long bTreeSelect(btPage *, int, FILE *);
#endif