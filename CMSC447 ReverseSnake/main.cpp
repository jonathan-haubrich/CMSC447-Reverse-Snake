#include <SFML/Graphics.hpp>
#include <Windows.h>

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
		int direction = 0;
		sf::Vector2f oldPosition = _head->_segment.getPosition(),
			newPosition = oldPosition,
			oldTail = _tail->_segment.getPosition(),
			tempPosition;
		SnakeNode *tmp = nullptr;
		std::vector<sf::Vector2f> validMoves;

		// Move the head
		for (int direction = 0; direction < 4; ++direction)
		{
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

			for (int direction = 0; direction < 4; ++direction)
			{
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
		}

		_head->_segment.setPosition(validMoves.at(rand() % validMoves.size()));

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

	bool checkCollision(sf::RectangleShape const &shape)
	{
		SnakeNode *tmp = _head;
		while (tmp)
		{
			if (tmp->_segment.getPosition() == shape.getPosition())
			{
				return true;
			}
			tmp = tmp->_next;
		}
		return false;
	}

private:
	struct SnakeNode {
		SnakeNode() { _segment.setFillColor(sf::Color::Red); }
		SnakeNode(sf::Vector2f pos) : SnakeNode() { _segment.setPosition(pos);  }
		sf::RectangleShape _segment{ {10.f, 10.f} };
		SnakeNode *_prev{ nullptr },
			*_next{ nullptr };
	};

	bool positionIsValid(sf::Vector2f &pos)
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

	int _length{ 0 };
	SnakeNode *_head{ nullptr },
		*_tail{ nullptr };
	bool _grow{ false };
};

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	sf::RenderWindow window(sf::VideoMode(400, 400), "Reverse Snake");
	sf::RectangleShape player(sf::Vector2f(10.f, 10.f));
	player.setFillColor(sf::Color::Green);
	Snake snake;
	sf::Clock moveClock,
		growthClock;
	sf::Time moveTime,
		growthTime;
	sf::Event event;
	
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("Press any key to continue...", font, 16);
	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);


	bool collided = false;

	while (window.isOpen() && window.waitEvent(event))
	{
		window.clear();
		window.draw(text);
		window.display();
		if (event.type == sf::Event::KeyPressed)
		{
			break;
		}
	}

	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				movePlayer(player);
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

		if (snake.checkCollision(player))
		{
			collided = true;
			break;
		}

		window.clear();
		snake.draw(window);
		window.draw(player);
		window.display();
	}

	if (collided)
	{
		sf::Text gameOver("GAME OVER", font, 32);
		sf::Text pressEnter("Press enter to exit", font, 16);
		
		gameOver.setFillColor(sf::Color::Red);
		gameOver.setOrigin(gameOver.getLocalBounds().width / 2.f, gameOver.getLocalBounds().height / 2.f);
		gameOver.setPosition(window.getSize().x / 2.f, (window.getSize().y / 2.f) - (pressEnter.getLocalBounds().height * 2));

		pressEnter.setOrigin(pressEnter.getLocalBounds().width / 2.f, pressEnter.getLocalBounds().height / 2.f);
		pressEnter.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

		window.clear();
		window.draw(gameOver);
		window.draw(pressEnter);
		window.display();
	}

	while (window.isOpen() && window.waitEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				break;
			}
		}
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