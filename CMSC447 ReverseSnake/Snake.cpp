#include "Snake.hpp"

Snake::Snake()
{
	_head = _tail = new SnakeNode;
	_head->_segment.setPosition({ 320.f, 320.f });
};
	
Snake::Snake(sf::Vector2f startingPosition) {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition(startingPosition);
	}
	
Snake::~Snake()
	{
		SnakeNode *tmp;
		while (_head)
		{
			tmp = _head->_next;
			delete _head;
			_head = tmp;
		}
	}

void Snake::draw(sf::RenderWindow &window, sf::Texture snakeHead, sf::Texture snakeBody)
{
	SnakeNode *tmp = _head;
	tmp->_segment.setTexture(snakeHead);
	while (tmp)
	{
		window.draw(tmp->_segment);
		if (tmp->_next == nullptr) {
			break;
		}
		tmp = tmp->_next;
		tmp->_segment.setTexture(snakeBody);
	}
}

void Snake::grow()
{
	_grow = true;
}

void Snake::move(sf::Vector2f targetPosition)
{
	int direction = 0;
	int targetX = 0;
	int targetY = 0;
	sf::Vector2f oldPosition = _head->_segment.getPosition(),
		newPosition = oldPosition,
		oldTail = _tail->_segment.getPosition(),
		tempPosition;
	targetX = abs(targetPosition.x - oldPosition.x);
	targetY = abs(targetPosition.y - oldPosition.y);
	if (targetX >= targetY)
	{
		if (targetPosition.x < oldPosition.x)
		{
			direction = 2;
		}
		else {
			direction = 3;
		}
	}
	else if (targetY >= targetX)
	{
		if (targetPosition.y < oldPosition.y)
		{
			direction = 0;
		}
		else
		{
			direction = 1;
		}
	}
	SnakeNode *tmp = nullptr;
	std::vector<sf::Vector2f> validMoves;

	// Move the head
	newPosition = oldPosition;
	switch (direction)
	{
	case 0: // Up
		newPosition.y -= MOVE_DISTANCE; // (y axis is flipped. it's weird)
		break;
	case 1: // Down
		newPosition.y += MOVE_DISTANCE;
		break;
	case 2: // Left
		newPosition.x -= MOVE_DISTANCE;
		break;
	case 3: // Right
		newPosition.x += MOVE_DISTANCE;
		break;
	}
	if (positionIsValid(newPosition))
	{
		validMoves.push_back(newPosition);
	}
	/*
	if (validMoves.size() == 0)
	{
		// Swap head and tail
		tmp = _tail;
		_tail->_next = _head->_next;
		_head->_next->_prev = _tail;
		_head->_next = nullptr;
		_head->_prev = _tail->_prev;
		_tail->_prev->_next = _head;
		_tail->_prev = nullptr;

		_tail = _head;
		_head = tmp;

		oldPosition = _head->_segment.getPosition();
		targetX = abs(targetPosition.x - oldPosition.x);
		targetY = abs(targetPosition.y - oldPosition.y);

		if (targetX > targetY)
		{
			if (targetPosition.x < oldPosition.x)
			{
				direction = 2;
			}
			else {
				direction = 3;
			}
		}
		else if (targetY >= targetX)
		{
			if (targetPosition.y < oldPosition.y)
			{
				direction = 0;
			}
			else
			{
				direction = 1;
			}
		}

		newPosition = oldPosition;
		switch (direction)
		{
		case 0: // Up
			newPosition.y -= MOVE_DISTANCE; // (y axis is flipped. it's weird)
			break;
		case 1: // Down
			newPosition.y += MOVE_DISTANCE;
			break;
		case 2: // Left
			newPosition.x -= MOVE_DISTANCE;
			break;
		case 3: // Right
			newPosition.x += MOVE_DISTANCE;
			break;
		}
		if (positionIsValid(newPosition))
		{
			validMoves.push_back(newPosition);
		}
	}
	*/

	//_head->_segment.setPosition(validMoves.at(rand() % validMoves.size()));
	_head->_segment.setPosition(newPosition);

	// Move the rest of the body
	tmp = _head->_next;
	while (tmp)
	{
		tempPosition = tmp->_segment.getPosition();
		tmp->_segment.setPosition(oldPosition);
		oldPosition = tempPosition;
		tmp = tmp->_next;
	}

	if (_grow)
	{
		_tail->_next = new SnakeNode(oldTail);
		_tail->_next->_prev = _tail;
		_tail = _tail->_next;
		_grow = false;
	}
}

sf::Vector2f Snake::getHead()
{
	return _head->_segment.getPosition();
}

int Snake::getLength()
{
	int length = 1;
	SnakeNode *tmp = _head;
	while (tmp)
	{
		length += 1;
		tmp = tmp->_next;
	}
	return length;
}

bool Snake::checkCollision(sf::RectangleShape const &shape)
{
	SnakeNode *tmp = _head;
	while (tmp)
	{
		if (tmp->_segment.getGlobalBounds().intersects(shape.getGlobalBounds()))
		{
			return true;
		}
		tmp = tmp->_next;
	}
	return false;
}

bool Snake::isSelfIntersecting()
{
	SnakeNode *tmp = _head->_next;
	while (tmp)
	{
		if (_head->_segment.getGlobalBounds().intersects(tmp->_segment.getGlobalBounds()))
		{
			return true;
		}
		tmp = tmp->_next;
	}
	return false;
}

bool Snake::checkSnakeCollision(Snake* otherSnake)
{
	SnakeNode *tmp = _head;
	SnakeNode *otherTmp = otherSnake->_head;
	while (tmp)
	{
		while (otherTmp)
		{
			if (tmp->_segment.getGlobalBounds().intersects(otherTmp->_segment.getGlobalBounds()))
			{
				return true;
			}
			otherTmp = otherTmp->_next;
		}
		tmp = tmp->_next;
	}
	return false;
}


bool Snake::positionIsValid(sf::Vector2f &pos)
{
	SnakeNode *tmp = _head;
	while (tmp)
	{
		if (tmp->_segment.getPosition() == pos)
		{
			return false;
		}
		tmp = tmp->_next;
	}
	return true;
}