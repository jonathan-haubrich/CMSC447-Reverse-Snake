#include <SFML/Graphics.hpp>

#define	MOVE_DISTANCE	10

void movePlayer(sf::RectangleShape &shape);

class Snake {
public:
	Snake() {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition({ 50.f, 50.f });
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
			tmp = _head->_next;
		}
	}

	void grow()
	{
		//SnakeNode* segment = new SnakeNode(_tail->_segment.getPosition());
		//if (_head == _tail)
		//{
		//	_head->_next = segment;
		//}
		//else
		//{
		//	_tail->_next = segment;
		//}
		//_tail = segment;
	}

	void move()
	{
		int direction = rand() % 4;
		sf::Vector2f oldPosition = _head->_segment.getPosition(),
			newPosition = oldPosition;

		// Move the head
		switch(direction)
		{
		case 0: // Up
			newPosition.y -= MOVE_DISTANCE; // (axes are flipped. it's weird)
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
		_head->_segment.setPosition(newPosition);

		// Move the rest of the body
		//SnakeNode *tmp = _head->_next;
		//while (tmp)
		//{
		//	newPosition = tmp->_segment.getPosition();
		//	tmp->_segment.setPosition(oldPosition);
		//	oldPosition = newPosition;
		//	tmp = tmp->_next;
		//}
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
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "Reverse Snake");
	sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
	shape.setFillColor(sf::Color::Green);
	Snake snake;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				movePlayer(shape);
				snake.grow();
				snake.move();
			}

		}

		window.clear();
		snake.draw(window);
		window.draw(shape);
		window.display();
		snake.grow();
	}

	return 0;
}

void movePlayer(sf::RectangleShape &shape)
{
	sf::Vector2f newPosition = shape.getPosition();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		newPosition.x -= MOVE_DISTANCE;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		newPosition.x += MOVE_DISTANCE;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		newPosition.y -= MOVE_DISTANCE;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		newPosition.y += MOVE_DISTANCE;
	}

	shape.setPosition(newPosition);
}