#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

#ifndef NAME_MAX
#	define NAME_MAX 256
#endif
#ifndef size_t
#	define size_t int
#endif

int iv_image_has_valid_extension( const char *path );
char **iv_image_list_from_folder( const char *folder, int *num );
int iv_image_load( const char *file, Pixmap pixmap, int width, int height );
