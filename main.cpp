#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

#include "ray_window.h"
#include "wall_object.h"
#include "ray_caster.h"
#include "maze_generator.h"

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

float get_stepx(float rate, int degrees);
float get_stepy(float rate, int degrees);

bool collides(float pos_x, float pos_y,
		float rate, int degrees, 
		generate_maze &maze, int units,
		int num);

bool has_won(float pos_x, float pos_y, generate_maze &maze,
		int units, int num);


int main(int argc, char** argv)
{
	bool is_running = true;
	bool is_winner = false;
	// if no argument or an even number entered,
	// tell user and quit
	
	if (argc !=2) 
		std::cout << "Usage: ./Pathfinder <Size>" << std::endl;
	if (!(atoi(argv[1])%2))
		std::cout << "Size of maze must be an odd number" << std::endl;
	
	// generate maze with size from argument
	generate_maze Maze(atoi(argv[1]));

	// initialize the map
	Maze.init_map();

	// generate the map
	Maze.generate_map();

	// print map
	Maze.print_map();

	/* End of changes I made in main() -Steve */

	ray_window cast_display(RESOLUTION_X, RESOLUTION_Y);

	keyboard_input inputs;

	wall_object rays[RESOLUTION_X];

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

	ray_caster caster;

	float angle = 0;
	float time = 0;
	double pos_x = (Maze.get_start_x() * UNIT_SIZE) + (UNIT_SIZE / 2);
	double pos_y = (Maze.get_start_y() * UNIT_SIZE) + (UNIT_SIZE / 2);

	double frame_rate;

	bool collision;

	// Game loop
	while(is_running)
	{
		frame_rate = get_elapsed_time();

		// Get the sizes of the walls to be drawn
		caster.get_raycast_array(rays, RESOLUTION_X,
                                 	pos_x, pos_y,
				 	Maze, atoi(argv[1]),
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
			collision = collides(pos_x, pos_y,
			                SPEED * frame_rate + 10, 
					angle, Maze, UNIT_SIZE,
        		       		1);
	
			// Move player forwards
			if(!collision)
			{
			pos_x += get_stepx(frame_rate * SPEED, angle);
			pos_y += get_stepy(frame_rate * SPEED, angle);
			}
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
			is_running = false;

		if(has_won(pos_x, pos_y, Maze, UNIT_SIZE, 3))
		{
			is_running = false;
			is_winner = true;
		}
	}
	
	if(is_winner)
	{
		cast_display.clear();

		cast_display.fill_box(0, 0, 
				RESOLUTION_X, RESOLUTION_Y,
				0x2F2F2F);

		cast_display.draw_text((RESOLUTION_X / 2) - 20,
					(RESOLUTION_Y / 2) - 20,
					"WINNER.", 0xFF0000);

		while(!inputs.W_QUIT)
		{
			cast_display.read_events(inputs);
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

/*
Really simple collision detection. Just stops the player if there
is going to be a collision in the map.
*/
bool collides(float pos_x, float pos_y,
		float rate, int degrees, 
		generate_maze &maze, int units,
		int num)
{
	float end_x = pos_x + get_stepx(rate, degrees);
	float end_y = pos_y + get_stepy(rate, degrees);

	int map_x = end_x / units;
	int map_y = end_y / units;

	if(maze.get_square(map_y, map_x) == num)
		return true;
	else
		return false;
		
}

/*
Returns true if the player is in the defined winning square
*/
bool has_won(float pos_x, float pos_y, generate_maze &maze,
		int units, int num)
{
	float map_x = pos_x / units;
	float map_y = pos_y / units;

	if(maze.get_square(map_y, map_x) == num)
		return true;
	else
		return false;
}








