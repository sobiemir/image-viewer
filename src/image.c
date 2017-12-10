#include "../inc/image.h"

/* ========================================================================== */

char **iv_image_list_from_folder( const char *dirname, size_t *num )
{
	struct dirent *entity;
	struct stat    st;

	DIR *dir;
	
	char **paths, *ptr,  *file;
	size_t files,  chars, iter, flen;
	
	assert( dirname );
	
	/* otwórz katalog */
	dir = opendir( dirname );
	
	if( dir == NULL )
	{
		printf( "There was an error while opening directory: '%s'.", dirname );
		return NULL;
	}
	
	/* przydziel potrzebne miejsce - ścieżka + nazwa pliku */
	flen = strlen( dirname );
	file = malloc( flen + NAME_MAX + 1 );
	
	/* kopiuj */
	memcpy( file, dirname, flen );
	
	files = 0;
	chars = 0;
	
	/* policz pliki */
	while( (entity = readdir(dir)) )
	{
		int entlen = strlen( entity->d_name );
		
		/* ścieżka do pliku */
		memcpy( file + flen, entity->d_name, entlen );
		*(file + flen + entlen) = '\0';

		stat( file, &st );

		/* sprawdź czy element jest plikiem */
		if( !S_ISREG(st.st_mode) )
			continue;

		/* oraz sprawdź czy posiada prawidłowe rozszerzenie */
		if( !iv_image_has_valid_extension(entity->d_name) )
			continue;
	
		files++;
		chars += strlen( entity->d_name );
	}
	
	/* przydziel pamięć na ścieżki do plików */
	paths = malloc( sizeof *paths * files + flen * files + chars + files );
	ptr   = (char*)(paths + files);
	iter  = 0;
	
	/* rozpocznij odczyt od nowa */
	rewinddir( dir );
	
	while( (entity = readdir(dir)) )
	{
		int entlen = strlen( entity->d_name );
		
		memcpy( file + flen, entity->d_name, entlen );
		*(file + flen + entlen) = '\0';
		
		stat( file, &st );

		if( !S_ISREG(st.st_mode) )
			continue;
		
		if( !iv_image_has_valid_extension(entity->d_name) )
			continue;
		
		/* kopiuj nazwę folderu */
		paths[iter++] = ptr;
		memcpy( ptr, dirname, flen );
		
		/* dołącz do niego nazwę pliku */
		ptr += flen;
		memcpy( ptr, entity->d_name, entlen );
		ptr += entlen;

		*ptr++ = '\0';
	}
		
	/* zamknij katalog */
	closedir( dir );
	

	if( num != NULL )
		*num = files;

	return paths;
}

/* ========================================================================== */

int iv_image_has_valid_extension( const char *path )
{
	/* dostępne rozszerzenia */
	static char *ext[] =
	{
		"jpg", "jpeg", "png",  "tiff", "gif",
		"xpm", "tga",  "pnm",  "bmp"
	};
	static const size_t extlen = sizeof ext / sizeof (char*);
	static char cext[5];
	
	char  *dpos;
	size_t slen, iter;
	
	/* zlokalizuj ostatnią kropkę */
	dpos = strrchr( path, '.' );

	if( !dpos )
		return 0;

	/* długość rozszerzenia (od 4 do 5 znaków z kropką) */
	slen = strlen( dpos );
	if( slen < 4 || slen > 5 )
		return 0;

	/* zmień wielkość liter rozszerzenia */
	cext[0] = *++dpos;
	cext[1] = *++dpos;
	cext[2] = *++dpos;
	cext[3] = slen > 4 ? *++dpos : '\0';
	cext[4] = '\0';
	
	/* sprawdź czy plik posiada odpowiednie rozszerzenie */
	for( iter = 0; iter < extlen; ++iter )
		if( strcmp(cext, ext[iter]) == 0 )
			return 1;
	
	return 0;
}

/* ========================================================================== */

int iv_image_load( const char *file, Pixmap pixmap, int width, int height )
{
	Imlib_Load_Error error = IMLIB_LOAD_ERROR_NONE;

	Imlib_Image image   = NULL;
	Imlib_Image cropped = NULL;

	int    w, h, diffx, diffy, diffz;
	double aspect1, aspect2;
	
	assert( file );
	assert( width != 0 );
	assert( height != 0 );
	
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

	imlib_context_set_image( image );

	/* pobierz wymiary obrazka */
	w = imlib_image_get_width();
	h = imlib_image_get_height();

	aspect1 = (double)width / (double)height;
	aspect2 = (double)w / (double)h;

	/* korekta w przypadku gdy szerokość przy skalowaniu będzie niepoprawna */
	if( aspect1 < aspect2 )
	{
		aspect2 = (double)h / (double)height;

		diffz = (int)((double)width * aspect2);
		diffy = 0;

		if( diffz % 2 > 0 )
			diffz = diffz + 1;

		diffx = (int)((double)abs(w - diffz) / 2.0);
		w     = diffz;
	}
	/* korekta w przypadku gdy wysokość przy skalowaniu będzie niepoprawna */
	else
	{
		aspect1 = (double)w / (double)width;

		diffz = (int)((double)height * aspect1);
		diffx = 0;

		if( diffz % 2 > 0 )
			diffz = diffz + 1;

		diffy = (int)((double)abs(h - diffz) / 2.0);
		h     = diffz;
	}

	/* przytnij obraz do obliczonych wymiarów */
	cropped = imlib_create_cropped_image( diffx, diffy, w, h );
	imlib_free_image();

	/* przypisz dane do mapy pikseli */
	imlib_context_set_drawable( pixmap );
	imlib_context_set_image( cropped );
	imlib_render_image_on_drawable_at_size( 0, 0, width, height );
	
	/* zwolnij pamięć po obrazku */
	imlib_free_image();

	return EXIT_SUCCESS;
}
