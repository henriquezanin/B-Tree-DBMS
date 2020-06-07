#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <btree.h>

/*Debug functions*/
void printPromotedKey(promotedKey *);
void printPageNode(btPage *);
/* End debug functions*/

btPage *readPageFromFile(FILE *fp){
    int i;
    btPage *node = (btPage*)malloc(sizeof(btPage));
    node->records = (record*)malloc(MAXKEYS*sizeof(record));
    node->childs = (long*)malloc((MAXKEYS+1)*sizeof(long));
    for(i = 0; i < MAXKEYS; i++)
        fread(&node->records[i].key, sizeof(int), 1, fp);
    for(i = 0; i < MAXKEYS; i++)
        fread(&node->records[i].recordRRN, sizeof(long), 1, fp);
    fread(node->childs, sizeof(long), MAXKEYS+1, fp);
    fread(&node->numberOfKeys, sizeof(short), 1, fp);
    fread(&node->isLeaf, sizeof(BOOL), 1, fp);
    return node;
}

Errors writePageIntoFile(long rrn, btPage *page, FILE *fp){
    if(!fp) return UNABLE_TO_WRITE_DATA;
    if(!page) return EMPTY_PAGE;
    if(rrn <= 0) return INVALID_RRN_PAGE;
    int i;
    unsigned char *freeSpace = (unsigned char*)calloc(FREE_SPACE_ON_PAGE, sizeof(unsigned char));
    fseek(fp, rrn * PAGESIZE, SEEK_SET);
    for(i = 0; i < MAXKEYS; i++)
        fwrite(&page->records[i].key, sizeof(int), 1, fp);
    for(i = 0; i < MAXKEYS; i++)
        fwrite(&page->records[i].recordRRN, sizeof(long), 1, fp);
    fwrite(page->childs, sizeof(long), MAXKEYS+1, fp);
    fwrite(&page->numberOfKeys, sizeof(short), 1, fp);
    fwrite(&page->isLeaf, sizeof(BOOL), 1, fp);
    fwrite(freeSpace, sizeof(unsigned char), FREE_SPACE_ON_PAGE, fp);
    fflush(fp);
    free(freeSpace);
    return SUCCESS;
}

btPage *getPage(long RRN, FILE *fp){
    if(!fp) return NULL;
    if(RRN <= 0) raiseError(INVALID_RRN_PAGE);
    fseek(fp,(RRN)*PAGESIZE,SEEK_SET);
    btPage *page = readPageFromFile(fp);
    page->pageRRN = RRN;
    return page;
}

long getTreeHeader(FILE *fp){
    long rootRRN;
    fseek(fp, 0l, SEEK_SET);
    fread(&rootRRN, sizeof(long), 1, fp);
    return rootRRN;
}

void writeTreeHeader(FILE *fp, long rootRRN){
    unsigned char *freeSpace = (unsigned char*)calloc(PAGESIZE-sizeof(long), sizeof(unsigned char));
    fseek(fp, 0l, SEEK_SET);
    fwrite(&rootRRN, sizeof(long), 1, fp);
    fwrite(freeSpace, sizeof(unsigned char), PAGESIZE-sizeof(long), fp);
    free(freeSpace);
    fflush(fp);
}

btPage *createTree(FILE *fp){
    int i;
    btPage *root = (btPage*)calloc(1, sizeof(btPage));
    root->records = (record*)calloc(MAXKEYS, sizeof(record));
    for(i = 0; i < MAXKEYS; i++)
        root->records[i].recordRRN = -1;
    root->childs = (long*)malloc((MAXKEYS+1)*sizeof(long));
    memset(root->childs, -1, MAXKEYS);
    root->isLeaf = TRUE;
    root->pageRRN = 1;
    writeTreeHeader(fp, 1);
    return root;
}

btPage *getOrCreateRoot(FILE *fp){
    if(!fp) return NULL;
    fseek(fp,0L,SEEK_END);
    long fileSize = ftell(fp);
    if(!fileSize) return createTree(fp);
    /*
    Get root page from B-tree header.
    There's 4088 bytes left for additional information
    */
    long rootRRN = getTreeHeader(fp);
    return getPage(rootRRN, fp);
}

btPage *getRoot(FILE *fp){
    if(!fp) return NULL;
    fseek(fp,0L,SEEK_END);
    long fileSize = ftell(fp);
    if(!fileSize) return NULL;
    long rootRRN = getTreeHeader(fp);
    return getPage(rootRRN, fp);
}

long getPageRrnFromFilePointer(FILE *fp){
    return ftell(fp)/PAGESIZE;
}

void freePage(btPage *page){
    free(page->records);
    free(page->childs);
    free(page);
}

btPage *searchPositionOnPageAndInsert(btPage *page, promotedKey *newKey){
    int pos;
    BOOL deallocatePage = FALSE;
    btPage *newPage = page;
    for(pos = 0;pos < page->numberOfKeys && page->records[pos].key < newKey->key; pos++);
    if(page->numberOfKeys == MAXKEYS){
        newPage = (btPage*)calloc(1,sizeof(btPage));
        newPage->records = (record*)calloc(MAXKEYS+1, sizeof(record));
        newPage->childs = (long*)calloc(MAXKEYS+2, sizeof(long));
        memcpy(newPage->records, page->records, pos*sizeof(record));
        /*A copia deve levar em consideracao o filho esquerdo e direito
        O +1 deve-se a esse fato*/
        memcpy(newPage->childs, page->childs,(pos+1)*sizeof(long));
        deallocatePage = TRUE;
    }
    memcpy(&newPage->records[pos+1], &page->records[pos], (page->numberOfKeys - pos)*sizeof(record));
    /* Verificar a integridade da copia dos filhos*/
    memcpy(&newPage->childs[pos+1],&page->childs[pos], (page->numberOfKeys - pos + 1)*sizeof(long));
    memcpy(&newPage->childs[pos], newKey->childs, 2*sizeof(long));
    newPage->records[pos].key = newKey->key;
    newPage->records[pos].recordRRN = newKey->recordRRN;
    newPage->numberOfKeys++;
    if(deallocatePage) freePage(page);
    return newPage;
}
/*
    If page size is odd the return is biggest slice
*/
btPage *splitAndCreateNewNode(btPage **page){
    int middle = (*page)->numberOfKeys/2;
    btPage *newPage = (btPage*)calloc(1,sizeof(btPage));
    newPage->records = (record*)calloc(MAXKEYS, sizeof(record));
    newPage->childs = (long*)calloc(MAXKEYS+1, sizeof(long));
    /*split records*/
    memcpy(newPage->records, &(*page)->records[middle], ((*page)->numberOfKeys-middle)*sizeof(record));
    memset(&(*page)->records[middle], 0, ((*page)->numberOfKeys-middle)*sizeof(record));
    /*split childs*/
    memcpy(newPage->childs, &(*page)->childs[middle], ((*page)->numberOfKeys-middle+1)*sizeof(long));
    /*set new size*/
    newPage->numberOfKeys = (*page)->numberOfKeys - middle;
    memset(&newPage->childs[newPage->numberOfKeys+1], -1, (MAXKEYS-newPage->numberOfKeys)*sizeof(long));
    (*page)->numberOfKeys = middle;
    memset(&(*page)->childs[(*page)->numberOfKeys+1], -1, (MAXKEYS-(*page)->numberOfKeys)*sizeof(long));
    newPage->isLeaf = (*page)->isLeaf;
    return newPage;
}

promotedKey *extractpromotedKey(btPage *page){
    promotedKey *node = (promotedKey*)calloc(1,sizeof(promotedKey));
    node->key = page->records[0].key;
    node->recordRRN = page->records[0].recordRRN;
    memcpy(page->records,&page->records[1],(page->numberOfKeys-1)*sizeof(record));
    memset(&page->records[page->numberOfKeys-1], 0, sizeof(record));
    memcpy(page->childs, &page->childs[1], page->numberOfKeys*sizeof(long));
    page->numberOfKeys--;
    return node;
}

promotedKey *_split(btPage *page,FILE *fp, promotedKey *newKey){
    if(!page){
        raiseError(EMPTY_PAGE);
        return NULL;
    }
    if(!fp){
        raiseError(INVALID_FILE_POINTER);
        return NULL;
    }
    btPage *newPage = splitAndCreateNewNode(&page);
    promotedKey *promoKey = extractpromotedKey(newPage);
    promoKey->childs[0] = page->pageRRN; 
    fseek(fp,0l,SEEK_END);
    promoKey->childs[1] = getPageRrnFromFilePointer(fp);
    writePageIntoFile(promoKey->childs[0],page,fp);
    writePageIntoFile(promoKey->childs[1],newPage,fp);
    return promoKey;
}

promotedKey *insertIntoNode(btPage *page, promotedKey *newKey, FILE *fp){
    page = searchPositionOnPageAndInsert(page,newKey);
    if(page->numberOfKeys == MAXKEYS)
        return _split(page,fp,newKey);
    writePageIntoFile(page->pageRRN,page,fp);
    return NULL;
}

btPage *createNodeWithPromotedKey(promotedKey *promoKey){
    btPage *newPage = (btPage*)calloc(1,sizeof(btPage));
    newPage->records = (record*)calloc(MAXKEYS, sizeof(record));
    newPage->childs = (long*)malloc((MAXKEYS+1)*sizeof(long));
    memset(newPage->childs, -1, sizeof(long)*(MAXKEYS+1));
    newPage->records[0].key = promoKey->key;
    newPage->records[0].recordRRN = promoKey->recordRRN;
    memcpy(newPage->childs, promoKey->childs, 2*sizeof(long));
    newPage->isLeaf = 0;
    newPage->numberOfKeys = 1;
    return newPage;
}

Errors setNodeAsRoot(btPage *page, FILE *fp){
    Errors err;
    fseek(fp, 0l, SEEK_END);
    long rrn = getPageRrnFromFilePointer(fp);
    err = writePageIntoFile(rrn, page, fp);
    writeTreeHeader(fp, rrn);
    return err;
}

promotedKey *_bTreeInsert(btPage *node, promotedKey *key, FILE *fp){
    promotedKey *promoKey = NULL;
    btPage *nextPage = NULL;
    int pos;
    if(node->isLeaf){
        promoKey = insertIntoNode(node, key, fp);
        return promoKey;
    }
    for(pos = 0; pos < node->numberOfKeys && node->records[pos].key < key->key; pos++);
    if(pos == node->numberOfKeys) pos--;
    if(key->key < node->records[pos].key)
        nextPage = getPage(node->childs[pos],fp);
    else
        nextPage = getPage(node->childs[pos+1],fp);
    promoKey = _bTreeInsert(nextPage, key,fp);
    if(promoKey)
       promoKey = insertIntoNode(node, promoKey, fp);
    freePage(nextPage);
    return promoKey;
}

Errors bTreeInsert(PrimaryIndex *newRecord, btPage *root, FILE *fp){
    Errors err = SUCCESS;
    promotedKey *key = (promotedKey*)malloc(sizeof(promotedKey));
    key->key = newRecord->key;
    key->recordRRN = newRecord->RRN;
    memset(key->childs, -1, 2*sizeof(long));
    promotedKey *promoKey = _bTreeInsert(root, key, fp);
    if(promoKey){
        btPage *newRoot = createNodeWithPromotedKey(promoKey);
        err = setNodeAsRoot(newRoot, fp);
    }
    return err;
}

long bTreeSelect(btPage *node, int key, FILE *fp){
    int pos;
    btPage *nextPage;
    for(pos = 0; pos < node->numberOfKeys && node->records[pos].key < key; pos++);
    if(pos == node->numberOfKeys) pos--;
    
    if(node->records[pos].key == key) return node->records[pos].recordRRN;
    if(node->isLeaf) return -1;

    if(key < node->records[pos].key)
        nextPage = getPage(node->childs[pos], fp);
    else
        nextPage = getPage(node->childs[pos+1], fp);
    return bTreeSelect(nextPage, key, fp);
}

void printPromotedKey(promotedKey *key){
    puts("CHAVE");
    printf("key: %d\n", key->key);
    printf("Record RRN: %ld\n", key->recordRRN);
    printf("Left Child: %ld\tRight Child: %ld\n\n", key->childs[0], key->childs[1]);
}

void printPageNode(btPage *page){
    int i;
    puts("NODE");
    printf("Number of keys: %d\n", page->numberOfKeys);
    printf("Is Leaf: %d\n", page->isLeaf);
    printf("Page RRN: %ld\n", page->pageRRN);
    puts("Records:[Key][Record RRN]");
    for(i=0;i<page->numberOfKeys;i++){
        printf("[%d][%ld]\t", page->records[i].key, page->records[i].recordRRN);
    }
    printf("\n");
    puts("Childs:[Page RRN]");
    for(i=0;i<page->numberOfKeys+1;i++){
        printf("[%ld] ", page->childs[i]);
    }
    printf("\n\n");
}