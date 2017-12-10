#include "../inc/viewer.h"

int iv_viewer_set_background( const char *file, IV_DATA *iv, int flush )
{
	Pixmap pixmap;
	
	/* twórz mapę pikseli */
	pixmap = XCreatePixmap( iv->Display, iv->Window, iv->Width, iv->Height, iv->Depth );

	if( !pixmap )
	{
		printf( "Error occured while creating Pixmap object.\n" );
		return -1;
	}
	
	/* załaduj plik do mapy pikseli */
	if( iv_image_load(file, pixmap, iv->Width, iv->Height) )
	{
		XFreePixmap( iv->Display, pixmap );
		return -1;
	}
		
	/* ustaw nowe tło dla okna */
	XSetWindowBackgroundPixmap( iv->Display, iv->Window, pixmap );
	
	/* wyczyść i wyświetl obraz */
	if( flush )
	{
		XClearWindow( iv->Display, iv->Window );
		XFlush( iv->Display );	
	}
	
	/* zwolnij pamięć po mapie pikseli (nie będzie już potrzebna) */
	XFreePixmap( iv->Display, pixmap );
}
