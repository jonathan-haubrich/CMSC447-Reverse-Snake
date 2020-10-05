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
			tmp = tmp->_next;
		}
	}

	void move()
	{
		int direction = rand() % 4;
		sf::Vector2f newPosition = _head->_segment.getPosition();
		SnakeNode *segment = nullptr;

		// Move the head
		switch(direction)
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
		segment = new SnakeNode(newPosition);
		segment->_next = _head;
		_head = segment;

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
				snake.move();
			}

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