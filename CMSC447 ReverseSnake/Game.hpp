#pragma once

#include <array>

#include "Snake.hpp"

void startScreen(sf::RenderWindow &window);
void leaderboard(sf::RenderWindow &window);
void gameLoop(sf::RenderWindow &window);
void endScreen(sf::Text scoreText, int score, sf::RenderWindow &window);
void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection);