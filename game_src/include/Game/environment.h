#include<utility>
#include<iostream>

#ifdef _WIN32
#include<window.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif

void Initialise_environment(std::pair<unsigned long long,unsigned long long> handles){
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
	
	#ifndef DEBUG_2D
    // Hide window right after creation
		#ifdef _WIN32
			ShowWindow(handle2D, SW_HIDE);
		#elif defined(__linux__)
			Display* dpy = XOpenDisplay(NULL);
			if (dpy) {
				XUnmapWindow(dpy, handle2D);
				XFlush(dpy);
				XCloseDisplay(dpy);
			}
		#endif
	#endif
	#ifndef DEBUG_3D
    // Hide window right after creation
		#ifdef _WIN32
			ShowWindow(handle3D, SW_HIDE);
		#elif defined(__linux__)
			Display* dpy = XOpenDisplay(NULL);
			if (dpy) {
				XUnmapWindow(dpy, handle3D);
				XFlush(dpy);
				XCloseDisplay(dpy);
			}
		#endif
	#endif
}