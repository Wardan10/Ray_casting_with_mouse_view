#include"game_window.h"
void Initialise_environment(std::pair<sf::WindowHandle,sf::WindowHandle> handles){
	#ifdef _WIN32
	HWND handle2D = handles.first;
	SetWindowPos(handle2D, HWND_TOP, 100, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Window Three_D(800, 802, "3D view");
	HWND handle3D = handles.second;
	SetWindowPos(handle3D, HWND_TOP, 1000, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	#elif defined(__linux__)
	::Window handle2D = handles.first;
	::Window handle3D = handles.second;
    Display* display;
	display = XOpenDisplay(NULL);
	if (display == NULL) {
		std::cerr << "Failed to open display!" << std::endl;
		return;
	}
	XMoveWindow(display,handle2D, 100, 100);
	XMoveWindow(display,handle3D, 1000, 100);
	XFlush(display);
	#endif
}

int main(){
    game_window main_game("RAYCASTING");
    // std::pair<sf::WindowHandle,sf::WindowHandle> handles=main_game.get_handles();
    Initialise_environment(main_game.get_handles());
    main_game.game_loop();
    std::cout<<"Exited game loop..Clearning resources"<<std::endl;
    main_game.close_game();
    return 0;
}
