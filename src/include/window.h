#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
class Window
{
public:
	sf::RenderWindow* window;
	unsigned int framerate;
	unsigned int map_unit;
	Window(int h, int w, std::string title);
	void change_framerate(unsigned int x);
	void close_window();
};

