// NOTE: Maze needs to receive an odd number as parameter as mazes
// with even number of cells don't generate as nicely

#ifndef Included_maze_generator_H
#define Included_maze_generator_H

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stack>

// max size of maze x/y value
#define MAX_SIZE 55

// cell types
#define SPACE 0
#define WALL 1
#define START 2
#define END 3
#define CHEAT_PATH 4

// wall directions
enum class Direction{
	NORTH,
	SOUTH,
	WEST,
	EAST
};

using namespace std;

// cell struct for each element of the array
// cell can be an empty slot, wall, start, or end of maze
// each cell keeps track of the walls to each of its sides
// and whether or not its been visited
struct Cell {
	bool visited;
	bool north_wall;
	bool south_wall;
	bool west_wall;
	bool east_wall;

	// 0 = empty; 1 = wall; 2 = start; 3 = end
	int cell_type;
};

class generate_maze {
private:
	int start_X, start_Y;
	int end_X, end_Y;
	int map_size;

	// these MAX_SIZE values are the maximum size the map can be,
	// the user still inputs the size of the map to generate
	// i can make this a 2d Vector of cells if you'd prefer
	// so we can make bigger mazes
	// since arrays need chunks of memory, once the size of the map gets
	// too large the program just crashes on startup
	// if you want me to change it to a vector I can,
	// wanted to ask first because I haven't really looked at how much
	// it'd take to change the code to work with a vector instead of an array
	// and if that's something you'd want to do or not
	Cell map[MAX_SIZE][MAX_SIZE];
public:
	generate_maze();
	generate_maze(int size) {
		start_X = 0;
		start_Y = 0;
		end_X = 0;
		end_Y = 0;
		map_size = size;
	}

	void init_map();
	void generate_map();
	void print_map();
	void cheat_mode(stack<int> cheat_Y, stack<int>cheat_X);

	int get_start_x() {return start_X;}
	int get_start_y() {return start_Y;}
	int get_end_x() {return end_X;}
	int get_end_y() {return end_Y;}

	void set_start_pos(int Y, int X) {start_Y = Y; start_X = X;}
	void set_end_pos(int Y, int X) {end_Y = Y; end_X = X;}

	// hope this doesn't get confusing, instead of (x,y) it's (y,x) because
	// first num in 2d arr is row and second is col so i went with y,x
	int get_square(int y, int x) {return map[y][x].cell_type;}
};

#endif
