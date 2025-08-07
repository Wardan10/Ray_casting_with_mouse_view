#include<iostream>
#include<utility>
#ifdef _WIN32
#include<window.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif
#undef None
#undef Default
#include"config.h"
#include"window.h"
#include"game_math.h"
