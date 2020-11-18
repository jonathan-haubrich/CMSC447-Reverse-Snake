#pragma once

#include <SFML/Graphics.hpp>

#define	MOVE_DISTANCE	16

class Snake {
public:
	Snake();
	Snake(sf::Vector2f startingPosition);
	~Snake();

	void draw(sf::RenderWindow &window, sf::Texture snakeHead, sf::Texture snakeBody);

	void grow();

	void move(sf::Vector2f targetPosition);

	sf::Vector2f getHead();

	int getLength();

	bool checkCollision(sf::RectangleShape const &shape);

	bool isSelfIntersecting();

	bool checkSnakeCollision(Snake* otherSnake);

private:
	struct SnakeNode {
		SnakeNode() {}
		SnakeNode(sf::Vector2f pos) : SnakeNode() { _segment.setPosition(pos); }
		sf::Sprite _segment;
		SnakeNode *_prev{ nullptr },
			*_next{ nullptr };
	};

	bool positionIsValid(sf::Vector2f &pos);
	int _length{ 0 };
	SnakeNode *_head{ nullptr },
		*_tail{ nullptr };
	bool _grow{ false };
};