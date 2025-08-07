#include<SFML/Graphics.hpp>
#include<vector>
class Window_manage
{
private:
	unsigned int framerate;
public:
	sf::RenderWindow* window_manage;
	unsigned int map_unit;
	Window_manage(u_int16_t h, u_int16_t w, std::string title);
	void close_window();
	void change_framerate(unsigned int x);
};

