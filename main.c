#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "Ressoures FS-20241016/tosfs.h"

int main(void)
{

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

    printf("address: %p \n", inodeTablePtr);


    struct tosfs_inode * inodePtr = inodeTablePtr;
    for(int i = 0; i < 8; i++) {
        printf("inode number %d: %d\n", i, inodePtr->inode);
        printf("inode size: %d\n\n", inodePtr->size);
        inodePtr++;
    }



    return 0;
}
