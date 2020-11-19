#pragma once
#pragma comment(lib, "sfml-audio-d.lib")

/* Internal includes */
#include "Snake.hpp"

/* System includes */
#include <array>
#include <shlobj_core.h>
#include <SFML/Audio.hpp>
#include <Shlwapi.h>
#include <vector>

/* External library includes */
#include <cpprest/json.h>

class Score {
public:
	Score(const int score, const std::string initials) : _score(score), _initials(initials) {}
	int score() { return _score; }
	std::string initials() { return _initials; }

private:
	int _score{ 0 };
	std::string _initials{ "AAA" };
};

void startScreen(sf::RenderWindow &window);
void leaderboard(std::vector<Score*> &highScores, sf::RenderWindow &window);
void gameLoop(std::vector<Score*> &highScores, sf::RenderWindow &window);
void endScreen(sf::Text scoreText, int score, std::vector<Score*> &highScores, sf::RenderWindow &window);
void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection);
void loadScores(std::vector<Score*> &scores);
void saveScores(std::vector<Score*> &scores);