#include "menu.h"

menu::menu(int x, int y)
{
	x_res = x;
	y_res = y;

	position = 0;
}

void menu::get_menu_choice(int &choice, 
			int &maze_size, 
			ray_window &game_window)
{
	choice = -1;

	int button = -1;
	int buffer = -1;

	int box_size_x = (x_res / 2) - 20;
	int box_size_y = 30;

	int box1_x, box2_x, box3_x, box4_x;
	int box1_y, box2_y, box3_y, box4_y;

	box1_y = y_res / 5;
	box2_y = y_res * 2 / 5;
	box3_y = y_res * 3 / 5;
	box4_y = y_res * 4 / 5;

	while(choice < 0)
	{
		button = -1;
		buffer = -1;

		buffer = game_window.read_buffered_input();

		if(buffer > 0)
			button = buffer;

		switch(button)
		{
			case 1:
				position--;
				break;
			case 2:
				position++;
				break;
			case 6:
				choice = position;
				break;
		}

		position = position < 0 ? 3 : position;
		position = position > 3 ? 0 : position;

		game_window.fill_box(0, 0, x_res, y_res, 0x3F3F3F);

		game_window.draw_text_box(20, box1_y, 
					box_size_x, box_size_y, "Easy", 0x600900);
		game_window.draw_text_box(20, box2_y, 
					box_size_x, box_size_y, "Medium", 0x600900);
		game_window.draw_text_box(20, box3_y, 
					box_size_x, box_size_y, "Hard", 0x600900);
		game_window.draw_text_box(20, box4_y, 
					box_size_x, box_size_y, "Quit", 0x600900);

		switch(position)
		{
			case 0:
				game_window.draw_box(19, box1_y - 1,
						box_size_x + 2, box_size_y + 2, 
						0xFF0000);
				break;
			case 1:
				game_window.draw_box(19, box2_y - 1,
						box_size_x + 2, box_size_y + 2, 
						0xFF0000);
				break;
			case 2:
				game_window.draw_box(19, box3_y - 1,
						box_size_x + 2, box_size_y + 2, 
						0xFF0000);
				break;
			case 3:
				game_window.draw_box(19, box4_y - 1,
						box_size_x + 2, box_size_y + 2, 
						0xFF0000);
				break;
		}
	}
}
