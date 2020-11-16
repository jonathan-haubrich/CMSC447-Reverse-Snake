#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <Windows.h>

#define	MOVE_DISTANCE	16

void movePlayer(sf::RectangleShape &shape);

class Snake {
public:
	Snake() {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition({ 320.f, 320.f });
	}
	~Snake()
	{
		SnakeNode *tmp;
		while (_head)
		{
			tmp = _head->_next;
			delete _head;
			_head = tmp;
		}
	}

	void draw(sf::RenderWindow &window)
	{
		sf::Texture snakeHead;
		snakeHead.loadFromFile("textures\\snakeHead.png");
		sf::Texture snakeBody;
		snakeBody.loadFromFile("textures\\snakeBody.png");
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

	void grow()
	{
		_grow = true;
	}

	void move(sf::Vector2f targetPosition)
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
			if (tmp->_segment.getGlobalBounds().intersects(shape.getGlobalBounds()))
			{
				return true;
			}
			tmp = tmp->_next;
		}
		return false;
	}

private:
	struct SnakeNode {
		SnakeNode() {}
		SnakeNode(sf::Vector2f pos) : SnakeNode() { _segment.setPosition(pos); }
		sf::Sprite _segment;
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

HWND hWnd = GetConsoleWindow();
ShowWindow(hWnd, SW_HIDE);

sf::RenderWindow window(sf::VideoMode(640, 640), "Reverse Snake");
sf::RectangleShape player(sf::Vector2f(16.f, 16.f));
player.setOrigin(-16.f, -16.f);
player.setFillColor(sf::Color::Green);
sf::Vector2f playerDirection({ MOVE_DISTANCE, 0.f });
Snake snake;
sf::Clock moveClock,
growthClock;
sf::Time moveTime,
growthTime;
sf::Event event;

//wall textures
std::array<sf::Sprite, 4> walls;
sf::Texture wallTxt;
wallTxt.loadFromFile("textures\\wall.png");
wallTxt.setRepeated(true);
for (auto &wall : walls)
{
	wall.setTexture(wallTxt);
}

walls[0].setTextureRect({ 0, 0, (int)window.getSize().x, 16 });
walls[1].setTextureRect({ 0, 0, (int)window.getSize().x, 16 });
walls[1].setPosition(0, window.getSize().y - 16);

walls[2].setTextureRect({ 0, 0, 16, (int)window.getSize().y });
walls[3].setTextureRect({ 0, 0, 16, (int)window.getSize().y });
walls[3].setPosition(window.getSize().x - 16, 0);

// start screen
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

// game loop
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
	/*
	if (moveTime.asSeconds() >= 0.3f)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				// Get player direction
				sf::Vector2f newDirection = playerDirection;
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					newDirection = { 0.f, -MOVE_DISTANCE};
					break;
				case sf::Keyboard::Down:
					newDirection = { 0.f, MOVE_DISTANCE};
					break;
				case sf::Keyboard::Left:
					newDirection = { -MOVE_DISTANCE, 0.f };
					break;
				case sf::Keyboard::Right:
					newDirection = { MOVE_DISTANCE, 0.f };
					break;
				case sf::Keyboard::W:
					newDirection = { 0.f, -MOVE_DISTANCE};
					break;
				case sf::Keyboard::S:
					newDirection = { 0.f, MOVE_DISTANCE};
					break;
				case sf::Keyboard::A:
					newDirection = { -MOVE_DISTANCE, 0.f };
					break;
				case sf::Keyboard::D:
					newDirection = { MOVE_DISTANCE, 0.f };
					break;
				default:
					break;
				}
				playerDirection = newDirection;
			}
		}
		movePlayer(player, playerDirection);
	}
	*/
	if (moveTime.asSeconds() >= 0.3f)
	{
		sf::Vector2f targetPosition = player.getPosition();
		snake.move(targetPosition);
		moveClock.restart();
	}
	// check for collision with snake
	if (snake.checkCollision(player))
	{
		collided = true;
		break;
	}
	// check for collision with walls
	for (auto &wall : walls)
	{
		if (wall.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			collided = true;
			break;
		}
	}

	window.clear();
	for (auto &wall : walls)
	{
		window.draw(wall);
	}
	snake.draw(window);
	window.draw(player);
	window.display();
}

if (collided)
{
	// end screen
	bool RetryButtonSelected = true;
	bool RetryButtonPressed = false;
	bool ExitButtonSelected = false;
	bool ExitButtonPressed = false;
	sf::Text gameOver("GAME OVER", font, 32);
	sf::Text exitButton("Exit", font, 20);
	exitButton.setOrigin(exitButton.getLocalBounds().width / 2, exitButton.getLocalBounds().height / 2);
	exitButton.setPosition(window.getSize().x / 2, window.getSize().y / 2.f);
	sf::Text retryButton("Retry", font, 20);
	retryButton.setOrigin(retryButton.getLocalBounds().width / 2, retryButton.getLocalBounds().height / 2);
	retryButton.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 25.f);

	gameOver.setFillColor(sf::Color::Red);
	gameOver.setOrigin(gameOver.getLocalBounds().width / 2.f, gameOver.getLocalBounds().height / 2.f);
	gameOver.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150.f);

	window.clear();
	window.draw(gameOver);
	window.draw(exitButton);
	window.draw(retryButton);
	window.display();

	while (window.waitEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
			{
				if (!RetryButtonSelected)
				{
					RetryButtonSelected = true;
					ExitButtonSelected = false;
					retryButton.setFillColor(sf::Color::Green);
					exitButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(gameOver);
					window.draw(exitButton);
					window.draw(retryButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::W:
			{
				if (!RetryButtonSelected)
				{
					RetryButtonSelected = true;
					ExitButtonSelected = false;
					retryButton.setFillColor(sf::Color::Green);
					exitButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(gameOver);
					window.draw(exitButton);
					window.draw(retryButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::Down:
			{
				if (!ExitButtonSelected)
				{
					RetryButtonSelected = false;
					ExitButtonSelected = true;
					exitButton.setFillColor(sf::Color::Green);
					retryButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(gameOver);
					window.draw(exitButton);
					window.draw(retryButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::S:
			{
				if (!ExitButtonSelected)
				{
					RetryButtonSelected = false;
					ExitButtonSelected = true;
					exitButton.setFillColor(sf::Color::Green);
					retryButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(gameOver);
					window.draw(exitButton);
					window.draw(retryButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::Enter:
			{
				RetryButtonPressed = false;
				ExitButtonPressed = false;

				if (RetryButtonSelected)
				{
					RetryButtonPressed = true;
					window.close();
					main();
					break;
				}
				else
				{
					ExitButtonPressed = true;
					window.close();
					break;
				}

				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
}



//while (window.isOpen() && window.waitEvent(event))
//{
	//if (event.type == sf::Event::KeyPressed)
	//{
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		//{
			//break;
		//}
	//}
//}

return 0;
}

/*
void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection)
{
	shape.move(playerDirection);
}
*/
void movePlayer(sf::RectangleShape &shape)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		shape.move(-MOVE_DISTANCE, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		shape.move(MOVE_DISTANCE, 0);
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