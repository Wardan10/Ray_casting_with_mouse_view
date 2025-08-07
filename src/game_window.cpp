#include"game_window.h"

game_window::game_window(std::string title){
    player_angle_yaw=5.0f ;             // horizontal view
	player_angle_pitch=0.0f;            // vertical view
	mouse_sensitivity=0.001;		    // sensitivity
	run_game=true;
	game_paused=false;
	main_line = sf::VertexArray(sf::Lines, 2); 
    twoD = new Window_manage(HEIGHT,WIDTH,title);
	ThreeD= new Window_manage(HEIGHT,WIDTH+2,title);
    ThreeD->window_manage->setMouseCursorVisible(false);
	// speed = sqrt(400 / map_unit);
	player.setFillColor(GREEN);
	player.setPosition({SCF(map_unit),SCF(map_unit) });
	player.setSize({ SCF(map_unit / 5),SCF(map_unit / 5) });
	sf::CircleShape test_object;
	test_object.setRadius(SCF(map_unit)/10);
	// player2.setRadius(SCF(map_unit)/10);
	test_object.setPosition({SCF(5*map_unit),SCF(5*map_unit)});
	// player2.setPosition({SCF(8*map_unit),SCF(5*map_unit)});
	test_object.setFillColor(BLUE);
	// player2.setFillColor(BLUE);
	Players.push_back(test_object);
	// Players.push_back(player2);
	players_dir.resize(2,1);
	twod_handle = twoD->window_manage->getSystemHandle();
	threeD_handle = ThreeD->window_manage->getSystemHandle();
}


/// @brief render other players
float game_window::manage_players(float player_yaw){
	sf::Vector2f player_pos=player.getPosition();
	float player_len=1e5;
	for(auto i:Players){
		sf::Vector2f curr_player_pos=i.getPosition();
		float player_size=i.getRadius();
		float angle=angleFromAToB(player_pos.x,player_pos.y,curr_player_pos.x,curr_player_pos.y);
		float angle_diff=angle-player_yaw;
		float distance=dist(player_pos.x,player_pos.y,curr_player_pos.x,curr_player_pos.y);
		if(abs(angle_diff)>PI/3)continue;
		float len_diff=abs(tan(angle_diff)*distance);
		if(len_diff>player_size)continue;
		player_len=std::min(SCF(distance/cos(angle_diff)),player_len);
	}
	return player_len;
}

/// @brief Main game loop using 2 elements from the window class for 2d and 3d view
void game_window::game_loop(){
    sf::Vector2i window_manage_center((ThreeD->window_manage->getSize().x) / 2, (ThreeD->window_manage->getSize().y) / 2);
    draw_2d_walls();
    while(run_game){
		// events
		if(!ThreeD->window_manage->hasFocus())continue;
        manage_events();
		if (!run_game)break;
		else if (game_paused)continue;
		// player handling
		sf::Vector2f player_pos=player.getPosition();
		sf::Vector2f player_size=player.getSize();
		float direction = player_angle_yaw;
		clear_entities();// clear entities
		Movement(direction,player_pos,player_size); // move the player
		move_players_test();
		// Update map
		ThreeD->window_manage->clear();	
		manage_mouse(window_manage_center);
		// Drawing lines
		main_line[0].position = { player_pos.x + player_size.x / 2, player_pos.y + player_size.y / 2 }; // Starting point
		draw_3d_walls(main_line,player_angle_yaw,0);
		// draw entities:
		draw_entities();
		float left_yaw = player_angle_yaw;
		float right_yaw = player_angle_yaw;
		uint32_t ray_count = 200;
		sf::VertexArray line(sf::Lines,2);
		line[0]=main_line[0];
		for (uint32_t i = 1; i <= ray_count; i++) {
			left_yaw += PI / SCF(6 * ray_count);
			if (left_yaw > 2 * PI)left_yaw -= 2 * PI;
			// handling the line!
			draw_3d_walls(line,left_yaw,-SCF(2*i));
		}
		for (uint32_t i = 0; i < ray_count; i++) {
			right_yaw -= PI / SCF(6 * ray_count);
			if (right_yaw < 0)right_yaw += 2 * PI;
			// handling the line!
			float wall_width=WIDTH/200;
			draw_3d_walls(line,right_yaw,2*i+wall_width/2);
		}
		// display the screen!
		twoD->window_manage->display();
		ThreeD->window_manage->display();
	}
}
/// @brief Manage window events for both 2D and 3D
void game_window::manage_events(){
    sf::Event Ev;
    sf::Vector2i window_manage_center((ThreeD->window_manage->getSize().x) / 2, (ThreeD->window_manage->getSize().y) / 2);
    while (ThreeD->window_manage->pollEvent(Ev)) {
        if (Ev.type == sf::Event::Closed) {
            run_game = false;
            break;
        }
        else if (Ev.type == sf::Event::KeyPressed && Ev.key.code == sf::Keyboard::Escape) {
            game_paused ^= true;
            ThreeD->window_manage->setMouseCursorVisible(game_paused);
            break;
        }
        else if (Ev.type == sf::Event::Resized) {
            map_unit = fmin(Ev.size.height, Ev.size.width) / 10;
            speed = sqrt(900 / map_unit);
            debug_t(Ev.size.height); 
            debug_n(Ev.size.width)
            debug_n(map_unit)
            sf::Vector2f old_player_pos = player.getPosition();
            player.setPosition({
                old_player_pos.x / twoD->map_unit * map_unit,
                old_player_pos.y / twoD->map_unit * map_unit
            });
            player.setSize({ static_cast<float>(map_unit / 5),static_cast<float>(map_unit / 5) });
            window_manage_center.x = (ThreeD->window_manage->getSize().x) / 2; window_manage_center.y = (ThreeD->window_manage->getSize().y) / 2;
        }
        else{
            //pass
        }
    }
    while (twoD->window_manage->pollEvent(Ev)) {
        //pass
    }
}

void game_window::manage_mouse(sf::Vector2i window_manage_center){
	sf::Vector2i Mouse_pos = sf::Mouse::getPosition(*ThreeD->window_manage);
	int32_t dx = Mouse_pos.x - window_manage_center.x;
	player_angle_yaw -= mouse_sensitivity * dx;
	if (player_angle_yaw > 2 * PI) player_angle_yaw -= 2 * PI;
	else if (player_angle_yaw < 0) player_angle_yaw += 2 * PI;
	sf::Mouse::setPosition(window_manage_center, *ThreeD->window_manage);
}

/// @brief draw the walls of the 2d space defined in the map
void game_window::draw_2d_walls(){
    sf::RectangleShape walls;
    walls.setSize({ static_cast<float>(map_unit),static_cast<float>(map_unit) });
    //debug_n(map_unit)
    walls.setFillColor({ RED });
    for (uint32_t i = 0; i < 10; i++) {
        for (uint32_t j = 0; j < 10; j++) {
            if (map[i][j] == 1) {
                walls.setPosition({ static_cast<float>(j * map_unit),static_cast<float>(i * map_unit) });
                twoD->window_manage->draw(walls);
            }
        }
    }
}

/// @brief Clear dynamic objects such as players and initial ray
void game_window::clear_entities(){
	main_line[0].color=BLACK;
	main_line[1].color=BLACK;
	twoD->window_manage->draw(main_line);
	player.setFillColor(BLACK);
	twoD->window_manage->draw(player);
	for(auto i:Players){
		i.setFillColor(BLACK);
		twoD->window_manage->draw(i);
	}
}

/// @brief draw dynamic objects such as players and initial ray
void game_window::draw_entities(){
	main_line[0].color = BLUE;
	main_line[1].color = BLUE;
	twoD->window_manage->draw(main_line);
	player.setFillColor(GREEN);
	twoD->window_manage->draw(player);
	for(auto i:Players){
		twoD->window_manage->draw(i);
	}
}

void game_window::Movement(float& direction,sf::Vector2f& player_pos,const sf::Vector2f& player_size){
	bool move = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))move = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))direction += PI2, move = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))direction += PI, move = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))direction += PI3, move = true;
	if (move) {
		float delta_x=SCF(speed * cos(direction));
		float delta_y=-SCF(speed * sin(direction));
		sf::Vector2f final_pos({player_pos.x + delta_x, player_pos.y +delta_y});
		bool can_player_move=true;
		if(inside_map(final_pos,map_unit,player_size)){
			player_pos=final_pos;
			can_player_move=false;
		}
		final_pos = { player_pos.x + delta_x,player_pos.y };
		if(can_player_move && inside_map(final_pos,map_unit,player_size)){
			player_pos=final_pos;
			can_player_move=false;
		}
		final_pos = {player_pos.x,player_pos.y +delta_y};
		if(can_player_move&& inside_map(final_pos,map_unit,player_size)){
			player_pos=final_pos;
			can_player_move=false;
		}
		if(!can_player_move)player.setPosition(player_pos);
	}
}

void game_window::close_game(){
    twoD->close_window();
    ThreeD->close_window();
}

void game_window::draw_3d_walls(sf::VertexArray& line, float player_angle_yaw,float diff){
	auto x = find_end({ line[0].position.x,line[0].position.y }, player_angle_yaw, map_unit);
	line[1].position = x.second;
	// first wall can_player_move:
	float len = x.first.first;
	float player_len=manage_players(player_angle_yaw);
	bool player_visible=true;
	if(player_len>len)player_visible=false;
	float height = SCF(map_unit)*200 / len;
	sf::RectangleShape wall;
	float wall_width=WIDTH/200;
	wall.setPosition({ SCF(WIDTH)/2+diff,SCF(HEIGHT- height)/2});
	wall.setSize({ wall_width,height });
	if (x.first.second)wall.setFillColor(sf::Color(255, 0, 0, 128));
	else wall.setFillColor(sf::Color(255, 0, 0));
	ThreeD->window_manage->draw(wall);
	if(player_visible){
		height=SCF(map_unit*200)/player_len;
		wall.setPosition({ SCF(WIDTH)/2+diff,SCF(HEIGHT- height)/2});
		wall.setSize({ wall_width,height });
		wall.setFillColor(BLUE);
		ThreeD->window_manage->draw(wall);
		// sf::Vector2f new_end;
		// new_end.x=line[0].position.x+player_len*cos(player_angle_yaw);
		// new_end.y=line[9].position.y-player_len*sin(player_angle_yaw);
		// line[1].position=new_end;
	}
}

void game_window::move_players_test(){
	int len=Players.size();
	for(int j=0;j<len;j++){
		auto& i=Players[j];
		auto pos=i.getPosition();
		sf::Vector2f next_pos({pos.x+speed*players_dir[j],pos.y});
		float radius=i.getRadius();
		if(inside_map(next_pos,map_unit,{2*radius,2*radius})){
			i.setPosition(next_pos);
		}
		else players_dir[j]=-players_dir[j];
	}
}


std::pair<sf::WindowHandle,sf::WindowHandle> game_window::get_handles(){
	return {twod_handle,threeD_handle};
}
