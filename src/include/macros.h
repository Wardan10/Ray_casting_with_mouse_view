#pragma once
#define debug_n(x) std::cout<<#x<<":"<<x<<std::endl;
#define debug_t(x) std::cout<<#x<<":"<<x<<" ";
#define PI 3.141592
#define PI2 PI/2
#define PI3 3*PI/2
#define SCF(x) static_cast<float>(x)
#define SCI(x) static_cast<int>(x)
int map[10][10] = {
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