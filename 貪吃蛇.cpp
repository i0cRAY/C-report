#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream> // 用於顯示分數

void resetGame(std::vector<sf::Vector2i>& snake, sf::Vector2i& food, sf::Vector2i fieldSize, bool& isDead, int& score) {
    snake = { {3, 4} };
    food = { rand() % fieldSize.x, rand() % fieldSize.y };
    isDead = false;
    score = 0;
}

int main() {
    sf::Vector2i fieldSize(20, 15); // 場地大小
    sf::Texture blockTexture;
    if (!blockTexture.loadFromFile("block.png")) {
        std::cout << "block.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<sf::Vector2i> snake = { {3, 4} };
    sf::Vector2i food(rand() % fieldSize.x, rand() % fieldSize.y);
    sf::Sprite block(blockTexture);
    sf::Vector2f blockSize(block.getLocalBounds().width, block.getLocalBounds().height);
    sf::VideoMode mode(
        unsigned int(fieldSize.x * blockSize.x),
        unsigned int(fieldSize.y * blockSize.y));
    sf::RenderWindow w(mode, L"貪吃蛇");

    enum class Direction { UP, DOWN, LEFT, RIGHT };
    Direction direction = Direction::RIGHT;

    sf::Clock clock;
    bool isDead = false;
    int score = 0;

    // 用於顯示分數的字體
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "arial.ttf is not found." << std::endl;
        return EXIT_FAILURE;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    while (w.isOpen()) {
        sf::Event evt;
        while (w.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                w.close();
            }
            if (evt.type == sf::Event::KeyPressed) {
                if (!isDead) {
                    if (evt.key.code == sf::Keyboard::Up && direction != Direction::DOWN) {
                        direction = Direction::UP;
                    }
                    if (evt.key.code == sf::Keyboard::Down && direction != Direction::UP) {
                        direction = Direction::DOWN;
                    }
                    if (evt.key.code == sf::Keyboard::Left && direction != Direction::RIGHT) {
                        direction = Direction::LEFT;
                    }
                    if (evt.key.code == sf::Keyboard::Right && direction != Direction::LEFT) {
                        direction = Direction::RIGHT;
                    }
                }
                else { // 如果遊戲失敗，按任意鍵重新開始
                    resetGame(snake, food, fieldSize, isDead, score);
                }
            }
        }

        if (!isDead && clock.getElapsedTime().asSeconds() >= 0.4f) {
            sf::Vector2i head = snake[0];
            if (direction == Direction::UP) head.y--;
            if (direction == Direction::DOWN) head.y++;
            if (direction == Direction::LEFT) head.x--;
            if (direction == Direction::RIGHT) head.x++;

            // 檢查邊界碰撞
            if (head.x < 0 || head.x >= fieldSize.x || head.y < 0 || head.y >= fieldSize.y) {
                isDead = true;
            }

            // 檢查自我碰撞
            for (size_t i = 1; i < snake.size(); ++i) {
                if (head == snake[i]) {
                    isDead = true;
                    break;
                }
            }

            if (!isDead) {
                if (food == head) { // 吃到食物
                    score++;
                    food = { rand() % fieldSize.x, rand() % fieldSize.y }; // 重新隨機產生食物
                }
                else {
                    snake.pop_back(); // 移除蛇的尾巴
                }
                snake.insert(snake.begin(), head); // 添加新的頭部位置
            }

            clock.restart();
        }

        w.clear(isDead ? sf::Color::Red : sf::Color::Black);

        // 更新分數顯示
        std::ostringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());
        w.draw(scoreText);

        // 繪製食物
        sf::Vector2f foodPos(food.x * blockSize.x, food.y * blockSize.y);
        block.setPosition(foodPos);
        w.draw(block);

        // 繪製蛇
        for (const sf::Vector2i& body : snake) {
            sf::Vector2f pos(body.x * blockSize.x, body.y * blockSize.y);
            block.setPosition(pos);
            w.draw(block);
        }

        w.display();
    }
    return EXIT_SUCCESS;
}
