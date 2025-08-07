#ifndef CONFIG_HPP
#define CONFIG_HPP
#define debug_n(x) std::cout<<#x<<":"<<x<<std::endl;
#define debug_t(x) std::cout<<#x<<":"<<x<<" ";
#define PI 3.141592
#define PI2 PI/2
#define PI3 3*PI/2
#define SCF(x) static_cast<float>(x)
#define SCI(x) static_cast<int>(x)
extern int map[10][10];
extern int map_unit;
extern int speed;
extern int player_size;
extern int HEIGHT;
extern int WIDTH;
// environment:
//--------------------------------------COLORS-------------------------------
#define GREEN sf::Color::Green
#define RED   sf::Color::Red
#define BLUE  sf::Color::Blue
#define BLACK sf::Color::Black

#endif
