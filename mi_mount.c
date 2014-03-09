#define _FILE_OFFSET_BITS   64
#define FUSE_USE_VERSION 25
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <fuse_lowlevel.h>
#include <fuse_opt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options {
	char *device_name;
	char *mount_point;
	char *mount_options;
	int debug;
};

static struct options options;
static const char *EXEC_NAME = "tkfs";

static struct fuse_lowlevel_ops tkfs_ops = {
	.init           = NULL,
	.destroy        = NULL,
	.lookup         = NULL,
	.forget         = NULL,
	.getattr        = NULL,
	.setattr        = NULL,
	.readlink       = NULL,
	.mknod		= NULL,
	.mkdir          = NULL,
	.rmdir          = NULL,
	.symlink        = NULL,
	.rename         = NULL,
	.link           = NULL,
	.unlink         = NULL,
	.open           = NULL,
	.read           = NULL,
	.write          = NULL,
	.flush          = NULL,
	.release        = NULL,
	.fsync          = NULL,
	.opendir        = NULL,
	.readdir        = NULL,
	.releasedir     = NULL,
	.fsyncdir       = NULL,
	.statfs         = NULL,
	.setxattr       = NULL,
	.getxattr       = NULL,
	.listxattr      = NULL,
	.removexattr    = NULL,
	.access         = NULL,
	.create         = NULL,
};

static int try_fuse_mount(char *mount_options) {
	int fc = -1;
	struct fuse_args margs = FUSE_ARGS_INIT(0, NULL);
	
	fc = fuse_mount(options.mount_point, &margs);
	return fc;
}




/*
TO READ: /usr/share/doc/libfuse-dev/README.gz
 * Main function of FUSE.
 *
 * This is for the lazy.  This is all that has to be called from the
 * main() function.
 *
 * This function does the following:
 *   - parses command line options (-d -s and -h)
 *   - passes relevant mount options to the fuse_mount()
 *   - installs signal handlers for INT, HUP, TERM and PIPE
 *   - registers an exit handler to unmount the filesystem on program exit
 *   - creates a fuse handle
 *   - registers the operations
 *   - calls either the single-threaded or the multi-threaded event loop
*/
int main(int argc, char *argv[]) {

	return fuse_main(argc, argv, &tkfs_ops);

	struct fuse_args custom_args = FUSE_ARGS_INIT(argc, argv);
	int err = -1;
	int fd;
	int fc = -1;
	options.mount_point = "mmm";
	options.device_name = "mmm";
	if ((fd = fuse_mount(options.mount_point, &custom_args ))!=-1) {

printf("1 aqui \n");
		struct fuse_session *se=(struct fuse_session*)1;

printf("2 aqui \n");
		se = fuse_lowlevel_new(&custom_args, &tkfs_ops, sizeof(tkfs_ops), options.device_name);
printf("4 aqui \n");
			if (fuse_set_signal_handlers(se) != -1) {
printf("5 aqui \n");
				struct fuse_chan *ch = fuse_kern_chan_new(fd);
printf("6 aqui \n");
				if ((ch != NULL) && (se != NULL)) {
printf("7 aqui \n");
					fuse_session_add_chan(se, ch);
printf("8 aqui \n");
					err = fuse_session_loop(se);
printf("9 aqui \n");
				}
				fuse_remove_signal_handlers(se);
			}
			fuse_session_destroy(se);
		}
		close(fd);
	
	fuse_unmount("mmm");
	//fuse_unmount(options.mount_point);
	fuse_opt_free_args(&custom_args);

	return err ? 1 : 0;

}
