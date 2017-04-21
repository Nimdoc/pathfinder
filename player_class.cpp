#include <iostream>
#include "player_class.h"
using namespace std;

//Function definitions

//Set Player Position
void Player::set_player_pos(double x, double y)
{
	player_x = x;
	player_y = y;

}

//Get Player X
double Player::get_player_x()
{
	return player_x;

}

//Get Player Y
double Player::get_player_y()
{
	return player_y;

}

//Set Player Speed
void Player::set_player_speed(double speed)
{
	player_speed = speed;

}

//Get Player Speed
double Player::get_player_speed()
{
	return player_speed;

}

//Set Direction
void Player::set_player_dir(int direction)
{
	player_dir = direction;

}

//Get Direction
double Player::get_player_dir()
{
	return player_dir;

}



//Step Forward
void Player::step_forward(double framerate)
{
	// variable to keep current distance traveled and total distance
	float current_dis, total_dis;

	player_x += (framerate * speed);

	// Collision detection
	// Use the get_distance function to see if the player's
	// ray intersects with the wall. If it does, the player can't
	// move any more than the distance returned by get_distance.

	// If the player's ray does intersect, they can only move that 
	// distance.
	if (get_distance(player_x, player_y, wall_x, wall_y) != -1)
	{
		total_dis = get_distance(player_x, player_y, wall_x, wall_y);
		
		//If the current distance is less than the total distance,
		// take a step.
		if (current_dis < total_dis)
		{
			player_x += (framerate * speed);
			current_dis += player_x;
		}
	}
	
	
}

//Step Backward
void Player::step_backward(double framerate)
{

	player_x -= (framerate * speed);
	// Add 180, then convert to radians

	//Degrees to radians: 
	// Radians = (Degrees * pi) / 180
	
	// Collision detection
	
}

//Step Left
void Player::step_left(double framerate)
{

	player_y -= (framerate * speed);
	// Add 270, then convert to radians
	
	// Collision detection

}

//Step Right
void Player::step_right(double framerate)
{

	player_y += (framerate * speed);
	// Add 90, then convert to radians
	
	// Collision detection
	
}

//Turn Left
void Player::turn_left(int amount)
{

	player_dir -= amount;

}

//Turn Right
void Player::turn_right(int amount)
{

	player_dir += amount;

}


//Takes in a direction and a rate and returns the rate of
//travel on the x-axis

float Player::get_stepx(float rate, int degrees)
{
	return (((cos(degrees * M_PI / 180.0f))*rate));
}

//Takes in a direction and a rate and returns the rate of
//travel on the y-axis

float Player::get_stepy(float rate, int degrees)
{
	return (((sin(degrees * M_PI / 180.0f))*rate));
}


//get_distance
float player_class::get_distance(float line1_x1, float line1_y1, float line1_x2, float line1_y2, float line2_x1, float line2_y1, float line2_x2, float line2_y2)
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
