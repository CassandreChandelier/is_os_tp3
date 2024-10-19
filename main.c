#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "Ressoures FS-20241016/tosfs.h"

void exploreFileSystem() {
    int fileDescription = open("../Ressoures FS-20241016/test_tosfs_files",O_RDONLY);

    if (fileDescription == -1) {
        perror("Error opening file");
    }

    struct stat statbuf;
    if (fstat(fileDescription, &statbuf) == -1) {
        perror("Error opening file");
        exit(-1);
    }

    void *file_map = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fileDescription, 0);
    if (file_map == MAP_FAILED) {
        perror("Error memory mapping");
        close(fileDescription);
    }

    struct tosfs_superblock * superBlockPtr = file_map;
    printf("address: %p \n:", superBlockPtr);
    if(superBlockPtr->magic == TOSFS_MAGIC) {
        printf("the magic number matches!");
    }
    else{
        printf("the magic number doesn't match!");
    }

    printf("magic number: %X\n",superBlockPtr->magic);
    printf("block_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n",PRINTF_BYTE_TO_BINARY_INT32(superBlockPtr->block_bitmap));
    printf("inode_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n",PRINTF_BYTE_TO_BINARY_INT32(superBlockPtr->inode_bitmap));
    printf("block_size: %d\n",superBlockPtr->block_size);
    printf("blocks: %d\n",superBlockPtr->blocks);
    printf("inodes: %d\n",superBlockPtr->inodes);
    printf("root_inode: %d\n",superBlockPtr->root_inode);

    void * inodeTablePtr = (void*)(superBlockPtr) + superBlockPtr->block_size;

    printf("address: %p \n\n\n", inodeTablePtr);


    for(int i = 1; i < 4; i++) {
        struct tosfs_inode *inodePtr = inodeTablePtr + i * sizeof(struct tosfs_inode);
        printf("inode number %d: %d\n", i, inodePtr->inode);
        printf("inode block_no: %d\n", inodePtr->block_no);
        printf("inode uid: %d\n", inodePtr->uid);
        printf("inode gid: %d\n", inodePtr->gid);
        printf("inode mode: "PRINTF_BINARY_PATTERN_INT16"\n",PRINTF_BYTE_TO_BINARY_INT16(inodePtr->mode));
        printf("inode perm: "PRINTF_BINARY_PATTERN_INT16"\n",PRINTF_BYTE_TO_BINARY_INT16(inodePtr->perm));
        printf("inode size: %d\n", inodePtr->size);
        printf("inode nlink: %d\n\n", inodePtr->nlink);
    }

    struct tosfs_dentry *dentryPtr = inodeTablePtr+ TOSFS_BLOCK_SIZE;
    for (int i = 0; i<4; i++) {
        printf("dentry inode: %d\n",dentryPtr->inode);
        printf("dentry name: %s\n\n", dentryPtr->name);
        dentryPtr++;
    }

    char * oneFilePtr = inodeTablePtr + 2 * TOSFS_BLOCK_SIZE;
    for (int i = 0; i < 14; i++) {
        if(*oneFilePtr + i == '\n') {
            printf("attention retour à la ligne");
        }
        printf("%c", *oneFilePtr + i);
    }
    printf("\n");

    char * twoFilePtr = inodeTablePtr + 3 * TOSFS_BLOCK_SIZE;
    for (int i = 0; i < 32; i++) {
        if(*twoFilePtr + i == '\n') {
            printf("attention retour à la ligne");
        }
        printf("%c", *twoFilePtr + i);
    }
    printf("\n");
}



int main(void)
{
    exploreFileSystem();
    return 0;
}

