#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#define debug_n(x) std::cout<<#x<<":"<<x<<std::endl;
#define debug_t(x) std::cout<<#x<<":"<<x<<" ";
#define PI 3.141592
#define PI2 PI/2
#define PI3 3*PI/2
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
double dist(float ax, float ay, float bx, float by) {
	return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}
sf::Vector2f find_end(sf::Vector2f start, float player_angle_yaw) {
	int mx, my;
	float rx, ry, ra, xo, yo;
	int count = 0;
	//  Horizontal line // 
	float HOR = INT32_MAX;
	float aTan = -1 / tan(player_angle_yaw);
	ra = player_angle_yaw;
	if (ra == 0 || ra == PI) {
		rx = start.x;
		ry = start.y;
		count = 10;
	}
	if (ra != 0 && ra < PI) {
		ry = floor(start.y / 100) * 100 - 1;
		rx = (ry - start.y) * aTan + start.x;
		yo = -100;
		xo = yo * aTan;
	}
	if (ra > PI) {
		ry = floor(start.y / 100) * 100 + 100;
		rx = (ry - start.y) * aTan + start.x;
		yo = 100;
		xo = yo * aTan;
	}
	while (count < 10) {
		mx = floor(rx / 100);
		my = floor(ry / 100);
		if (mx >= 0 && my >= 0 && mx < 10 && my < 10 && map[my][mx] == 1) {
			count = 10;
			HOR = dist(start.x,start.y, rx, ry);
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
	float VER = INT32_MAX;
	count = 0;
	float Tan = -tan(player_angle_yaw);
	vra = player_angle_yaw;
	if (vra == 0 || vra == PI2) {
		vrx = start.x;
		vry = start.y;
		count = 10;
	}
	if (vra > PI2 || vra < PI3) {
		vrx = floor(start.x / 100) * 100 - 1;
		vry = (vrx - start.x) * Tan + start.y;
		vxo = -100;
		vyo = vxo * Tan;
	}
	if (vra <PI2 || vra>PI3) {
		vrx = floor(start.x / 100) * 100 + 100;
		vry = (vrx - start.x) * Tan + start.y;
		vxo = 100;
		vyo = vxo * Tan;
	}
	while (count < 10) {
		vmx = floor(vrx / 100);
		vmy = floor(vry / 100);
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
	if (HOR < VER) {
		vry = ry;
		vrx = rx;
	}
	return { vrx,vry };
}
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
		line[1].position = find_end({ line[0].position.x,line[0].position.y }, player_angle_yaw);
		line[0].color = sf::Color::Blue;
		line[1].color = sf::Color::Blue;
		window->draw(line);
		float left_yaw= player_angle_yaw;
		float right_yaw = player_angle_yaw;
		int ray_count = 50;
		int line_length = 100;
		for (int i = 0; i < ray_count; i++) {
			left_yaw += PI / (6*ray_count);
			if (left_yaw > 2 * PI)left_yaw -= 2 * PI;
			// handling the line!
			line[1].position = find_end({ line[0].position.x,line[0].position.y }, left_yaw);// Ending point
			window->draw(line);
		}
		for (int i = 0; i < ray_count; i++) {
			right_yaw -= PI / (6*ray_count);
			if(right_yaw < 0)right_yaw += 2 * PI;
			// handling the line!
			line[1].position =find_end({ line[0].position.x,line[0].position.y },right_yaw);// Ending point
			window->draw(line);
		}
		// display the screen!
		window->display();
	}
	delete window;
	return 0;
}
