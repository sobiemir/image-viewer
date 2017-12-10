#include <time.h>
#include "inc/image.h"
#include "inc/viewer.h"


int main( int argc, char **argv )
{
	IV_DATA  wndinfo;
	char    *entity = NULL,
	       **images = NULL;
	int      imgnum;
	size_t   iter;
	Display *display;

	/* wyzeruj strukturę */
	memset( &wndinfo, 0, sizeof wndinfo );
	
	/* nazwa pliku musi być podana */
	if( argc < 2 )
		return 1;

	// opcje
	for( iter = 1; iter < argc; ++iter )
	{
		char *data = argv[iter];
		
		/* jeżeli argument nie jest przełącznikiem, traktuj go jako ścieżkę */
		if( data[0] != '-' )
		{
			if( entity != NULL )
				goto _IVG_PRINTHELP;

			entity = data;
			continue;
		}
		
		/* sprawdzaj przełączniki */
		switch( data[1] )
		{
		/* traktuj podaną ścieżkę jako folder */
		case 'd':
			if( data[2] != '\0' )
				goto _IVG_PRINTHELP;

			wndinfo.IsDir = 1;
		break;
		default:
_IVG_PRINTHELP:
			printf( "Here should be help info.\n" );
			return 1;
		}
	}

	/* ustawienia domyślne */
	wndinfo.Timer      = 3;
	wndinfo.Stepper    = 0.05;
	wndinfo.MicroTimer = 30000;

	/* pobierz listę plików z folderu */
	if( wndinfo.IsDir )
	{
		images = iv_image_list_from_folder( entity, &imgnum );

		if( images == NULL )
			return 1;
	}

	/* ziarno losowania */
	srand( (unsigned int)time(NULL) );

	/* otwórz połączenie z X11 */
	display = XOpenDisplay( NULL );

	wndinfo.Display = display;
	wndinfo.Screen  = DefaultScreenOfDisplay( display );
	wndinfo.Window  = RootWindow( display, DefaultScreen(display) );
	wndinfo.Visual  = DefaultVisualOfScreen( wndinfo.Screen );
	wndinfo.Width   = wndinfo.Screen->width;
	wndinfo.Height  = wndinfo.Screen->height;
	wndinfo.Depth   = DefaultDepthOfScreen( wndinfo.Screen );
	
	/* przygotuj bibliotekę Imlib2 do działania */
    imlib_context_set_display( display );
    imlib_context_set_visual( wndinfo.Visual );
    imlib_context_set_colormap( DefaultColormapOfScreen(wndinfo.Screen) );

    /* ustaw obraz */
	if( !wndinfo.IsDir )
		iv_viewer_set_background( entity, &wndinfo, 1 );
	else
		iv_viewer_set_background( images[rand() % imgnum], &wndinfo, 1 );

	/* zamknij połączenie z X11 */
	XCloseDisplay( wndinfo.Display );

	/* zwolnij pamięć przydzieloną na listę obrazków */
	if( images )
		free( images );
	
	return 0;
}
