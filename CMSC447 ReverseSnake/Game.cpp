#include "Game.hpp"

void movePlayer(sf::RectangleShape &shape, sf::Vector2f playerDirection)
{
	shape.move(playerDirection);
}

void startScreen(sf::RenderWindow &window)
{
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sounds\\EKANS Theme.wav");
	sf::Sound background;
	background.setBuffer(buffer);
	background.setVolume(25);
	background.setLoop(true);
	background.play();

	sf::Event event;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text title("Reverse Snake", font, 64);
	title.setFillColor(sf::Color::Red);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 - 150.f);
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

	std::vector<Score*> highScores;

	loadScores(highScores);

	window.clear();
	window.draw(title);
	window.draw(startButton);
	window.draw(scoreButton);
	window.display();

	while (window.waitEvent(event))
	{
		if (event.type == sf::Event::MouseMoved)
		{
			if (startButton.getGlobalBounds().contains(event.mouseMove.x,
				event.mouseMove.y))
			{
				startButton.setFillColor(sf::Color::Green);
				scoreButton.setFillColor(sf::Color::White);
			}
			if (scoreButton.getGlobalBounds().contains(event.mouseMove.x,
				event.mouseMove.y))
			{
				startButton.setFillColor(sf::Color::White);
				scoreButton.setFillColor(sf::Color::Green);
			}
		}
		else if (event.type == sf::Event::Closed)
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
					gameLoop(highScores, window);
					break;
				}
				else
				{
					scoreButtonPressed = true;
					background.stop();
					leaderboard(highScores, window);
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
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (startButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			{
				gameLoop(highScores, window);
			}
			else if (scoreButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			{
				background.stop();
				leaderboard(highScores, window);
			}
		}
		window.clear();
		window.draw(title);
		window.draw(startButton);
		window.draw(scoreButton);
		window.display();
	}
}

void leaderboard(std::vector<Score*> &highScores, sf::RenderWindow &window)
{
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sounds\\Leaderboard Theme.wav");
	sf::Sound background;
	background.setBuffer(buffer);
	background.setVolume(25);
	background.setLoop(true);
	background.play();

	sf::Event event;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text title("Leaderboard", font, 64);
	title.setFillColor(sf::Color::Red);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 - 260.f);
	sf::Text scores("Scores:", font, 24);
	scores.setOrigin(scores.getLocalBounds().width / 2.f, scores.getLocalBounds().height / 2.f);
	scores.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 200.f);
	sf::Text text("Press any key to continue...", font, 16);
	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 240.f);

	std::vector<std::pair<sf::Text, sf::Text>> listings;
	int highScoreOrigin = 50;

	std::vector<Score*>::iterator end = (highScores.size() > 5) ? highScores.begin() + 5 : highScores.end();
	for (auto iter = highScores.begin(); iter != end; ++iter)
	{
		Score *score = *iter;
		sf::Text initials(score->initials(), font, 18);
		sf::Text highScore(std::to_string(score->score()), font, 18);

		initials.setOrigin(0, initials.getLocalBounds().height / 2.f);
		highScore.setOrigin(highScore.getLocalBounds().width,
			highScore.getLocalBounds().height / 2.f);

		initials.setPosition(title.getGlobalBounds().left,
			scores.getPosition().y + highScoreOrigin);
		highScore.setPosition(title.getGlobalBounds().left + title.getLocalBounds().width,
			scores.getPosition().y + highScoreOrigin);

		listings.push_back({ initials, highScore });

		highScoreOrigin += 30;
	}

	while (window.isOpen() && window.waitEvent(event))
	{
		window.clear();
		window.draw(title);
		window.draw(scores);
		for (auto iter = listings.begin(); iter != listings.end(); ++iter)
		{
			window.draw(iter->first);
			window.draw(iter->second);
		}
		window.draw(text);
		window.display();
		if (event.type == sf::Event::KeyPressed)
		{
			background.stop();
			startScreen(window);
			break;
		}
		else if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void gameLoop(std::vector<Score*> &highScores, sf::RenderWindow &window)
{
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sounds\\Collision Sound Effect.wav");
	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("sounds\\Collision Sound Effect2(player collision).wav");
	sf::Sound collision;
	sf::Sound playerCollision;
	collision.setBuffer(buffer);
	collision.setVolume(25);
	playerCollision.setBuffer(buffer2);
	playerCollision.setVolume(25);

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
	srand((unsigned int)time(0));
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
	food.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 - 200.f);
	sf::Texture food2Txt;
	food2Txt.loadFromFile("textures\\food2.png");
	sf::Sprite food2(food2Txt);
	food2.setPosition(window.getSize().x / 2 - 200.f, window.getSize().y / 2.f);
	sf::Texture cookieTxt;
	cookieTxt.loadFromFile("textures\\cookie.png");
	sf::Sprite cookie(cookieTxt);
	cookie.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 + 200.f);

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
	walls[1].setPosition(0, window.getSize().y - 16.f);

	walls[2].setTextureRect({ 0, 0, 16, (int)window.getSize().y });
	walls[3].setTextureRect({ 0, 0, 16, (int)window.getSize().y });
	walls[3].setPosition(window.getSize().x - 16.f, 0);

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
						collision.play();
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
						collision.play();
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
						collision.play();
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
						collision.play();
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
						collision.play();
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
						collision.play();
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
				collision.play();
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
				collision.play();
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
				collision.play();
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
				playerCollision.play();
				collided = true;
				break;
			}
		}
		if (snake2exists)
		{
			if (snake2->checkCollision(player))
			{
				playerCollision.play();
				collided = true;
				break;
			}
		}
		if (snake3exists)
		{
			if (snake3->checkCollision(player))
			{
				playerCollision.play();
				collided = true;
				break;
			}
		}
		// check for collision with walls
		if (walls[0].getGlobalBounds().intersects(player.getGlobalBounds()) || walls[1].getGlobalBounds().intersects(player.getGlobalBounds())
			|| walls[2].getGlobalBounds().intersects(player.getGlobalBounds()) || walls[3].getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			playerCollision.play();
			collided = true;
			break;
		}
		// check for collision with food
		if (food.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			collision.play();
			float x = rand() % 560 + 48.f;
			float y = rand() % 560 + 48.f;

			points.setPosition(food.getPosition().x, food.getPosition().y - 16);
			pointClock.restart();
			food.setPosition(x, y);
			score += 10;
			scoreText.setString("Score : " + std::to_string(score));
		}
		if (food2.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			collision.play();
			float x = rand() % 560 + 48.f;
			float y = rand() % 560 + 48.f;

			points2.setPosition(food2.getPosition().x, food2.getPosition().y - 16);
			point2Clock.restart();
			food2.setPosition(x, y);
			score += 10;
			scoreText.setString("Score : " + std::to_string(score));
		}
		// check for collision with cookie
		if (cookie.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			collision.play();
			cookieClock.restart();
			float x = rand() % 560 + 48.f;
			float y = rand() % 560 + 48.f;

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
		endScreen(scoreText, score, highScores, window);
	}
}

void endScreen(sf::Text scoreText, int score, std::vector<Score*> &highScores, sf::RenderWindow &window)
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
	gameOver.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 - 150.f);

	sf::Text retryButton("Retry", font, 20);
	retryButton.setOrigin(retryButton.getLocalBounds().width / 2, retryButton.getLocalBounds().height / 2);
	retryButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2 + 200.f);
	retryButton.setFillColor(sf::Color::Green);

	sf::Text exitButton("Exit", font, 20);
	exitButton.setOrigin(exitButton.getLocalBounds().width / 2, exitButton.getLocalBounds().height / 2);
	exitButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 225.f);

	scoreText.setOrigin(scoreText.getLocalBounds().width / 2.f, scoreText.getLocalBounds().height / 2.f);
	scoreText.setPosition(window.getSize().x / 2 - 15.f, window.getSize().y / 2 - 100.f);
	scoreText.setCharacterSize(26);

	sf::RectangleShape playerInput(sf::Vector2f(150, 30));
	playerInput.setFillColor(sf::Color::White);
	playerInput.setOrigin(playerInput.getLocalBounds().width / 2.f,
		playerInput.getLocalBounds().height / 2.f);
	playerInput.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	std::string playerInitials;
	sf::Text playerText("", font, 20);
	playerText.setFillColor(sf::Color::Black);
	playerText.setOrigin(playerText.getLocalBounds().width / 2.f,
		playerText.getLocalBounds().height / 2.f);
	playerText.setPosition(playerInput.getGlobalBounds().left + (playerInput.getGlobalBounds().width / 2.f),
		playerInput.getGlobalBounds().top + (playerInput.getGlobalBounds().height / 2.f));

	window.clear();
	window.draw(gameOver);
	window.draw(scoreText);
	window.draw(exitButton);
	window.draw(retryButton);
	window.draw(playerInput);
	window.draw(playerText);
	window.display();

	while (window.waitEvent(event))
	{
		if (event.type == sf::Event::MouseMoved)
		{
			if (retryButton.getGlobalBounds().contains(event.mouseMove.x,
				event.mouseMove.y))
			{
				retryButton.setFillColor(sf::Color::Green);
				exitButton.setFillColor(sf::Color::White);
			}
			if (exitButton.getGlobalBounds().contains(event.mouseMove.x,
				event.mouseMove.y))
			{
				exitButton.setFillColor(sf::Color::Green);
				retryButton.setFillColor(sf::Color::White);
			}
		}
		else if (event.type == sf::Event::Closed)
		{
			saveScores(highScores);
			window.close();
		}
		else if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == '\b')
			{
				if (playerInitials.size() > 0)
				{
					playerInitials.erase(playerInitials.size() - 1, 1);
				}
			}
			else if (playerInitials.size() < 3)
			{
				playerInitials += toupper(static_cast<char>(event.text.unicode));
			}
			playerText.setString(playerInitials);
			playerText.setOrigin(playerText.getLocalBounds().width / 2.f,
				playerText.getLocalBounds().height / 2.f);
			playerText.setPosition(playerInput.getGlobalBounds().left + (playerInput.getGlobalBounds().width / 2.f),
				playerInput.getGlobalBounds().top + (playerInput.getGlobalBounds().height / 2.f) - 5);
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
					if (playerInitials.size() > 0)
					{
						highScores.push_back(new Score(score, playerInitials));
					}
					saveScores(highScores);
					gameLoop(highScores, window);
					break;
				}
				else
				{
					ExitButtonPressed = true;
					if (playerInitials.size() > 0)
					{
						highScores.push_back(new Score(score, playerInitials));
					}
					saveScores(highScores);
					window.close();
					break;
				}

				break;
			}
			default:
			{
			}
			}
		}
		else if(event.type == sf::Event::MouseButtonReleased)
		{
			if (exitButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			{
				if (playerInitials.size() > 0)
				{
					highScores.push_back(new Score(score, playerInitials));
				}
				saveScores(highScores);
				window.close();
			}
			else if (retryButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			{
				if (playerInitials.size() > 0)
				{
					highScores.push_back(new Score(score, playerInitials));
				}
				saveScores(highScores);
				gameLoop(highScores, window);
			}
		}

		window.clear();
		window.draw(gameOver);
		window.draw(scoreText);
		window.draw(exitButton);
		window.draw(retryButton);
		window.draw(playerInput);
		window.draw(playerText);
		window.display();
	}
}

void loadScores(std::vector<Score*> &scores)
{
	HRESULT hResult = 0;
	PWSTR appDataPath = NULL;
	LPCWSTR gameFolder = L"Ekans",
		scoresFileName = L"scores.json";
	WCHAR gameFolderAbsPath[MAX_PATH] = { 0 },
		scoresFileAbsPath[MAX_PATH] = { 0 };
	HANDLE hScoresFile = INVALID_HANDLE_VALUE;
	DWORD dwFileSize = 0,
		dwBytesRead = 0;
	PSTR pbFileData = NULL;
	web::json::value jsonScores;

	// Get RoamingAppData location
	hResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData,
		KF_FLAG_DEFAULT,
		NULL,
		&appDataPath);

	if (FAILED(hResult))
	{
		goto end;
	}

	if (NULL == PathCombineW(gameFolderAbsPath, appDataPath, gameFolder)
		|| NULL == PathCombineW(scoresFileAbsPath, gameFolderAbsPath, scoresFileName))
	{
		goto end;
	}

	if (ERROR == CreateDirectoryW(gameFolderAbsPath, NULL)
		&& ERROR_ALREADY_EXISTS != GetLastError())
	{
		goto end;
	}

	hScoresFile = CreateFileW(scoresFileAbsPath,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);

	// We're done with path now
	CoTaskMemFree(appDataPath);
	appDataPath = NULL;

	if (INVALID_HANDLE_VALUE == hScoresFile)
	{
		goto end;
	}

	dwFileSize = GetFileSize(hScoresFile, NULL);
	if (0 == dwFileSize)
	{
		goto end;
	}

	pbFileData = (PSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	if(NULL == pbFileData)
	{
		goto end;
	}

	if (FALSE == ReadFile(hScoresFile,
		pbFileData,
		dwFileSize,
		&dwBytesRead,
		NULL))
	{
		goto end;
	}

	jsonScores = web::json::value::parse(std::string(pbFileData));
	if (0 == jsonScores.size())
	{
		goto end;
	}

	// Load global scores
	getGlobalScores(jsonScores);

	for (web::json::object::const_iterator iter = jsonScores.as_object().cbegin();
		iter != jsonScores.as_object().cend();
		++iter)
	{
		const utility::string_t initials = iter->first;
		const int score = iter->second.as_integer();

		scores.push_back(new Score(score, utility::conversions::to_utf8string(initials)));
	}

	// Sort in reverse so highest is first
	std::sort(scores.rbegin(), scores.rend(), [](Score *lhs, Score *rhs) {
		return lhs->score() < rhs->score();
	});

end:
	if (pbFileData)
	{
		HeapFree(GetProcessHeap(), 0, pbFileData);
		pbFileData = NULL;
	}
	if (INVALID_HANDLE_VALUE != hScoresFile)
	{
		CloseHandle(hScoresFile);
		hScoresFile = INVALID_HANDLE_VALUE;
	}
	if (appDataPath) {
		CoTaskMemFree(appDataPath);
		appDataPath = NULL;
	}
}

void saveScores(std::vector<Score*> &scores)
{
	HRESULT hResult = 0;
	PWSTR appDataPath = NULL;
	LPCWSTR gameFolder = L"Ekans",
		scoresFileName = L"scores.json";
	WCHAR gameFolderAbsPath[MAX_PATH] = { 0 },
		scoresFileAbsPath[MAX_PATH] = { 0 };
	HANDLE hScoresFile = INVALID_HANDLE_VALUE;
	DWORD dwBytesWritten = 0;
	web::json::value jsonScores;

	for (std::vector<Score*>::iterator iter = scores.begin();
		iter != scores.end();
		++iter)
	{
		Score *score = *iter;
		try {
			if (jsonScores.at(utility::conversions::to_utf16string(score->initials())).as_integer() < score->score())
			{
				jsonScores[utility::conversions::to_utf16string(score->initials())] = score->score();
			}
		}
		catch (web::json::json_exception &e)
		{
			jsonScores[utility::conversions::to_utf16string(score->initials())] = score->score();
		}
	}

	postGlobalScores(jsonScores);

	utility::string_t stringScores = jsonScores.serialize();
	std::string fileData = utility::conversions::to_utf8string(stringScores);

	// Get RoamingAppData location
	hResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData,
		KF_FLAG_DEFAULT,
		NULL,
		&appDataPath);

	if (FAILED(hResult))
	{
		goto end;
	}

	if (NULL == PathCombineW(gameFolderAbsPath, appDataPath, gameFolder)
		|| NULL == PathCombineW(scoresFileAbsPath, gameFolderAbsPath, scoresFileName))
	{
		goto end;
	}

	if (ERROR == CreateDirectoryW(gameFolderAbsPath, NULL)
		&& ERROR_ALREADY_EXISTS != GetLastError())
	{
		goto end;
	}

	hScoresFile = CreateFileW(scoresFileAbsPath,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);

	// We're done with path now
	CoTaskMemFree(appDataPath);
	appDataPath = NULL;

	if (INVALID_HANDLE_VALUE == hScoresFile)
	{
		goto end;
	}

	WriteFile(hScoresFile,
		fileData.data(),
		fileData.length(),
		&dwBytesWritten,
		NULL);
	
end:
	if (INVALID_HANDLE_VALUE != hScoresFile)
	{
		CloseHandle(hScoresFile);
		hScoresFile = INVALID_HANDLE_VALUE;
	}
	if (appDataPath) {
		CoTaskMemFree(appDataPath);
		appDataPath = NULL;
	}

}

void getGlobalScores(web::json::value &jsonScores)
{
	web::http::client::http_client client(U("https://scores.edc137.com/scores"));
	
	web::json::value globalScores;

	client.request(web::http::methods::GET).then([](const web::http::http_response &response) -> pplx::task<web::json::value> {
		if (response.status_code() == web::http::status_codes::OK)
		{
			OutputDebugStringA("status OK\r\n");

			return response.extract_json();
		}
		}).then([&globalScores](const pplx::task<web::json::value>& task) {
			try {
				globalScores = web::json::value::parse(task.get().as_string());
			}
			catch (const web::http::http_exception& e) {
				OutputDebugStringA(e.what());
			}
			catch (const web::json::json_exception& e) {
				OutputDebugStringA(e.what());
			}

			})
			.wait();

	web::json::value::value_type jsonValueType = globalScores.type();

	if (web::json::value::Object == jsonValueType)
	{
		for (web::json::object::const_iterator iter = globalScores.as_object().cbegin();
			iter != globalScores.as_object().cend();
			++iter)
		{
			const utility::string_t initials = iter->first;
			const int score = iter->second.as_integer();

			try {
				if (jsonScores.at(initials).as_integer() < score)
				{
					jsonScores[initials] = score;
				}
			}
			catch (web::json::json_exception &e)
			{
				jsonScores[initials] = score;
			}
		}
	}
}

void postGlobalScores(web::json::value &jsonScores)
{
	web::http::client::http_client client(U("https://scores.edc137.com/scores"));

	client.request(web::http::methods::POST, U("/"), jsonScores).wait();
}