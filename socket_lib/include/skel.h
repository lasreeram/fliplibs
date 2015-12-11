#ifndef __SKEL_H__
#define __SKEL_H__
#include <unistd.h>

/* Unix version */
typedef int SOCKET;
#define INIT() ( sockets_lib::program_name = \
			strrchr( argv[0], '/' ) ) ? sockets_lib::program_name ++: (sockets_lib::program_name = argv[0])

#define EXIT(s) exit(s)
#define set_errno(e) (errno = e)
#define isvalidsock(s) (s >= 0)
#endif
