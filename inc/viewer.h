#include <X11/Xlib.h>
#include <stdio.h>
#include "image.h"

typedef struct IVS_DATA
{
	/* monitor wyświetlający */
	Display *Display;

	/* okno aplikacji */
	Window Window;

	/* pulpit */
	Screen *Screen;

	/* ustawienia wizualne */
	Visual *Visual;

	/* Czy podany został folder obrazków czy sam obrazek? */
	int IsDir;

	/* Czy włączyć pokaz slajdów? */
	int SlideShow;

	/* Typ pokazu slajdów */
	int SlideType;

	/* Czasomierz */
	int Timer;

	/* Dla usleep, nie używany na korzyść XSync */
	int MicroTimer;

	/* Szybkość przejścia animacji */
	double Stepper;

	/* Szerokość okna aplikacji */
	int Width;

	/* Wysokość okna aplikacji */
	int Height;

	/* Głębia kolorów okna aplikacji */
	unsigned int Depth;
}
IV_DATA;

int iv_viewer_set_background( const char *file, IV_DATA *iv, int flush );
