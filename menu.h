#ifndef Included_menu_H
#define Included_menu_H


#include "ray_window.h"


class menu
{
	public:
		menu(int x, int y);		

		void get_menu_choice(int &choice, 
                        int &maze_size, 
                        ray_window &game_window);
	private:
		int x_res;
		int y_res;

		int position;
};

#endif
