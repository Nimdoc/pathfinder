#include "ray_caster.h"

/*
This is an inefficient placeholder until I can think of something more
efficient. 

The raycaster will:

For each ray, check every square on the map to see if the ray intersects
anything. If so what is the shortest intersection.

Lets say the map is 8 x 8

That is 64 squares.

lets say half are filled.

If the horizontal resolution is 800, Then that is 32 * 800 ray calculations
per pass through

32 * 800 = 19000 Calculations
*/
void ray_caster::get_raycast_array(wall_object raycast_array[],
                                        int raycast_array_size,
                                        float pos_x, float pos_y,
                                        generate_maze &maze, int maze_size,
                                        int FOV, float direction,
                                        int wall_size)
{
	// Reset all of the wall objects to have no wall
	// and reset the color to maximum brightness
	for(int i = 0; i < raycast_array_size; i++)
	{
		raycast_array[i].set_size(-1);
		raycast_array[i].set_base_hex_color(0xFF,0x00,0x00);
		raycast_array[i].set_hex_color(0xFF,0x00,0x00);
	}


	float shortest_distance;
	float distance_calc;

	// The start angle to start casting rays
	float start_angle = direction - (FOV / 2.0f);	
	// The angle that we're currently casting rays at
	float current_angle = direction - (FOV / 2.0f);
	// The increment for every ray in the Field of View
	float angle_step = 1.0f * FOV / raycast_array_size;


	for(int i = 0; i < raycast_array_size; i++)
	{
		// Set the current angle of the ray
		current_angle = start_angle + (i * angle_step);

		// Current ray position
		float raypos_x;
		float raypos_y;

		// Direction vector components of the ray
		float ray_dir_x;
		float ray_dir_y;

		// The distance between X and Y steos
		float step_x;
		float step_y;

		// Initial distance to first X and Y coordinate
		float side_x;
		float side_y;

		// Position in the array
		int map_x;
		int map_y;

		// Map array change
		int map_dx;
		int map_dy;

		bool hit = false;
		int wall;
		int side;

		// New wall object colors
		int new_r;
		int new_g;
		int new_b;

		// Get the starting ray position coordinates
		raypos_x = pos_x / wall_size;
		raypos_y = pos_y / wall_size;

		// Get the array starting positions
		map_x = int(pos_x / wall_size);
		map_y = int(pos_y / wall_size);

		// Get the vector components for the ray direction
		ray_dir_x = get_stepx(1.0, current_angle);
		ray_dir_y = get_stepy(1.0, current_angle);

		// Get the step for between X and Y jumps
		step_x = sqrt(1 + pow(ray_dir_y, 2) / pow(ray_dir_x, 2));
		step_y = sqrt(1 + pow(ray_dir_x, 2) / pow(ray_dir_y, 2));


		// See what direction to jump in the array
		if(ray_dir_x < 0)
		{
			map_dx = -1;
			side_x = (raypos_x - map_x) * step_x;
		}
		else
		{
			map_dx = 1;
			side_x = (map_x + 1.0f - raypos_x) * step_x;
		}
		if(ray_dir_y < 0)
		{
			map_dy = -1;
			side_y = (raypos_y - map_y) * step_y;
		}
		else
		{
			map_dy = 1;
			side_y = (map_y + 1.0f - raypos_y) * step_y;
		}

		// While a wall has not been hit..
		while(!hit)
		{
			// There are basically two rays competing
			// 
			// side_x & side_y
			// 
			// They jump from square to square
			// The shortest rays, side_x or side_y,
			// always jumps first
			if(side_x < side_y)
			{
				side_x += step_x;
				map_x += map_dx;
				side = 0;
			}
			else // side_y < side_x
			{
				side_y += step_y;
				map_y += map_dy;
				side = 1;
			}

			// Get what is in the square
			wall = maze.get_square(map_y, map_x);

			// If there is something in the square and
			// it's not the start
			if(wall > 0 && wall != 2)
				hit = true;
		}

		// If a wall is hit
		if(hit)
			// The wall is on the NS side
			if(side == 0)
			{
				distance_calc = (map_x - raypos_x + 
						(1 - map_dx) / 2) / ray_dir_x;
			}
			// The wall is on the EW side
			else
			{
				distance_calc = (map_y - raypos_y + 
						(1 - map_dy) / 2) / ray_dir_y;
			}

		// Multiple the distance calc by the wall size parameter
		distance_calc *= wall_size;

		// Remove fisheye effect
		distance_calc *= cos(((-FOV / 2.0f) + 
				(angle_step * i))* M_PI / 180.0f);


		// If the wall is the end wall
		if(wall == 3)
		{
			// Set the wall to green
			raycast_array[i].set_base_hex_color(0x00, 0xFF, 0x00);
		}

		

		// Set the wall_object color and the wall_object size
		raycast_array[i].set_hex_color(
		raycast_array[i].get_base_red() / (1 + (distance_calc / 100.0f)),
		raycast_array[i].get_base_green() / (1 + (distance_calc / 100.0f)),
		raycast_array[i].get_base_blue() / (1 + (distance_calc / 100.0f)));
			raycast_array[i].set_size(distance_calc);

	}
}

/*
Takes in a direction and a rate and returns the rate of
travel on the X-Axis
*/
float ray_caster::get_stepx(float rate, int degrees)
{
    return (((cos(degrees * M_PI / 180.0f))*rate));
}

/*
Takes in a direction and a rate and returns the rate of
travel on the Y-Axis
*/
float ray_caster::get_stepy(float rate, int degrees)
{
    return (((sin(degrees * M_PI / 180.0f))*rate));
}













