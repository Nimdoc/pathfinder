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
#define EMPTY 0
#define WALL 1
#define START 2
#define END 3
#define CHEAT_PATH 4

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3

// flags for cout to color text in console
// no real use just made the console maze easier to see
#define RESET   "\033[0m" 		/* Reset color */
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

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

	// 0 = empty; 1 = wall; 2 = start; 3 = end; 4 = cheat_path
	int cell_type;
};

class generate_maze {
private:
	bool cheats = true;
	Cell map[MAX_SIZE][MAX_SIZE];

	int pos_X, pos_Y;
	int start_X, start_Y;
	int end_X, end_Y;
	int map_size;
	// tracks number of cells visited, start at 1 for START cell
	// every time a target cell is chosen and the wall between current cell
	// and target cell is erased, num_visited_cells is incremented
	int num_visited_cells;

	// stores the pos for X and Y
	// when dad end is reached, pop X and Y off and check
	// for a traversable cell
	// repeat until maze is generated
	stack<int> traversed_X, traversed_Y;

	// stores the cheat path positoins for X and Y
	// using different stacks so that we can store
	// the odd and even coordinates without changing the
	// maze generation, using traversed_X and traversed_Y
	// marks only the odd coordinates as cheat_path
	stack<int> cheat_X, cheat_Y;

	// moves pos_X and pos_Y to the target cell north/south/west/east of
	// the current cell
	void move_north();
	void move_south();
	void move_west();
	void move_east();
public:
	generate_maze();
	generate_maze(int size) {
		start_X = 0;
		start_Y = 0;
		pos_Y = 0;
		pos_X = 0;
		end_X = 0;
		end_Y = 0;
		map_size = size;
		num_visited_cells = 1;
	}

	// initializes the maze, sets every cell as WALL with restrictions on
	// the walls bordering edges
	void init_map();
	// begins at start pos and clears walls until the maze is finished
	// marks the last target cell as END
	void generate_map();
	// prints the maze in the console
	void print_map();
	// sets the path generated from START to END as different cell_types
	// this will usually not be the shortest route since we are clearing the
	// walls around the start position to give the user more choices to move
	// in the beginning, this is done after the maze is generated and the
	// cheat path is stored
	void cheat_mode();

	// getters for start coords
	int get_start_x() {return start_X;}
	int get_start_y() {return start_Y;}
	// getters for end coords
	int get_end_x() {return end_X;}
	int get_end_y() {return end_Y;}

	// setters for start and end coords
	void set_start_pos(int Y, int X) {start_Y = Y; start_X = X;}
	void set_end_pos(int Y, int X) {end_Y = Y; end_X = X;}

	// returns what is in the square
	int get_square(int y, int x) {return map[y][x].cell_type;}

	// tests if cell is Wall at given y,x coords
	bool is_wall(int y, int x) {return map[y][x].cell_type == WALL;}
	// tests if cell is maze End at given y,x coords
	bool is_end(int y, int x) {return map[y][x].cell_type == END;}
	// tests if cell is maze Start at given y,x coords
	bool is_start(int y, int x) {return map[y][x].cell_type == START;}
	// tests if cell is empty at given y,x coords
	bool is_empty(int y, int x) {return map[y][x].cell_type == EMPTY;}

	// used to test if a valid target cell is north/south/west/east of the
	// current cell
	bool test_north(int pos_Y, int pos_X);
	bool test_south(int pos_Y, int pos_X);
	bool test_west(int pos_Y, int pos_X);
	bool test_east(int pos_Y, int pos_X);

	// clears the walls around the START position that do not give away the
	// END position. If clearing a wall exposes the END then the wall will stay
	void clear_start_walls();
};

#endif
