#include "game_math.h"
#include <SFML/System.hpp>
#include "macros.h"
double dist(float ax, float ay, float bx, float by) {
	return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}
bool inside_map(sf::Vector2f final_pos,int map_unit,sf::Vector2f player_size){
	return( 
		map[SCI(final_pos.y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
		map[SCI(final_pos.y + player_size.y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
		map[SCI(final_pos.y) / map_unit][SCI(final_pos.x + player_size.x) / map_unit] == 0 &&
		map[SCI(final_pos.y + player_size.y) / map_unit][SCI(final_pos.x + player_size.x) / map_unit] == 0);
}
std::pair<std::pair<int, bool>, sf::Vector2f> find_end(sf::Vector2f start, float player_angle_yaw, int map_unit) {
	int mx, my;
	float rx, ry, ra, xo, yo;
	int count = 0;
	//  Horizontal line // 
	float HOR = __INT32_MAX__;
	float aTan = -1 / tan(player_angle_yaw);
	ra = player_angle_yaw;
	if (ra == 0 || ra == PI) {
		rx = start.x;
		ry = start.y;
		count = 10;
	}
	if (ra != 0 && ra < PI) {
		ry = floor(start.y / map_unit) * map_unit - 1;
		rx = (ry - start.y) * aTan + start.x;
		yo = -map_unit;
		xo = yo * aTan;
	}
	if (ra > PI) {
		ry = floor(start.y / map_unit) * map_unit + map_unit;
		rx = (ry - start.y) * aTan + start.x;
		yo = map_unit;
		xo = yo * aTan;
	}
	while (count < 10) {
		mx = floor(rx / map_unit);
		my = floor(ry / map_unit);
		if (mx >= 0 && my >= 0 && mx < 10 && my < 10 && map[my][mx] == 1) {
			count = 10;
			HOR = dist(start.x, start.y, rx, ry);
		}
		else {
			rx += xo;
			ry += yo;
			count++;
		}
	}
	// Vertical line // 
	int vmx, vmy;
	float vrx, vry, vra, vxo, vyo;
	float VER = __INT32_MAX__;
	count = 0;
	float Tan = -tan(player_angle_yaw);
	vra = player_angle_yaw;
	if (vra == 0 || vra == PI2) {
		vrx = start.x;
		vry = start.y;
		count = 10;
	}
	if (vra > PI2 || vra < PI3) {
		vrx = floor(start.x / map_unit) * map_unit - 1;
		vry = (vrx - start.x) * Tan + start.y;
		vxo = -map_unit;
		vyo = vxo * Tan;
	}
	if (vra <PI2 || vra>PI3) {
		vrx = floor(start.x / map_unit) * map_unit + map_unit;
		vry = (vrx - start.x) * Tan + start.y;
		vxo = map_unit;
		vyo = vxo * Tan;
	}
	while (count < 10) {
		vmx = floor(vrx / map_unit);
		vmy = floor(vry / map_unit);
		if (vmx >= 0 && vmy >= 0 && vmx < 10 && vmy < 10 && map[vmy][vmx] == 1) {
			count = 10;
			VER = dist(start.x, start.y, vrx, vry);
		}
		else {
			vrx += vxo;
			vry += vyo;
			count++;
		}
	}
	bool hor_ver = true;
	if (HOR < VER) {
		vry = ry;
		vrx = rx;
		hor_ver = false;
		VER = HOR;
	}
	return { {VER,hor_ver},{ vrx,vry} };
}