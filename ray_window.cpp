#include "ray_window.h"

ray_window::ray_window(int v_XRES, int v_YRES)
{
	// Set the XRES & YRES to the visiting variables
	XRES = v_XRES;
	YRES = v_YRES;

	// Initialize the display
	initialize();


	// Initialize window_buffer
	char *image32=(char *)malloc(XRES*YRES*4);
        window_buffer = XCreateImage(current_display, 
		DefaultVisual(current_display,0),
                DefaultDepth(current_display, 
		DefaultScreen(current_display)),
                ZPixmap, 0, image32, XRES, YRES, 32, 0);
}

ray_window::~ray_window()
{
	// Free up resources
	close();
}

void ray_window::initialize()
{
	unsigned long black,white;

	// Create the X connection to open the current_displayplay
        current_display = XOpenDisplay((char *)0);
        screen = DefaultScreen(current_display);
        black = BlackPixel(current_display,screen),
        white = WhitePixel(current_display, screen);
        
	// Create the window after the current_displayplay is initialized
	current_window=XCreateSimpleWindow(current_display,
					   DefaultRootWindow(current_display),
					   0,0,
                			   XRES, YRES, 5,
					   black, white);
        
	// Set the properties of the window
	XSetStandardProperties(current_display,
				current_window,
				"Ray Window",
				"Ray Window",
				None,NULL,0,NULL);
        
	// Set the type of inputs that are allowed
	XSelectInput(current_display, current_window, 
		     ExposureMask|KeyPressMask|KeyReleaseMask);
        
	// Create graphics content
	graphics_context=XCreateGC(current_display, current_window, 0,0);
        
	// Set the background and foreground colors
	XSetBackground(current_display,graphics_context,white);
        XSetForeground(current_display,graphics_context,black);
        
	// Clear the window and bring it to the top of the other windows
	XClearWindow(current_display, current_window);
        XMapRaised(current_display, current_window);
}

void ray_window::close()
{
	// Free up system resources
	XDestroyImage(window_buffer);
	XFreeGC(current_display, graphics_context);
        XDestroyWindow(current_display,current_window);
        XCloseDisplay(current_display);
}


// Chris if you're reading this this is problem a function to work on.
void ray_window::read_events(keyboard_input &inputs)
{

	char keys[25];
	int len = 25;
	KeySym keysym;

	inputs.W_SELECT = false;
/*
	// Reset keyboard inputs	
	inputs.W_FORWARD	= false;
	inputs.W_BACKWARD	= false;
	inputs.W_LEFT		= false;
	inputs.W_RIGHT		= false;
	inputs.W_QUIT		= false;
*/

	// If there is an event to be read. Right now either
	// key press or key release.
	for(int i = 0; i < 10; i++)
	if(XCheckMaskEvent(current_display, 
			   KeyPressMask|KeyReleaseMask, 
			   &event))
		switch(event.type)
		{
			// Event a button was pressed
			case KeyPress:
				// Gets the keys pressed
				len = XLookupString(&event.xkey, 
						keys, 25, &keysym, 
						NULL);

				// Cycle through keys to see
				// Which ones were pressed and
				// Set the pressed ones to true
				for(int i = 0; i < len; i++)
				{
					if (keys[i] == 'w')
					{
						inputs.W_FORWARD = true;
					}
					if (keys[i] == 's')
					{
						inputs.W_BACKWARD = true;
					}
					if (keys[i] == 'a')
					{
						inputs.W_LEFT = true;
					}
					if (keys[i] == 'd')
					{
						inputs.W_RIGHT = true;
					}
					if (keys[i] == 'p')
					{
						inputs.W_QUIT = true;
					}
					if (keys[i] == 'k')
					{
				//		inputs.W_SELECT = true;
					}
				}
				break;
			case KeyRelease:
				// Gets the keys pressed
				len = XLookupString(&event.xkey, 
						keys, 25, &keysym, 
						NULL);

				// Cycle through keys to see
				// Which ones were pressed and
				// Set the pressed ones to true
				for(int i = 0; i < len; i++)
				{
					if (keys[i] == 'w')
					{
						inputs.W_FORWARD = false;
					}
					if (keys[i] == 's')
					{
						inputs.W_BACKWARD = false;
					}
					if (keys[i] == 'a')
					{
						inputs.W_LEFT = false;
					}
					if (keys[i] == 'd')
					{
						inputs.W_RIGHT = false;
					}
					if (keys[i] == 'p')
					{
						inputs.W_QUIT = false;
					}
					if (keys[i] == 'k')
					{
						inputs.W_SELECT = true;
					}
				}
				break;
		}
}

/*
Takes an array of wall_objects and displays them across the screen
the line object array must be smaller than the X resolution and
should be equal to it.
*/
void ray_window::line_cast_to_buffer(wall_object line_array[])
{
	int current_point;
	int draw_size;

	// For every line in the X direction..
	for(int i = 0; i < XRES; i++)
	{
		// Start from the center minus half of the size
		current_point = (YRES / 2) - (line_array[i].get_size() / 2);
		draw_size = (YRES / 2) + (line_array[i].get_size() / 2);

		// If the size is off the window less than zero set the draw
		// point to the edge
		if(current_point < 0)
			current_point = 0;

		// While there are points that need to be drawn...
		while(current_point < draw_size && current_point < YRES)
		{
			// Make sure we're within the bounds
			if((current_point > 0) && (current_point < YRES))
			{
				// Draw the point on the window buffer
				XPutPixel(window_buffer, i, current_point,  
						line_array[i].get_hex_color());
			}
			// Proceed to the next point
			current_point++;
		}
	}
}

void ray_window::draw_text(int x, int y, std::string text, int color)
{
	XSetForeground(current_display,graphics_context,
                                        color);

	XDrawString(current_display, current_window, graphics_context,
		x, y, text.c_str(), text.length());
}

/*
Draws a rectangle from the top left point to the bottom right point.
Must be from top left to bottom right.
*/
void ray_window::draw_rectangle_to_buffer(int X1, int Y1,
				int X2, int Y2,
				int color)
{
	// Loop through the X row
	for(int i = X1; i < X2; i++)
	{
		// Make sure we're in bounds of the window buffer
		if((i > 0) && (i < XRES))
		{
			// Loop through the Y row
			for(int j = Y1; j < Y2; j++)
			{
				// Make sure we're still in bounds
				if((j > 0) && (j < YRES))
				{
					// Draw the point on to the
					// window buffer 
					XPutPixel(window_buffer, i, j, color);
				}
			}
		}
	}
}

/*
Draws the window buffer to the screen
*/
void ray_window::draw_buffer()
{
	// Send window_buffer to the screen
	XPutImage(current_display, current_window, 
		DefaultGC(current_display, 0), window_buffer,
		0,0,0,0, XRES, YRES);
}

void ray_window::line_cast(wall_object line_array[])
{
	XClearWindow(current_display, current_window);

	// Set the draw color to white and draw the ceiling
	XSetForeground(current_display,graphics_context,
					0xFFFFFF
					);

	XFillRectangle(current_display,
			current_window,
			graphics_context,
			0, 0,
			XRES, (YRES / 2));

	// Set the draw color to tan and draw the floor
	XSetForeground(current_display,graphics_context,
					0xBA924E
					);
	XFillRectangle(current_display,
			current_window,
			graphics_context,
			0, (YRES / 2),
			XRES, YRES);

	for(int i = 0; i < XRES; i++)
	{
		// Set the draw color to the shade of the current
		// wall_object
		XSetForeground(current_display,graphics_context,
                                        line_array[i].get_hex_color()
                                        );
		
		// If the size of the wall is greater than zero, as in it
		// exists, then draw the wall
		if(line_array[i].get_size() > 0)
			XDrawLine(current_display, 
				current_window, 
				graphics_context,
			  i, ((YRES / 2) - (line_array[i].get_size() / 2)),
			  i, ((YRES / 2) + (line_array[i].get_size() / 2)));
	}
}

void ray_window::clear()
{
	// Set the window to white
	XClearWindow(current_display, current_window);
}

void ray_window::draw_text_box(int x, int y, 
		int x_size, int y_size,
		std::string text,
		int color)
{
	XSetForeground(current_display,graphics_context, color);

	XDrawRectangle(current_display, current_window, graphics_context,
			x, y, x_size, y_size);
	draw_text(x + 8, y + (y_size / 2), text, 0x000000);
}

void ray_window::draw_box(int x, int y,
                int x_size, int y_size,
                int color)
{
        XSetForeground(current_display,graphics_context, color);

        XDrawRectangle(current_display, current_window, graphics_context,
                        x, y, x_size, y_size);
}

void ray_window::fill_box(int x, int y,
			int x_size, int y_size,
        	        int color)
{
	XSetForeground(current_display,graphics_context, color);

	XFillRectangle(current_display,
                        current_window,
                        graphics_context,
                        x, y, 
                        x_size, y_size);
}

int ray_window::read_buffered_input()
{
	int input = -1;

	// Read next event.
	XNextEvent(current_display, &event);
	switch(event.type)
	{
		case KeyPress:
			char keys[25];
			int len = 25;
			KeySym keysym;
			len = XLookupString(&event.xkey, 
					keys, 25, &keysym, 
					NULL);
			for(int i = 0; i < len; i++)
			{
				if (keys[i] == 'w')
				{
					input = 1;
				}
				else if (keys[i] == 's')
				{
					input = 2;
				}
				else if (keys[i] == 'a')
				{
					input = 3;
				}
				else if (keys[i] == 'd')
				{
					input = 4;
				}
				else if (keys[i] == 'p')
				{
					input = 5;
				}
				else if (keys[i] == 'k')
				{
					input = 6;
				}
				else
				{
					input = -1;
				}
			}
			break;
	}

	return input;
}











