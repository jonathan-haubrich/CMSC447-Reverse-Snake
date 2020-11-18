#include <iostream>
#include <fstream>  
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <Windows.h>
#include <time.h>

#define	MOVE_DISTANCE	16

std::ofstream outfile("scores.txt");

void startScreen(sf::RenderWindow &window);
void leaderboard(sf::RenderWindow &window);
void gameLoop(sf::RenderWindow &window);
void endScreen(sf::Text scoreText, int score, sf::RenderWindow &window);
void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection);

class Snake {
public:
	Snake() {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition({ 320.f, 320.f });
	}
	Snake(sf::Vector2f startingPosition) {
		_head = _tail = new SnakeNode;
		_head->_segment.setPosition(startingPosition);
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

	void draw(sf::RenderWindow &window, sf::Texture snakeHead, sf::Texture snakeBody)
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

	sf::Vector2f getHead()
	{
		return _head->_segment.getPosition();
	}

	int getLength()
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

	bool isSelfIntersecting()
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

	bool checkSnakeCollision(Snake* otherSnake)
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

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	sf::RenderWindow window(sf::VideoMode(640, 640), "Reverse Snake");

	// start screen
	startScreen(window);
	return 0;
}


void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection)
{

	shape.move(playerDirection);
}

void startScreen(sf::RenderWindow &window)
{
	sf::Event event;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text title("Reverse Snake", font, 64);
	title.setFillColor(sf::Color::Red);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150.f);
	sf::Text startButton("Start", font, 20);
	startButton.setOrigin(startButton.getLocalBounds().width / 2.f, startButton.getLocalBounds().height / 2.f);
	startButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 25);
	startButton.setFillColor(sf::Color::Green);
	sf::Text scoreButton("Leaderboard", font, 20);
	scoreButton.setOrigin(scoreButton.getLocalBounds().width / 2.f, scoreButton.getLocalBounds().height / 2.f);
	scoreButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	bool startButtonSelected = true;
	bool startButtonPressed = false;
	bool scoreButtonSelected = false;
	bool scoreButtonPressed = false;

	window.clear();
	window.draw(title);
	window.draw(startButton);
	window.draw(scoreButton);
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
				if (!startButtonSelected)
				{
					startButtonSelected = true;
					scoreButtonSelected = false;
					startButton.setFillColor(sf::Color::Green);
					scoreButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(title);
					window.draw(startButton);
					window.draw(scoreButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::W:
			{
				if (!startButtonSelected)
				{
					startButtonSelected = true;
					scoreButtonSelected = false;
					startButton.setFillColor(sf::Color::Green);
					scoreButton.setFillColor(sf::Color::White);
					window.clear();
					window.draw(title);
					window.draw(startButton);
					window.draw(scoreButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::Down:
			{
				if (!scoreButtonSelected)
				{
					startButtonSelected = false;
					scoreButtonSelected = true;
					startButton.setFillColor(sf::Color::White);
					scoreButton.setFillColor(sf::Color::Green);
					window.clear();
					window.draw(title);
					window.draw(startButton);
					window.draw(scoreButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::S:
			{
				if (!scoreButtonSelected)
				{
					startButtonSelected = false;
					scoreButtonSelected = true;
					startButton.setFillColor(sf::Color::White);
					scoreButton.setFillColor(sf::Color::Green);
					window.clear();
					window.draw(title);
					window.draw(startButton);
					window.draw(scoreButton);
					window.display();
				}
				break;
			}
			case sf::Keyboard::Enter:
			{
				startButtonPressed = false;
				scoreButtonPressed = false;

				if (startButtonSelected)
				{
					startButtonPressed = true;
					gameLoop(window);
					break;
				}
				else
				{
					scoreButtonPressed = true;
					leaderboard(window);
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

void leaderboard(sf::RenderWindow &window)
{
	sf::Event event;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text title("Leaderboard", font, 64);
	title.setFillColor(sf::Color::Red);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 260.f);
	sf::Text scores("Scores:", font, 24);
	scores.setOrigin(scores.getLocalBounds().width / 2.f, scores.getLocalBounds().height / 2.f);
	scores.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 200.f);
	sf::Text text("Press any key to continue...", font, 16);
	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 240.f);
	while (window.isOpen() && window.waitEvent(event))
	{
		window.clear();
		window.draw(title);
		window.draw(scores);
		window.draw(text);
		window.display();
		if (event.type == sf::Event::KeyPressed)
		{
			startScreen(window);
			break;
		}
	}
}

void gameLoop(sf::RenderWindow &window)
{
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::RectangleShape player(sf::Vector2f(16.f, 16.f));
	player.setOrigin(-16.f, -16.f);
	player.setFillColor(sf::Color::Green);
	sf::Vector2f playerDirection({ MOVE_DISTANCE, 0.f });
	Snake * snake = new Snake({ 560, 560 });
	Snake * snake2 = nullptr;
	Snake * snake3 = nullptr;
	int score = 0;
	srand(time(0));
	sf::Text scoreText;
	sf::Clock moveClock,
		playerClock,
		snake1Clock,
		snake2Clock,
		snake3Clock,
		snakeDClock,
		pointClock,
		point2Clock,
		boostClock,
		cookieClock,
		gameClock,
		finalClock,
		growthClock;
	sf::Time moveTime,
		playerTime,
		snake1Time,
		snake2Time,
		snake3Time,
		cookieTime,
		growthTime;
	sf::Event event;

	//snake textures
	sf::Texture snakeHead;
	snakeHead.loadFromFile("textures\\snakeHead.png");
	sf::Texture snakeBody;
	snakeBody.loadFromFile("textures\\snakeBody.png");
	sf::Texture snakeHead2;
	snakeHead2.loadFromFile("textures\\snakeHead2.png");
	sf::Texture snakeBody2;
	snakeBody2.loadFromFile("textures\\snakeBody2.png");
	sf::Texture snakeHead3;
	snakeHead3.loadFromFile("textures\\snakeHead3.png");
	sf::Texture snakeBody3;
	snakeBody3.loadFromFile("textures\\snakeBody3.png");

	//food textures
	sf::Texture foodTxt;
	foodTxt.loadFromFile("textures\\food.png");
	sf::Sprite food(foodTxt);
	food.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 200.f);
	sf::Texture food2Txt;
	food2Txt.loadFromFile("textures\\food2.png");
	sf::Sprite food2(food2Txt);
	food2.setPosition(window.getSize().x / 2 - 200.f, window.getSize().y / 2);
	sf::Texture cookieTxt;
	cookieTxt.loadFromFile("textures\\cookie.png");
	sf::Sprite cookie(cookieTxt);
	cookie.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 200.f);

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

	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setCharacterSize(16);

	bool collided = false;
	float playerSpeed = 0.2f;
	float respawnSpeed = 5.f;
	float growSpeed = 2.f;
	float snakeSpeed = 0.3f;
	sf::Text points("+10", font, 16);
	sf::Text points2("+15", font, 16);
	sf::Text snakeDeath("+10", font, 16);
	sf::Text boost("2x Speed", font, 16);
	bool snake1exists = true, snake2exists = false, snake3exists = false;

	// game loop
	while (window.isOpen())
	{
		growthTime = growthClock.getElapsedTime();
		if (growthTime.asSeconds() >= growSpeed)
		{
			if (snake1exists)
			{
				snake->grow();
			}
			if (snake2exists)
			{
				snake2->grow();
			}
			if (snake3exists)
			{
				snake3->grow();
			}
			score += 1;
			growthClock.restart();
		}

		snake1Time = snake1Clock.getElapsedTime();
		snake2Time = snake2Clock.getElapsedTime();
		snake3Time = snake3Clock.getElapsedTime();
		if (snake1Time.asSeconds() >= respawnSpeed && snake1exists == false)
		{
			snake = new Snake({ 560, 560 });
			snake1exists = true;
		}
		if (snake2Time.asSeconds() >= respawnSpeed && snake2exists == false)
		{
			snake2 = new Snake({ 80, 560 });
			snake2exists = true;
		}
		if (snake3Time.asSeconds() >= respawnSpeed * 2 && snake3exists == false)
		{
			snake3 = new Snake({ 560, 80 });
			snake3exists = true;
		}
		playerTime = playerClock.getElapsedTime();
		if (playerTime.asSeconds() >= playerSpeed)
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
						newDirection = { 0.f, -MOVE_DISTANCE };
						break;
					case sf::Keyboard::Down:
						newDirection = { 0.f, MOVE_DISTANCE };
						break;
					case sf::Keyboard::Left:
						newDirection = { -MOVE_DISTANCE, 0.f };
						break;
					case sf::Keyboard::Right:
						newDirection = { MOVE_DISTANCE, 0.f };
						break;
					case sf::Keyboard::W:
						newDirection = { 0.f, -MOVE_DISTANCE };
						break;
					case sf::Keyboard::S:
						newDirection = { 0.f, MOVE_DISTANCE };
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
			playerClock.restart();
		}
		moveTime = moveClock.getElapsedTime();
		int snakeLength = 0;
		if (moveTime.asSeconds() >= snakeSpeed)
		{
			sf::Vector2f targetPosition = player.getPosition();
			if (snake1exists)
			{
				snake->move(targetPosition);
				// check if snake1 collided with other snakes
				if (snake2exists)
				{
					if (snake->checkSnakeCollision(snake2))
					{
						snakeLength = snake->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake->getHead().x, snake->getHead().y);
						delete snake;
						score += 10 * snakeLength;
						snake1exists = false;
						snake1Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}
				if (snake3exists && snake1exists)
				{
					if (snake->checkSnakeCollision(snake3))
					{
						snakeLength = snake->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake->getHead().x, snake->getHead().y);
						delete snake;
						score += 10 * snakeLength;
						snake1exists = false;
						snake1Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}

			}
			if (snake2exists)
			{
				snake2->move(targetPosition);
				// check if snake 2 collided with other snakes
				if (snake1exists)
				{
					if (snake2->checkSnakeCollision(snake))
					{
						snakeLength = snake2->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake2->getHead().x, snake2->getHead().y);
						delete snake2;
						score += 10 * snakeLength;
						snake2exists = false;
						snake2Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}
				if (snake3exists && snake2exists)
				{
					if (snake2->checkSnakeCollision(snake3))
					{
						snakeLength = snake2->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake2->getHead().x, snake2->getHead().y);
						delete snake2;
						score += 10 * snakeLength;
						snake2exists = false;
						snake2Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}
			}
			if (snake3exists)
			{
				snake3->move(targetPosition);
				// check if snake 3 collided with other snakes
				if (snake1exists)
				{
					if (snake3->checkSnakeCollision(snake))
					{
						snakeLength = snake3->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake3->getHead().x, snake3->getHead().y);
						delete snake3;
						score += 10 * snakeLength;
						snake3exists = false;
						snake3Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}
				if (snake2exists && snake3exists)
				{
					if (snake3->checkSnakeCollision(snake2))
					{
						snakeLength = snake3->getLength();
						snakeDeath.setString("+" + std::to_string(10 * snakeLength));
						snakeDeath.setPosition(snake3->getHead().x, snake3->getHead().y);
						delete snake3;
						score += 10 * snakeLength;
						snake3exists = false;
						snake3Clock.restart();
						snakeDClock.restart();
						scoreText.setString("Score : " + std::to_string(score));
					}
				}
			}
			moveClock.restart();
		}
		// check for self intersection
		if (snake1exists)
		{
			if (snake->isSelfIntersecting())
			{
				snakeLength = snake->getLength();
				snakeDeath.setString("+" + std::to_string(10 * snakeLength));
				snakeDeath.setPosition(snake->getHead().x, snake->getHead().y);
				delete snake;
				score += 10 * snakeLength;
				snake1exists = false;
				snake1Clock.restart();
				snakeDClock.restart();
				scoreText.setString("Score : " + std::to_string(score));
			}
		}
		if (snake2exists)
		{
			if (snake2->isSelfIntersecting())
			{
				snakeLength = snake2->getLength();
				snakeDeath.setString("+" + std::to_string(10 * snakeLength));
				snakeDeath.setPosition(snake2->getHead().x, snake2->getHead().y);
				delete snake2;
				score += 10 * snakeLength;
				snake2exists = false;
				snake2Clock.restart();
				snakeDClock.restart();
				scoreText.setString("Score : " + std::to_string(score));
			}
		}
		if (snake3exists)
		{
			if (snake3->isSelfIntersecting())
			{
				snakeLength = snake3->getLength();
				snakeDeath.setString("+" + std::to_string(10 * snakeLength));
				snakeDeath.setPosition(snake3->getHead().x, snake3->getHead().y);
				delete snake3;
				score += 10 * snakeLength;
				snake3exists = false;
				snake3Clock.restart();
				snakeDClock.restart();
				scoreText.setString("Score : " + std::to_string(score));
			}
		}
		// check for player collision with snake
		if (snake1exists)
		{
			if (snake->checkCollision(player))
			{
				collided = true;
				break;
			}
		}
		if (snake2exists)
		{
			if (snake2->checkCollision(player))
			{
				collided = true;
				break;
			}
		}
		if (snake3exists)
		{
			if (snake3->checkCollision(player))
			{
				collided = true;
				break;
			}
		}
		// check for collision with walls
		if (walls[0].getGlobalBounds().intersects(player.getGlobalBounds()) || walls[1].getGlobalBounds().intersects(player.getGlobalBounds())
			|| walls[2].getGlobalBounds().intersects(player.getGlobalBounds()) || walls[3].getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			collided = true;
			break;
		}
		// check for collision with food
		if (food.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			int x = rand() % 560 + 48;
			int y = rand() % 560 + 48;

			points.setPosition(food.getPosition().x, food.getPosition().y - 16);
			pointClock.restart();
			food.setPosition(x, y);
			score += 10;
			scoreText.setString("Score : " + std::to_string(score));
		}
		if (food2.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			int x = rand() % 560 + 48;
			int y = rand() % 560 + 48;

			points2.setPosition(food2.getPosition().x, food2.getPosition().y - 16);
			point2Clock.restart();
			food2.setPosition(x, y);
			score += 10;
			scoreText.setString("Score : " + std::to_string(score));
		}
		// check for collision with cookie
		if (cookie.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			cookieClock.restart();
			int x = rand() % 560 + 48;
			int y = rand() % 560 + 48;

			boost.setPosition(cookie.getPosition().x, cookie.getPosition().y - 16);
			boostClock.restart();
			cookie.setPosition(x, y);
			playerSpeed = 0.1f;
		}
		cookieTime = cookieClock.getElapsedTime();
		// reset player speed after 5 seconds
		if (cookieTime.asSeconds() >= 5.f)
		{
			playerSpeed = 0.2f;
		}
		// remove snake respawn time after 45 seconds
		if (gameClock.getElapsedTime().asSeconds() >= 45)
		{
			respawnSpeed = 0;
			growSpeed = 1;
		}
		// after 1min 30 seconds increase snake movement speed and snake growth
		if (finalClock.getElapsedTime().asSeconds() >= 90)
		{
			growSpeed = .5f;
			snakeSpeed = 0.2f;
		}


		window.clear();
		for (auto &wall : walls)
		{
			window.draw(wall);
		}
		scoreText.setString("Score: " + std::to_string(score));
		window.draw(scoreText);
		window.draw(food);
		window.draw(food2);
		window.draw(cookie);
		if (snake1exists)
		{
			snake->draw(window, snakeHead, snakeBody);
		}
		if (snake2exists)
		{
			snake2->draw(window, snakeHead2, snakeBody2);
		}
		if (snake3exists)
		{
			snake3->draw(window, snakeHead3, snakeBody3);
		}
		window.draw(player);
		if (pointClock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(points);
		}
		if (point2Clock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(points2);
		}
		if (boostClock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(boost);
		}
		if (snakeDClock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(snakeDeath);
		}
		window.display();
	}

	if (collided)
	{
		endScreen(scoreText, score, window);
	}
}

void endScreen(sf::Text scoreText, int score, sf::RenderWindow &window)
{
	sf::Event event;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	// end screen
	bool RetryButtonSelected = true;
	bool RetryButtonPressed = false;
	bool ExitButtonSelected = false;
	bool ExitButtonPressed = false;

	sf::Text gameOver("GAME OVER", font, 32);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setOrigin(gameOver.getLocalBounds().width / 2.f, gameOver.getLocalBounds().height / 2.f);
	gameOver.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150.f);

	sf::Text exitButton("Exit", font, 20);
	exitButton.setOrigin(exitButton.getLocalBounds().width / 2, exitButton.getLocalBounds().height / 2);
	exitButton.setPosition(window.getSize().x / 2, window.getSize().y / 2.f);

	sf::Text retryButton("Retry", font, 20);
	retryButton.setOrigin(retryButton.getLocalBounds().width / 2, retryButton.getLocalBounds().height / 2);
	retryButton.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 25.f);
	retryButton.setFillColor(sf::Color::Green);

	scoreText.setOrigin(scoreText.getLocalBounds().width / 2.f, scoreText.getLocalBounds().height / 2.f);
	scoreText.setPosition(window.getSize().x / 2 - 15.f, window.getSize().y / 2 - 100.f);
	scoreText.setCharacterSize(26);

	window.clear();
	window.draw(gameOver);
	window.draw(scoreText);
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
					window.draw(scoreText);
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
					window.draw(scoreText);
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
					window.draw(scoreText);
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
					window.draw(scoreText);
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
					//window.close();
					//main();
					gameLoop(window);
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
