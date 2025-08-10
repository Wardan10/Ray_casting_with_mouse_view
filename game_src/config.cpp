#include"config.h"
#include<iostream>
#include<math.h>
int map[10][10]={
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,1,0,0,0,1},
	{1,0,1,0,0,0,0,1,1,1},
	{1,0,1,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,0,1,1,1,1},
	{1,0,1,0,1,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1},
};
int HEIGHT=800;
int WIDTH=800;
// width to be multiple of 200, in multiple of ray count 
int map_unit=std::min(HEIGHT,WIDTH)/10;
// int map_unit=80
int speed = std::sqrt(900/map_unit);
