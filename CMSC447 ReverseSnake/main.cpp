#include <iostream>
#include <fstream>  
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <Windows.h>
#include <time.h>

#include "Game.hpp"

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	sf::RenderWindow window(sf::VideoMode(640, 640), "Ekans");

	// start screen
	startScreen(window);
	return 0;
}

