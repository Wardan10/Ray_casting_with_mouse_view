#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#define debug_n(x) std::cout<<x<<std::endl;
#define debug_t(x) std::cout<<x<<" ";
#define PI 3.141592
int main(){
	sf::RenderWindow* window;
	sf::Event Ev;
	sf::VideoMode video;
	video.height=1000;
	video.width=1000;
	window = new sf::RenderWindow(video, "Ray casting!", sf::Style::Default);
	window->setFramerateLimit(60);
	window->setMouseCursorVisible(false);
	float player_angle_yaw = 0.0f;             // horizontal view
	float player_angle_pitch = 0.0f;           // vertical view
	float mouse_sensitivity = 0.001;		   // sensitivity
	const sf::Vector2i Window_center((window->getSize().x) / 2, (window->getSize().y) / 2);
	bool game = true;
	int speed = 3;
	sf::RectangleShape player;
 	player.setFillColor(sf::Color::Green);
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
	player.setPosition({ 100,100 });
	player.setSize({ 20,20 });
	while(game){
		// key management:
		while (window->pollEvent(Ev)) {
			if (Ev.type == sf::Event::Closed) {
				window->close();
				game = false;
				break;
			}
			else if (Ev.type == sf::Event::KeyPressed && Ev.key.code == sf::Keyboard::Escape) {
				window->close();
				game = false;
				break;
			}
		}
		sf::Vector2f player_pos = player.getPosition();
		// Movement: 
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			if (map[static_cast<int>(player_pos.y + speed * -1.f) / 100][static_cast<int>(player_pos.x) / 100] == 0
				&& map[static_cast<int>(player_pos.y + speed * -1.f) / 100][static_cast<int>(player_pos.x + player.getSize().x) / 100] == 0) {
				player.move({ 0.f, speed * -1.f });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			if (map[static_cast<int>(player_pos.y) / 100][static_cast<int>(player_pos.x + speed * -1.f) / 100] == 0
				&& map[static_cast<int>(player_pos.y + player.getSize().y) / 100][static_cast<int>(player_pos.x + speed * -1.f) / 100] == 0) {
				player.move({ speed * -1.f, 0.f });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			if (map[static_cast<int>(player_pos.y + speed * 1.f + player.getSize().y)/100][static_cast<int>(player_pos.x) / 100] == 0
				&& map[static_cast<int>(player_pos.y + speed * 1.f + player.getSize().y) / 100][static_cast<int>(player_pos.x + player.getSize().x) / 100] == 0){
					player.move({ 0.f, speed * 1.f });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			if (map[static_cast<int>(player_pos.y) / 100][static_cast<int>(player_pos.x + speed * 1.f +player.getSize().x) / 100] == 0
				&& map[static_cast<int>(player_pos.y +player.getSize().y)/ 100 ][static_cast<int>(player_pos.x + speed * 1.f + player.getSize().x) / 100] == 0) {
				player.move({ speed * 1.f, 0.f });
			}
		}
		// Update Map:
		window->clear();
		sf::RectangleShape walls;
		walls.setSize({ 100,100});
		walls.setFillColor({ sf::Color::Red});
		for (int i = 0; i < 10; i++){
			for (int j = 0; j < 10; j++) {
				if (map[i][j] == 1) {
					walls.setPosition({static_cast<float>(j * 100),static_cast<float>(i * 100) });
					window->draw(walls);
				}
			}
		}
		// draw player
		window->draw(player);
		// Updating angle:
		sf::Vector2i Mouse_pos=sf::Mouse::getPosition(*window);
		float Diff = mouse_sensitivity * (Mouse_pos.x - Window_center.x);
		player_angle_yaw -= Diff;
		if (player_angle_yaw > 2 * PI)player_angle_yaw -= 2 * PI;
		else if (player_angle_yaw < 0)player_angle_yaw += 2 * PI;
		sf::Mouse::setPosition(Window_center,*window);
		// Drawing lines
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = { player_pos.x + player.getSize().x / 2, player_pos.y + player.getSize().y / 2 }; // Starting point
		line[0].color = sf::Color::Blue;
		line[1].color = sf::Color::Blue;
		int line_length = 100;
		line[1].position = { line[0].position.x + line_length * cosf(player_angle_yaw),line[0].position.y - line_length * sinf(player_angle_yaw)};
		window->draw(line);
		float left_yaw= player_angle_yaw;
		float right_yaw = player_angle_yaw;
		int ray_count = 50;
		for (int i = 0; i < ray_count; i++) {
			left_yaw += PI / (6*ray_count);
			if (left_yaw > 2 * PI)left_yaw -= 2 * PI;
			// handling the line!
			line[1].position = { line[0].position.x + line_length * cosf(left_yaw),line[0].position.y - line_length * sinf(left_yaw) };// Ending point
			window->draw(line);
		}
		for (int i = 0; i < ray_count; i++) {
			right_yaw -= PI / (6*ray_count);
			if(right_yaw < 0)right_yaw += 2 * PI;
			// handling the line!
			line[1].position = { line[0].position.x + line_length * cosf(right_yaw),line[0].position.y - line_length * sinf(right_yaw) };// Ending point
			window->draw(line);
		}
		// display the screen!
		window->display();
	}
	delete window;
	return 0;
}
// handling main line:
/*int line_length, mx, my, mp; float rx, ry, ra, xo, yo;
int count = 0;
float aTan = -1 / tan(player_angle_yaw);
ra = player_angle_yaw;
if(ra>PI){
	ry = static_cast<int>((line[0].position.y/ 100) * 100)+100;
	rx = (line[0].position.y - ry) * aTan + line[0].position.x;
	yo = 100;
	xo = -yo * aTan;
}
if (ra < PI) {
	ry = static_cast<int>((line[0].position.y / 100) * 100)-1;
	rx = (line[0].position.y - ry) * aTan + line[0].position.x;
	yo = 100;
	xo = -yo * aTan;
}
if (ra == 0 || ra == PI) {
	rx = line[0].position.x;
	ry = line[0].position.y;
	count = 10;
}
while (count < 10) {
	mx = rx/ 100;
	my = ry/ 100;
	if (mx*10+my<100 && map[mx][my] == 1)count = 10;
	else {
		rx += xo;
		ry += yo;
		count++;
	}
}
line[1].position = { rx,ry };*/