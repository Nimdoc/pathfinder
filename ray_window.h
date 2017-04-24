#ifndef Included_ray_window_H
#define Included_ray_window_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>

#include <stdlib.h>
#include <string>
#include <iostream>

#include "wall_object.h"

struct keyboard_input
{
	bool W_FORWARD		= false;
	bool W_BACKWARD		= false;
	bool W_LEFT		= false;
	bool W_RIGHT		= false;

	bool W_QUIT		= false;
};

class ray_window
{
	public:
		ray_window(int v_XRES, int v_YRES);
		~ray_window();
	
		void read_events(keyboard_input &inputs);

		void line_cast_to_buffer(wall_object line_array[]);
		void draw_rectangle_to_buffer(int X1, int Y1, 
						int X2, int Y2,
						int color);

		void draw_text(int x, int y, std::string text);

		void draw_buffer();
		
		void line_cast(wall_object line_array[]);
		void clear();

		void draw_text_box(int x, int y,
                	int x_size, int y_size,
	                std::string text,
	                int color);

		void fill_box(int x, int y,
                        int x_size, int y_size,
                        int color);

		int read_buffered_input();

		void draw_box(int x, int y,
	                int x_size, int y_size,
        	        int color);

	private:

		Display *current_display;
		int screen;
		Window current_window;
		GC graphics_context;

		XEvent event;

		int XRES;
		int YRES;

		XImage *window_buffer;
		
		void initialize();
		void close();

};

#endif
