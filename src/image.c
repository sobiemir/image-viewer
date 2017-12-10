#include "../inc/image.h"

/* ================================================================================================================== */

int iv_image_load( const char *file, Pixmap pixmap, int width, int height )
{
	Imlib_Load_Error error   = IMLIB_LOAD_ERROR_NONE;
	Imlib_Image      image   = NULL;
	Imlib_Image      cropped = NULL;
	
	assert( file );
	
	/* próbuj otworzyć plik */
	image = imlib_load_image_with_error_return( file, &error );
	
	/* wykryj błąd */
	if( !image || error != IMLIB_LOAD_ERROR_NONE )
	{
		switch( error )
		{
		case IMLIB_LOAD_ERROR_FILE_DOES_NOT_EXIST:
			printf( "File with name '%s' doesn't exists.\n", file );
		break;
		case IMLIB_LOAD_ERROR_OUT_OF_MEMORY:
			printf( "Out of memory.\n" );
		break;
		case IMLIB_LOAD_ERROR_NO_LOADER_FOR_FILE_FORMAT:
			printf( "File format is not supported.\n" );
		break;
		case IMLIB_LOAD_ERROR_PATH_TOO_LONG:
			printf( "Path to file: '%s' is too long.\n", file );
		break;
		case IMLIB_LOAD_ERROR_PERMISSION_DENIED_TO_READ:
			printf( "Access denied for file: '%s' .\n", file );
		break;
		default:
			printf( "Error occured while loading file: '%s'.\n", file );
		}
		return 1;
	}
	
	/* przypisz dane do mapy pikseli */
	imlib_context_set_drawable( pixmap );
	imlib_context_set_image( image );
	imlib_render_image_on_drawable_at_size( 0, 0, width, height );
	
	/* zwolnij pamięć po obrazku */
	imlib_free_image();

	return EXIT_SUCCESS;
}
