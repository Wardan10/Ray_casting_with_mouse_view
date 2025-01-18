#include<core.h>
std::pair<std::pair<int,bool>,sf::Vector2f> find_end(sf::Vector2f start, float player_angle_yaw, int map_unit) {
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
int main(){	
	Window two_D(800, 800,"2D view");
	sf::Event Ev;
	unsigned int map_unit = two_D.map_unit;
	two_D.window->setMouseCursorVisible(false);
	float player_angle_yaw = 0.0f;             // horizontal view
	float player_angle_pitch = 0.0f;           // vertical view
	float mouse_sensitivity = 0.001;		   // sensitivity
	sf::Vector2i Window_center((two_D.window->getSize().x) / 2, (two_D.window->getSize().y) / 2);
	std::cout << map_unit << std::endl;
	int speed = sqrt(900/map_unit);
	std::cout << speed << std::endl;
	sf::RectangleShape player;
 	player.setFillColor(sf::Color::Green);
	player.setPosition({ static_cast<float>(map_unit),static_cast<float>(map_unit)});
	player.setSize({ static_cast<float>(map_unit/5),static_cast<float>(map_unit/5) });
	bool game = true;
	bool game_paused = false;
	HWND handle2D = two_D.window->getSystemHandle();
	SetWindowPos(handle2D, HWND_TOP, 100, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	// 3D window:
	Window Three_D(800, 802, "3D view");
	HWND handle3D = Three_D.window->getSystemHandle();
	SetWindowPos(handle3D, HWND_TOP, 1000, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	while(game){
		if (!two_D.window->hasFocus() && !Three_D.window->hasFocus())continue; // if game tab is in background stall the game
		// key management:
		// 2D Window events:
		while (two_D.window->pollEvent(Ev)) {
			if (Ev.type == sf::Event::Closed) {
				game = false;
				break;
			}
			else if (Ev.type == sf::Event::KeyPressed && Ev.key.code == sf::Keyboard::Escape) {
				game_paused^=true;
				two_D.window->setMouseCursorVisible(game_paused);
				break;
			}
			else if (Ev.type == sf::Event::Resized) {
				// update 
				map_unit = fmin(Ev.size.height, Ev.size.width)/10;
				speed = sqrt(900/map_unit);
				debug_t(Ev.size.height);debug_n(Ev.size.width)
				debug_n(map_unit)
				sf::Vector2f old_player_pos = player.getPosition();
				player.setPosition({
					old_player_pos.x / two_D.map_unit * map_unit,
					old_player_pos.y / two_D.map_unit * map_unit
					});
				// set new position according to old postion:
				//player.setPosition({ static_cast<float>(map_unit),static_cast<float>(map_unit) });
				player.setSize({ static_cast<float>(map_unit / 5),static_cast<float>(map_unit / 5) });
				Window_center.x = (two_D.window->getSize().x) / 2; Window_center.y = (two_D.window->getSize().y) / 2;
			}
		}
		// 3D Window events:
		while (Three_D.window->pollEvent(Ev)) {

		}
		// 3D Window events:
		// empty:
		if (!game)break;
		else if (game_paused)continue;
		sf::Vector2f player_pos = player.getPosition();
		// Movement: 
		float direction = player_angle_yaw;
		bool move = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))move = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))direction += PI2, move = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))direction += PI, move = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))direction += PI3, move = true;
		if (move) {
			sf::Vector2f final_pos({ player_pos.x + static_cast<float>(speed * cos(direction)),
									 player_pos.y - static_cast<float>(speed * sin(direction)) });
			bool test = true;
			if (map[SCI(final_pos.y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0) {
				player.setPosition(final_pos);
				test = false;
			}
			final_pos = { player_pos.x + static_cast<float>(speed * cos(direction)),player_pos.y };
			if (test && map[SCI(final_pos.y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0) {
				player.setPosition(final_pos);
				test = false;
			}
			final_pos = {player_pos.x,player_pos.y - static_cast<float>(speed * sin(direction))};
			if (test && map[SCI(final_pos.y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x) / map_unit] == 0 &&
				map[SCI(final_pos.y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0 &&
				map[SCI(final_pos.y + player.getSize().y) / map_unit][SCI(final_pos.x + player.getSize().x) / map_unit] == 0) {
				player.setPosition(final_pos);
			}
		}
		// Update Map:
		two_D.window->clear();
		Three_D.window->clear();
		sf::RectangleShape walls;
		walls.setSize({ static_cast<float>(map_unit),static_cast<float>(map_unit)});
		//debug_n(map_unit)
		walls.setFillColor({ sf::Color::Red});
		for (int i = 0; i < 10; i++){
			for (int j = 0; j < 10; j++) {
				if (map[i][j] == 1) {
					walls.setPosition({static_cast<float>(j * map_unit),static_cast<float>(i * map_unit) });
					two_D.window->draw(walls);
				}
			}
		}
		// draw player
		// Updating angle:
		sf::Vector2i Mouse_pos=sf::Mouse::getPosition(*two_D.window);
		float Diff = mouse_sensitivity * (Mouse_pos.x - Window_center.x);
		player_angle_yaw -= Diff;
		if (player_angle_yaw > 2 * PI)player_angle_yaw -= 2 * PI;
		else if (player_angle_yaw < 0)player_angle_yaw += 2 * PI;
		sf::Mouse::setPosition(Window_center,*two_D.window);
		// Drawing lines
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = { player_pos.x + player.getSize().x / 2, player_pos.y + player.getSize().y / 2 }; // Starting point
		auto x=find_end({ line[0].position.x,line[0].position.y }, player_angle_yaw, map_unit);
		line[1].position = x.second;
		// first wall test:
		float len = x.first.first;
		float height = 16000 / len;
		sf::RectangleShape wall;
		wall.setSize({ 2,height});
		wall.setPosition({ 400,(800 - height) / 2});
		if (x.first.second)wall.setFillColor(sf::Color(255, 0, 0, 128));
		else wall.setFillColor(sf::Color(255, 0, 0));
		Three_D.window->draw(wall);

		line[0].color = sf::Color::Blue;
		line[1].color = sf::Color::Blue;
		two_D.window->draw(line);
		two_D.window->draw(player);
		float left_yaw= player_angle_yaw;
		float right_yaw = player_angle_yaw;
		int ray_count = 200;
		for (int i = 1; i <=ray_count; i++) {
			left_yaw += PI / (6*ray_count);
			if (left_yaw > 2 * PI)left_yaw -= 2 * PI;
			// handling the line!
			auto x=find_end({ line[0].position.x,line[0].position.y }, left_yaw,map_unit);// Ending point
			line[1].position = x.second;
			/*two_D.window->draw(line);*/
			// 
			// 3D window wall:
			float len = x.first.first;
			float height = 16000 / len;
			wall.setSize({ 4,height });
			wall.setPosition({static_cast<float>(400-2*i),(800 - height) / 2 });
			if(x.first.second)wall.setFillColor(sf::Color(255,0,0,128));
			else wall.setFillColor(sf::Color(255,0,0));
			Three_D.window->draw(wall);
		}
		for (int i = 0; i < ray_count; i++) {
			right_yaw -= PI / (6*ray_count);
			if(right_yaw < 0)right_yaw += 2 * PI;
			// handling the line!
			auto x=find_end({ line[0].position.x,line[0].position.y },right_yaw,map_unit);// Ending point
			line[1].position = x.second;
			//two_D.window->draw(line);

			// 3D window wall:
			float len = x.first.first;
			float height = 16000 / len;
			wall.setSize({ 4,height });
			wall.setPosition({ static_cast<float>(402 + 2 * i),(800 - height) / 2 });
			if (x.first.second)wall.setFillColor(sf::Color(255, 0, 0, 128));
			else wall.setFillColor(sf::Color(255, 0, 0));
			Three_D.window->draw(wall);
		}
		// display the screen!
		two_D.window->display();
		Three_D.window->display();
	}
	two_D.close_window();
	Three_D.close_window();
	return 0;
}
