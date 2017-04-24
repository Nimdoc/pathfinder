# Begin Makefile

raycast: main.o ray_window.o wall_object.o ray_caster.o maze_generator.o menu.o
	g++ -o pathfinder main.o ray_window.o wall_object.o \
	ray_caster.o maze_generator.o menu.o -std=c++14 -lX11

main.o:	wall_object.o
	g++ -c main.cpp -std=c++14

ray_window.o: wall_object.o
	g++ -c ray_window.cpp -std=c++14 -lX11

ray_caster.o: wall_object.o maze_generator.o
	g++ -c ray_caster.cpp -std=c++14

wall_object.o:
	g++ -c wall_object.cpp -std=c++14

maze_generator.o:
	g++ -c maze_generator.cpp -std=c++14

menu.o: ray_window.o
	g++ -c menu.cpp -std=c++14

# End of Makefile
