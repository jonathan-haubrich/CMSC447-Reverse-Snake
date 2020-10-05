#include <SFML/Graphics.hpp>

void movePlayer(sf::RectangleShape &shape);

class Snake {
public:
	Snake() {
		_head = _tail = new SnakeNode;
		_head->segment.setPosition({ 50.f, 50.f });
		_head->segment.setFillColor(sf::Color::Red);
	}
	~Snake()
	{
		SnakeNode *tmp;
		while(_head)
		{
			tmp = _head->next;
			delete _head;
			_head = tmp;
		} 
	}

	void draw(sf::RenderWindow &window)
	{
		SnakeNode *tmp = _head;
		while (tmp)
		{
			window.draw(tmp->segment);
			tmp = _head->next;
		}
	}

private:
	struct SnakeNode {
		sf::RectangleShape segment{ {10.f, 10.f} };
		SnakeNode *next{ nullptr };
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
				movePlayer(shape);
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
		newPosition.x -= 10;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		newPosition.x += 10;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		newPosition.y -= 10;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		newPosition.y += 10;
	}

	shape.setPosition(newPosition);
}