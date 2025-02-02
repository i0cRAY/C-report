#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

int main() {
	sf::Font font;
	if (font.loadFromFile("arial.ttf") == false) {
		return EXIT_FAILURE;
	}
	enum class Direction { LEFT_TO_RIGHT, RIGHT_TO_LEFT }; // 定義文字移動的方向
	enum class Speed { s1, s2, s3, s4, s5 }; // 定義 5 種固定速度
	enum class Speed2 { Nothing, Double, Half }; // 定義額外的速度變化

	sf::RenderWindow w(sf::VideoMode(800, 250), "Hello");

	// 方向 & 速度
	Direction direction = Direction::RIGHT_TO_LEFT;
	Speed speed = Speed::s1;
	Speed2 speed2 = Speed2::Nothing;

	// 顏色
	sf::Text text("Hello", font, 200);
	text.setFillColor(sf::Color(0, 255, 0));

	sf::CircleShape circle(2);
	circle.setFillColor(sf::Color(0, 0, 0));

	// 判斷開始位置
	if (direction == Direction::RIGHT_TO_LEFT) {
		text.setPosition(float(w.getSize().x), 0);
	}

	while (w.isOpen()) {
		sf::Event evt;
		while (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Key::Left) {
					direction = Direction::RIGHT_TO_LEFT;
				}
				if (evt.key.code == sf::Keyboard::Key::Right) {
					direction = Direction::LEFT_TO_RIGHT;
				}

				// (80%) 用鍵盤的 f 跟 s 鍵來切換文字移動的快慢
				if (evt.key.code == sf::Keyboard::Key::S) {
					speed = Speed::s1;
				}
				if (evt.key.code == sf::Keyboard::Key::F) {
					speed = Speed::s5;
				}

				// (5%) 用鍵盤上方跟右方的數字鍵 1, 2, 3, 4, 5 切換不同的速度
				if (evt.key.code == sf::Keyboard::Key::Num1) {
					speed = Speed::s1;
				}
				if (evt.key.code == sf::Keyboard::Key::Num2) {
					speed = Speed::s2;
				}
				if (evt.key.code == sf::Keyboard::Key::Num3) {
					speed = Speed::s3;
				}
				if (evt.key.code == sf::Keyboard::Key::Num4) {
					speed = Speed::s4;
				}
				if (evt.key.code == sf::Keyboard::Key::Num5) {
					speed = Speed::s5;
				}

				// (5%) 每次按鍵盤上方或右方的 + 會加速 (增加 2 倍)，按 - 會減速 (變為 1/2)
				if (evt.key.code == sf::Keyboard::Add || evt.key.code == sf::Keyboard::Equal) {
					speed2 = Speed2::Double;
				}
				if (evt.key.code == sf::Keyboard::Subtract || evt.key.code == sf::Keyboard::Dash) {
					speed2 = Speed2::Half;
				}
			}
		}
		w.clear();
		w.draw(text);
		for (unsigned int i = 0; i < w.getSize().x; i = i + 4) {
			for (unsigned int j = 0; j < w.getSize().y; j += 4) {
				circle.setPosition((float)i, (float)j);
				w.draw(circle);
			}
		}
		w.display();

		float diff;
		switch (speed) {
		case Speed::s1:
			diff = 0.05f;
			break;
		case Speed::s2:
			diff = 0.06f;
			break;
		case Speed::s3:
			diff = 0.7f;
			break;
		case Speed::s4:
			diff = 0.8f;
			break;
		case Speed::s5:
			diff = 0.9f;
			break;
		}

		switch (speed2) {
		case Speed2::Nothing:
			break;
		case Speed2::Double:
			diff *= 2;
			break;
		case Speed2::Half:
			diff /= 2;
			break;
		}

		switch (direction) {
		case Direction::LEFT_TO_RIGHT:
			text.move(diff, 0.f);
			break;
		case Direction::RIGHT_TO_LEFT:
			text.move(-diff, 0.f);
			break;
		}
	}
	return EXIT_SUCCESS;
}
