#include "window.h"
Window::Window(int h,int w,std::string title) {
	this->framerate = 60;
	sf::VideoMode video;
	video.height = h;
	video.width = h;
	map_unit = fmin(h, w) / 10;
	this->window = new sf::RenderWindow(video, title, sf::Style::Default);
	window->setFramerateLimit(framerate);
}
void Window::close_window() {
	if (this->window->isOpen())window->close();
	delete this->window;
}
void Window::change_framerate(unsigned int x) {
	framerate = x;
	this->window->setFramerateLimit(x);
	return;
}