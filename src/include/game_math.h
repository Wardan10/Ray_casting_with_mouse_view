#include<cmath>
#include<SFML/System.hpp>
double dist(float ax, float ay, float bx, float by);

bool inside_map(sf::Vector2f final_pos,int map_unit,sf::Vector2f player_size);

std::pair<std::pair<float, bool>, sf::Vector2f> find_end(sf::Vector2f start, float player_angle_yaw, int map_unit);

double angleFromAToB(float a_x, float a_y, float b_x, float b_y);

