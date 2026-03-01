#include <SFML/Graphics.hpp>
#include "Status.h"
#include "Radar.h"
#include "World.h"
#include "Input.h"

int main() {
    World world(42);
    sf::RenderWindow gameWindow(sf::VideoMode({ 800, 600 }), "The Submarines");
    sf::RenderWindow radarWindow(sf::VideoMode({ 400, 400 }), "RADAR");
    Submarine sub;
    Radar radar({ 200.f, 200.f }, 180.f);
    sf::Clock clock;

    while (gameWindow.isOpen()) {
        float dt = clock.restart().asSeconds();

        // 이벤트 처리
        while (const std::optional event = gameWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                gameWindow.close();
            if (auto* key = event->getIf<sf::Event::KeyPressed>())
                HandleInput(sub, key, radar.blips);
        }
        while (const std::optional event = radarWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                radarWindow.close();
        }

        // 업데이트
        radar.Update(dt, world, sub);

        // 렌더링
        gameWindow.clear(sf::Color::Black);
        gameWindow.display();

        radarWindow.clear(sf::Color(0, 20, 0));
        radar.Draw(radarWindow, sub, world);
        radarWindow.display();
    }
    return 0;
}