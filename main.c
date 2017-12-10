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

	entity = argv[1];

	/* ustawienia domyślne */
	wndinfo.Timer      = 3;
	wndinfo.Stepper    = 0.05;
	wndinfo.MicroTimer = 30000;

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
	iv_viewer_set_background( entity, &wndinfo, 1 );
	
	/* zamknij połączenie z X11 */
	XCloseDisplay( wndinfo.Display );

	/* zwolnij pamięć przydzieloną na listę obrazków */
	if( images )
		free( images );
	
	return 0;
}
