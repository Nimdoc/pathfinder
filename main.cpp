#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

#include "ray_window.h"
#include "wall_object.h"
#include "ray_caster.h"
#include "maze_generator.h"
#include "menu.h"

// GLOBAL CONSTANTS

#define RESOLUTION_X 640
#define RESOLUTION_Y 480

#define CUBE_SIDE_LENGTH 100
#define UNIT_SIZE 100

#define FIELD_OF_VIEW 70 

#define SPEED 250
#define TURN_RATE 100

// HELPER FUNCTIONS

double get_elapsed_time();


// Chris these should be in your player class. 
// They're here so we can experiment.
float get_stepx(float rate, int degrees);
float get_stepy(float rate, int degrees);

int main(int argc, char** argv)
{
	bool program_running = true;
	bool game_running = false;

	int choice = -1;
	int maze_size;

	float angle = 0;
	float time = 0;

	double pos_x;
	double pos_y;

	double frame_rate;

	ray_caster caster;

	menu game_menu(RESOLUTION_X, RESOLUTION_Y);

	ray_window cast_display(RESOLUTION_X, RESOLUTION_Y);

	keyboard_input inputs;

	wall_object rays[RESOLUTION_X];

	generate_maze Maze(5);

	// if no argument or an even number entered,
	// tell user and quit
		

	/* End of changes I made in main() -Steve */

	// Set default values for all the wall_objects in the rays array
	for(int i = 0; i < RESOLUTION_X; i++)
	{
		// Set the max height of a ray
		rays[i].set_max(RESOLUTION_Y);
		// Define wall units
		rays[i].set_unit(CUBE_SIDE_LENGTH);
		// Set the actual size of the way, using distance.
		// Will have to change later
		rays[i].set_size(i);
		// Set the color of the wall
		rays[i].set_base_hex_color(0xFF,0x00,0x00);
	}
	

	// Game loop
	while(program_running)
	{

	game_menu.get_menu_choice(choice, maze_size, cast_display);

	switch(choice)
	{
		case 0:
			maze_size = 13;
			game_running = true;
			break;
		case 1:
			maze_size = 25;
			game_running = true;
			break;
		case 2:
			maze_size = 37;
			game_running = true;
			break;
		case 3:
			maze_size = 5;
			game_running = false;
			break;
	}

	// generate maze with size from argument
	Maze = generate_maze(maze_size);

	// initialize the map
	Maze.init_map();

	// generate the map
	Maze.generate_map();

	// print map
	Maze.print_map();

	pos_x = (Maze.get_start_x() * UNIT_SIZE) + (UNIT_SIZE / 2);
	pos_y = (Maze.get_start_y() * UNIT_SIZE) + (UNIT_SIZE / 2);


	while(game_running)
	{
		frame_rate = get_elapsed_time();

		// Get the sizes of the walls to be drawn
		caster.get_raycast_array(rays, RESOLUTION_X,
                                 	pos_x, pos_y,
				 	Maze, maze_size,
                                 	FIELD_OF_VIEW, angle,
                                 	CUBE_SIDE_LENGTH);
	
		// Draw the ceiling
		cast_display.draw_rectangle_to_buffer(0,0, RESOLUTION_X - 1,
				RESOLUTION_Y / 2, 0xFFFFFF);

		// Draw the floor
		cast_display.draw_rectangle_to_buffer(0, RESOLUTION_Y / 2, 
				RESOLUTION_X - 1, RESOLUTION_Y - 1, 0xBA924E);
		// Draw the wall
		cast_display.line_cast_to_buffer(rays);
		
		cast_display.draw_rectangle_to_buffer(
			(RESOLUTION_X / 2) - 4,
			(RESOLUTION_Y / 2) - 4,
			(RESOLUTION_X / 2) + 4,
			(RESOLUTION_Y / 2) + 4,
			0x000000);

		cast_display.draw_rectangle_to_buffer(
			(RESOLUTION_X / 2) - 2,
			(RESOLUTION_Y / 2) - 2,
			(RESOLUTION_X / 2) + 2,
			(RESOLUTION_Y / 2) + 2,
			0xFFFFFF);

		// Draw the buffer to the screen
		cast_display.draw_buffer();

		// Read the events and get keyboard inputs
		cast_display.read_events(inputs);

		// Modify player position
		if(inputs.W_FORWARD)
		{
			// Move player forwards
			pos_x += get_stepx(frame_rate * SPEED, angle);
			pos_y += get_stepy(frame_rate * SPEED, angle);
		}
		if(inputs.W_BACKWARD)
		{
			// Move player backwards
			pos_x += get_stepx(frame_rate * SPEED, 180 + angle);
			pos_y += get_stepy(frame_rate * SPEED, 180 + angle);
		}
		if(inputs.W_LEFT)
		{
			// Turn player to the left
			angle -= frame_rate * TURN_RATE;
		}
		if(inputs.W_RIGHT)
		{
			// Turn player to the right
			angle += frame_rate * TURN_RATE;
		}

		// Check if quit button was pressed 
		if(inputs.W_QUIT)
			game_running = false;
	}
	}
}

double get_elapsed_time()
{
	static clock_t begin = 10;
	clock_t end = clock();
	
	double elapsed_time = double(end - begin) / CLOCKS_PER_SEC;

	begin = clock();
}

/*
Takes in a direction and a rate and returns the rate of
travel on the X-Axis
*/
float get_stepx(float rate, int degrees)
{
    return (((cos(degrees * M_PI / 180.0f))*rate));
}

/*
Takes in a direction and a rate and returns the rate of
travel on the Y-Axis
*/
float get_stepy(float rate, int degrees)
{
    return (((sin(degrees * M_PI / 180.0f))*rate));
}
