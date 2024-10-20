
#include "/usr/include/fuse/fuse_common.h"

#include <utime.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/uio.h>

/** Inode number type */
typedef unsigned long fuse_ino_t;

/** Request pointer type */
typedef struct fuse_req *fuse_req_t;



struct fuse_lowlevel_ops2 {

    void (*getattr) (fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);

    void (*readdir) (fuse_req_t req, fuse_ino_t ino, size_t size, struct fuse_file_info *fi);

    void (*lookup) (fuse_req_t req, fuse_ino_t parent, const char *name);

    void (*read) (fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);

    void (*create) (fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi);

};