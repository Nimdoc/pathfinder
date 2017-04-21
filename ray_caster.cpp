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
		raycast_array[i].set_red(0xFF);
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
		int side;

		raypos_x = pos_x / wall_size;
		raypos_y = pos_y / wall_size;

		map_x = int(pos_x / wall_size);
		map_y = int(pos_y / wall_size);

		ray_dir_x = get_stepx(1.0, current_angle);
		ray_dir_y = get_stepy(1.0, current_angle);

		step_x = sqrt(1 + pow(ray_dir_y, 2) / pow(ray_dir_x, 2));
		step_y = sqrt(1 + pow(ray_dir_x, 2) / pow(ray_dir_y, 2));

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

if(i == 320)
std::cout << std::endl << "SX: " << step_x << "  " <<  "SY: " << step_y
	<< "  " << "SiX: " << side_x << "  " << "SiY: " << side_y
	<< "  " << "RPX: " << raypos_x << "  " << "RPY: " <<raypos_y;

		while(!hit)
		{
			if(side_x < side_y)
			{
				side_x += step_x;
				map_x += map_dx;
				side = 0;
			}
			else
			{
				side_y += step_y;
				map_y += map_dy;
				side = 1;
			}

			if(maze.get_square(map_y, map_x) > 0)
				hit = true;
		}

		if(hit)
		if(side == 0)
		{
			distance_calc = (map_x - raypos_x + 
					(1 - step_x) / 2) / ray_dir_x;
		}
		else
		{
			distance_calc = (map_y - raypos_y + 
					(1 - step_y) / 2) / ray_dir_y;
		}

		distance_calc *= wall_size;

		raycast_array[i].set_hex_color(
		raycast_array[i].get_base_red() / (distance_calc / 100.0f),
		raycast_array[i].get_base_green() / (distance_calc / 100.0f),
		raycast_array[i].get_base_blue() / (distance_calc / 100.0f));
			raycast_array[i].set_size(distance_calc);

	}
}


/*
This is how I imagined the sides of the boxes

	----1----
	|	|
	2	3
	|	|
	----4----

get_box_distance checks each side of the box against a line for an
intersection and returns the shortest distance from the origin of the
line or returns -1 if no intersection was found
*/
float ray_caster::get_box_distance(float X1, float Y1, float X2, float Y2,
                                        int box_x, int box_y, int wall_size)
{
	bool hit_wall = false;
	float shortest_distance = 99999999999;
	float current_distance;


	// Check side 1
	current_distance = get_distance(X1, Y1, X2, Y2,
                        		box_x, box_y,
                        		box_x + wall_size, box_y);
	
	// Check if the box side distance is shorter than the current distance
	// and the distance exists (greater than 0!). If so set the shortest
	// distance to the calculated distance and set the hit_wall flag
	// to true 
	if(current_distance < shortest_distance && current_distance > 0)
		{
			shortest_distance = current_distance;
			hit_wall = true;
		}
	// Check side 2		
	current_distance = get_distance(X1, Y1, X2, Y2,
                        		box_x, box_y,
                        		box_x, box_y + wall_size);

	// Check if the box side distance is shorter than the current distance
	// and the distance exists (greater than 0!). If so set the shortest
	// distance to the calculated distance and set the hit_wall flag
	// to true 
	if(current_distance < shortest_distance && current_distance > 0)
		{
			shortest_distance = current_distance;
			hit_wall = true;
		}

	// Check side 3
	current_distance = get_distance(X1, Y1, X2, Y2,
                        		box_x, box_y + wall_size,
                        		box_x + wall_size, box_y + wall_size);

	// Check if the box side distance is shorter than the current distance
	// and the distance exists (greater than 0!). If so set the shortest
	// distance to the calculated distance and set the hit_wall flag
	// to true 
	if(current_distance < shortest_distance && current_distance > 0)
		{
			shortest_distance = current_distance;
			hit_wall = true;
		}

	// Check side 4
	current_distance = get_distance(X1, Y1, X2, Y2,
                        		box_x + wall_size, box_y,
                        		box_x + wall_size, box_y + wall_size);
	
	// Check if the box side distance is shorter than the current distance
	// and the distance exists (greater than 0!). If so set the shortest
	// distance to the calculated distance and set the hit_wall flag
	// to true 
	if(current_distance < shortest_distance && current_distance > 0)
		{
			shortest_distance = current_distance;
			hit_wall = true;
		}

	// If a wall was hit
	if(hit_wall)
		// Return the shortest distance
		return shortest_distance;
	else // Wall was not hit
		// return an impossible distance
		return -1;
}

bool ray_caster::lineIntersection(
float Ax, float Ay,
float Bx, float By,
float Cx, float Cy,
float Dx, float Dy,
float *X, float *Y) {

  float  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line is undefined.
  if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return false;

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if the lines are parallel.
  if (Cy==Dy) return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;

  //  Success.
  return true; } 

float ray_caster::get_distance(float line1_x1, float line1_y1,
                                        float line1_x2, float line1_y2,
                                        float line2_x1, float line2_y1,
                                        float line2_x2, float line2_y2)
{
	float p_x, p_y, distance;

	lineIntersection(line1_x1, line1_y1,
                         line1_x2, line1_y2,
                         line2_x1, line2_y1,
                         line2_x2, line2_y2,
			 &p_x, &p_y);

	distance = sqrt(pow(line1_x1 - p_x, 2) + pow(line1_y1 - p_y, 2));

	return distance;
}

/*
float ray_caster::get_distance(float line1_x1, float line1_y1,
                                        float line1_x2, float line1_y2,
                                        float line2_x1, float line2_y1,
                                        float line2_x2, float line2_y2)
{
	float distance;

	float A1 = line1_y2 - line1_y1;
	float B1 = line1_x1 - line1_x2;
	float C1 = (A1 * line1_x1) + (B1 * line1_y1);

	float A2 = line2_y2 - line2_y1;
	float B2 = line2_x1 - line2_x2;
	float C2 = (A2 * line2_x1) + (B2 * line2_y1);

	

    float denominator = (A1 * B2) - (A2 * B1);

    if(denominator == 0)
    {
        return -1;
    }

    // These following intersection values are assuming the line is
    // infinitly extendeding.

    float intersectX = ((B2 * C1) - (B1 * C2)) / denominator;
    float intersectY = ((A1 * C2) - (A2 * C1)) / denominator;

    // These following values are used for checking if the intersection
    // point is within the bounds of the lines.

    float rx0 = (intersectX - line1_x1) / (line1_x2 - line1_x1);
    float ry0 = (intersectY - line1_y1) / (line1_y2 - line1_y1);
    float rx1 = (intersectX - line2_x1) / (line2_x2 - line2_x1);
    float ry1 = (intersectY - line2_y1) / (line2_y2 - line2_y1);


    if(((rx0 >= 0.0f && rx0 <= 1) || 
	(ry0 >= 0.0f && ry0 <= 1)) && 
	((rx1 >= 0.0f && rx1 <= 1) || 
	(ry1 >= 0.0f && ry1 <= 1)))
    { 
        return sqrt(pow((intersectX - line1_x1), 2) + 
		    pow((intersectY - line1_y1), 2));
    }
    else
    {
        return -1;
    }
}
*/

void ray_caster::get_intersect_points(float line1_x1, float line1_y1,
                                        float line1_x2, float line1_y2,
                                        float line2_x1, float line2_y1,
                                        float line2_x2, float line2_y2,
                                        float x, float y)
{
	float distance;

	float A1 = line1_y2 - line1_y1;
	float B1 = line1_x1 - line1_x2;
	float C1 = (A1 * line1_x1) + (B1 * line1_y1);

	float A2 = line2_y2 - line2_y1;
	float B2 = line2_x1 - line2_x2;
	float C2 = (A2 * line2_x1) + (B2 * line2_y1);

	

    float denominator = (A1 * B2) - (A2 * B1);

    if(denominator == 0)
    {
        return;
    }

    // These following intersection values are assuming the line is
    // infinitly extendeding.

    float intersectX = ((B2 * C1) - (B1 * C2)) / denominator;
    float intersectY = ((A1 * C2) - (A2 * C1)) / denominator;

    // These following values are used for checking if the intersection
    // point is within the bounds of the lines.

    float rx0 = (intersectX - line1_x1) / (line1_x2 - line1_x1);
    float ry0 = (intersectY - line1_y1) / (line1_y2 - line1_y1);
    float rx1 = (intersectX - line2_x1) / (line2_x2 - line2_x1);
    float ry1 = (intersectY - line2_y1) / (line2_y2 - line2_y1);


    if(((rx0 >= 0.0f && rx0 <= 1) || 
	(ry0 >= 0.0f && ry0 <= 1)) && 
	((rx1 >= 0.0f && rx1 <= 1) || 
	(ry1 >= 0.0f && ry1 <= 1)))
    {
	x = intersectX;
	y = intersectY; 
    }
    else
    {
        x = -1;
	y = -1;
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

















