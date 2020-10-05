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
		SnakeNode *segment = new SnakeNode(_head->_segment.getPosition());

		// Move the head
		switch(direction)
		{
		case 0: // Up
			segment->_segment.move(0, -MOVE_DISTANCE); // (y axis is flipped. it's weird)
			break;
		case 1: // Down
			segment->_segment.move(0, MOVE_DISTANCE);
			break;
		case 2: // Left
			segment->_segment.move(-MOVE_DISTANCE, 0);
			break;
		case 3: // Right
			segment->_segment.move(MOVE_DISTANCE, 0);
			break;
		}

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