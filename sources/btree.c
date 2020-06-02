#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <btree.h>

/*Debug functions*/
void printPromotedKey(promotedNode *);
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
    /*printf("freespace: %d\n", FREE_SPACE_ON_PAGE);*/
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

btPage *getRoot(FILE *fp){
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

long getPageRrnFromFilePointer(FILE *fp){
    return ftell(fp)/PAGESIZE;
}

void freePage(btPage *page){
    free(page->records);
    free(page->childs);
    free(page);
}

/*TESTAR MASSIVAMENTE ESSA FUNCAO*/
btPage *searchPositionOnPageAndInsert(btPage *page, promotedNode *newKey){
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
    /*puts("SearchPositionOnPageAndInsert com numero de chaves < que MAXKEYS");
    printPageNode(page);*/
    memcpy(&newPage->records[pos+1], &page->records[pos], (page->numberOfKeys - pos)*sizeof(record));
    /* Verificar a integridade da copia dos filhos*/
    memcpy(&newPage->childs[pos+1],&page->childs[pos], (page->numberOfKeys - pos + 1)*sizeof(long));
    memcpy(&newPage->childs[pos], newKey->childs, 2*sizeof(long));
    newPage->records[pos].key = newKey->key;
    newPage->records[pos].recordRRN = newKey->recordRRN;
    newPage->numberOfKeys++;
    if(deallocatePage) freePage(page);
    /*puts("SearchPositionOnPageAndInsert finalizada");
    printPageNode(newPage);*/
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

promotedNode *extractPromotedNode(btPage *page){
    promotedNode *node = (promotedNode*)calloc(1,sizeof(promotedNode));
    node->key = page->records[0].key;
    node->recordRRN = page->records[0].recordRRN;
    memcpy(page->records,&page->records[1],(page->numberOfKeys-1)*sizeof(record));
    memset(&page->records[page->numberOfKeys-1], 0, sizeof(record));
    memcpy(page->childs, &page->childs[1], page->numberOfKeys*sizeof(long));
    page->numberOfKeys--;
    return node;
}

promotedNode *_split(btPage *page,FILE *fp, promotedNode *newKey){
    if(!page){
        raiseError(EMPTY_PAGE);
        return NULL;
    }
    if(!fp){
        raiseError(INVALID_FILE_POINTER);
        return NULL;
    }
    /*page = searchPositionOnPageAndInsert(page, newKey);*/
    btPage *newPage = splitAndCreateNewNode(&page);
    promotedNode *promoNode = extractPromotedNode(newPage);
    promoNode->childs[0] = page->pageRRN; 
    fseek(fp,0l,SEEK_END);
    promoNode->childs[1] = getPageRrnFromFilePointer(fp);
    writePageIntoFile(promoNode->childs[0],page,fp);
    writePageIntoFile(promoNode->childs[1],newPage,fp);
    return promoNode;
}

promotedNode *insertIntoNode(btPage *page, promotedNode *newKey, FILE *fp){
    /*puts("InsertIntoNode");*/
    page = searchPositionOnPageAndInsert(page,newKey);
    if(page->numberOfKeys == MAXKEYS)
        return _split(page,fp,newKey);
    writePageIntoFile(page->pageRRN,page,fp);
    return NULL;
}

btPage *createNodeWithPromotedKey(promotedNode *promoKey){
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

promotedNode *_bTreeInsert(btPage *node, promotedNode *key, FILE *fp){
    promotedNode *promoKey = NULL;
    btPage *nextPage = NULL;
    int pos;
    /*Como setar se o nó é uma folha????*/
    /*printPromotedKey(key);
    printPageNode(node);*/
    if(node->isLeaf){
        /*printf("Is leaf recursivo\n");*/
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
    /*freePage(nextPage);*/
    return promoKey;
}

Errors bTreeInsert(PrimaryIndex *newRecord, btPage *root, FILE *fp){
    Errors err = SUCCESS;
    promotedNode *key = (promotedNode*)malloc(sizeof(promotedNode));
    key->key = newRecord->key;
    key->recordRRN = newRecord->RRN;
    memset(key->childs, -1, 2*sizeof(long));
    /*printf("Chave a ser inserida:\n");*/
    /*printPromotedKey(key);*/
    promotedNode *promoKey = _bTreeInsert(root, key, fp);
    if(promoKey){
        /*printPromotedKey(promoKey);*/
        btPage *newRoot = createNodeWithPromotedKey(promoKey);
        err = setNodeAsRoot(newRoot, fp);
    }
    return err;
}

long bTreeSelect(btPage *node, int key, FILE *fp){
    int pos;
    /*printPageNode(node);*/
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

void printPromotedKey(promotedNode *key){
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