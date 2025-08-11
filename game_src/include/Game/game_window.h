#include"core.h"

class game_window{
	private:
	u_int32_t framerate;
	Window_manage* twoD;
	Window_manage* ThreeD;
	sf::RectangleShape player;
	std::vector<sf::CircleShape> Players;
    float player_angle_yaw ;             // horizontal view
	float player_angle_pitch ;           // vertical view
	float mouse_sensitivity ;		   // sensitivity
	bool run_game;
	bool game_paused;
	sf::VertexArray main_line;
	std::vector<int> players_dir;
	sf::WindowHandle twod_handle,threeD_handle;
	bool player_moved;
	public:
	game_window(std::string title="New game");
	std::pair<sf::WindowHandle,sf::WindowHandle> get_handles();
    void close_game();
	// void change_framerate(u_int32_t x);
    void game_loop();
    void manage_events();
	void clear_entities();
	void draw_entities();
	void manage_mouse(sf::Vector2i window_center);
	bool Movement(float& direction,sf::Vector2f& player_pos,const sf::Vector2f& player_size);
	void draw_3d_walls(sf::VertexArray& line, float player_angle_yaw,float diff);
    void draw_2d_walls();
	float manage_players(float player_yaw);
};