#include <SFML/Graphics.hpp>

#define	MOVE_DISTANCE	8

void movePlayer(sf::RectangleShape &shape);

class Snake {
public:
	Snake() {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition({ 200.f, 200.f });
	}
	~Snake()
	{
		SnakeNode *tmp;
		while(_head)
		{
			tmp = _head->_next;
			delete _head;
			_head = tmp;
		} 
	}

	void draw(sf::RenderWindow &window)
	{
		SnakeNode *tmp = _head;
		while (tmp)
		{
			window.draw(tmp->_segment);
			tmp = tmp->_next;
		}
	}

	void grow()
	{
		_grow = true;
	}

	void move()
	{
		int direction = rand() % 4;
		sf::Vector2f oldPosition = _head->_segment.getPosition(),
			oldTail = _tail->_segment.getPosition(),
			tempPosition;
		SnakeNode *tmp = _head->_next;

		// Move the head
		switch(direction)
		{
		case 0: // Up
			_head->_segment.move(0, -MOVE_DISTANCE); // (y axis is flipped. it's weird)
			break;
		case 1: // Down
			_head->_segment.move(0, MOVE_DISTANCE);
			break;
		case 2: // Left
			_head->_segment.move(-MOVE_DISTANCE, 0);
			break;
		case 3: // Right
			_head->_segment.move(MOVE_DISTANCE, 0);
			break;
		}

		// Move the rest of the body
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
			_tail = _tail->_next;
			_grow = false;
		}
	}

private:
	struct SnakeNode {
		SnakeNode() { _segment.setFillColor(sf::Color::Red); }
		SnakeNode(sf::Vector2f pos) : SnakeNode() { _segment.setPosition(pos);  }
		sf::RectangleShape _segment{ {10.f, 10.f} };
		SnakeNode *_next{ nullptr };
	};

	int _length{ 0 };
	SnakeNode *_head{ nullptr },
		*_tail{ nullptr };
	bool _grow{ false };
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "Reverse Snake");
	sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
	shape.setFillColor(sf::Color::Green);
	Snake snake;
	sf::Clock moveClock,
		growthClock;
	sf::Time moveTime,
		growthTime;
	while (window.isOpen())
	{
		window.setFramerateLimit(8);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				movePlayer(shape);

			}

		}

		growthTime = growthClock.getElapsedTime();
		if (growthTime.asSeconds() >= 3.f)
		{
			snake.grow();
			growthClock.restart();
		}
		moveTime = moveClock.getElapsedTime();
		if (moveTime.asSeconds() >= 0.3f)
		{
			snake.move();
			moveClock.restart();
		}

		window.clear();
		snake.draw(window);
		window.draw(shape);
		window.display();
	}

	return 0;
}

void movePlayer(sf::RectangleShape &shape)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		shape.move(-MOVE_DISTANCE, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		shape.move(MOVE_DISTANCE, 0 );
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		shape.move(0, -MOVE_DISTANCE);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		shape.move(0, MOVE_DISTANCE);
	}
}