#include <cmath>
#include <climits>

#include "wall_object.h"
#include "maze_generator.h"

class ray_caster
{
	public:
		void get_raycast_array(wall_object raycast_array[],
					int raycast_array_size,
					float pos_x, float pos_y,
				  	generate_maze &maze, int maze_size,
				  	int FOV, float direction,
					int wall_size);
	private:
		float get_stepx(float rate, int degrees);
		float get_stepy(float rate, int degrees);
};
