#include"game_window.h"
#include"environment.h"
#include"network.h"

int main(int argc, char* argv[]){
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <send_shm_name> <recv_shm_name>\n";
        return 1;
    }
    std::string send_name = argv[1];
    std::string recv_name = argv[2];
    game_window main_game("RAYCASTING");
    std::pair<sf::WindowHandle,sf::WindowHandle> handles=main_game.get_handles();
    Initialise_environment(handles);
    Network network;
    std::cout<<"Shared memory initialised with "<<send_name<<" "<<recv_name<<std::endl;
    network.initialise_connection(send_name,recv_name);
    main_game.game_loop();
    std::cout<<"Exited game loop..Clearning resources"<<std::endl;
    network.close_connection();
    main_game.close_game();
    return 0;
}
