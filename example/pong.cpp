#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong");
    sf::CircleShape shape(10.f);
    sf::RectangleShape player(sf::Vector2f(20, 100)), enemy(sf::Vector2f(20, 100));
    sf::Font f; f.loadFromFile("arial.ttf");
    sf::Text t; t.setFont(f); t.setCharacterSize(30); t.setPosition(1280 / 2, 720 / 2);
    player.setFillColor(sf::Color::White);
    player.setPosition(sf::Vector2f(0, 360));
    enemy.setPosition(sf::Vector2f(1260, 360));
    shape.setPosition(sf::Vector2f(1280 / 2, 720 / 2));
    float ballspeedx = -0.5, ballspeedy = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        time /= 200; if(time > 5) time = 5;
        clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) if (event.type == sf::Event::Closed) window.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.move(0, -0.5 * time);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.move(0, 0.5 * time);
        if(shape.getGlobalBounds().intersects(player.getGlobalBounds()) || shape.getGlobalBounds().intersects(enemy.getGlobalBounds())) { if(ballspeedx > 0) ballspeedx = -(ballspeedx + 0.2); else ballspeedx = -(ballspeedx - 0.2); ballspeedy += (player.getPosition().y + enemy.getPosition().y) / 5000; }
        if(shape.getPosition().y >= 700 || shape.getPosition().y <= 10) ballspeedy = -ballspeedy;
        if(shape.getPosition().y > enemy.getPosition().y) enemy.move(0, 0.5 * time); else enemy.move(0, -0.5 * time);
        shape.move(ballspeedx * time, ballspeedy * time);
        if(shape.getPosition().x < player.getPosition().x) { t.setString(L"You lose!"); window.draw(t); window.display(); std::this_thread::sleep_for(std::chrono::seconds(3)); window.close(); main(); }
        if(shape.getPosition().x > enemy.getPosition().x) { t.setString(L"You won!"); window.draw(t); window.display(); std::this_thread::sleep_for(std::chrono::seconds(3)); window.close(); main(); }
        window.clear();
        window.draw(shape);
        window.draw(enemy);
        window.draw(player);
        window.draw(t);
        window.display();
    }
}
