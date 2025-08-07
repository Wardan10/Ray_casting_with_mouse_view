#include "window.h"
Window_manage::Window_manage(u_int16_t h,u_int16_t w,std::string title) {
	this->framerate = 60;
	sf::VideoMode video;
	video.height = h;
	video.width = h;
	map_unit = std::min(h, w)/10;
	this->window_manage = new sf::RenderWindow(video, title, sf::Style::Default);
	window_manage->setFramerateLimit(framerate);
}

void Window_manage::close_window() {
	if (this->window_manage->isOpen())window_manage->close();
	delete this->window_manage;
}

void Window_manage::change_framerate(unsigned int x) {
	framerate = x;
	this->window_manage->setFramerateLimit(x);
	return;
}
