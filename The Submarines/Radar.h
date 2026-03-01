#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Status.h"
#include "World.h"

struct EchoBlip {
    float wx, wy;        // 월드 좌표
    float timer;
    sf::Color color;
};

class Radar {
public:
    sf::Vector2f center;
    float radius;
    float scanAngle = 0.f;
    float scanSpeed = 90.f;
    std::vector<EchoBlip> blips;

    Radar(sf::Vector2f center, float radius)
        : center(center), radius(radius) {
    }

    void Update(float dt, World& world, Submarine& sub) {
        float prevAngle = scanAngle;
        scanAngle += scanSpeed * dt;
        if (scanAngle >= 360.f) scanAngle -= 360.f;

        for (auto& line : world.lines) {
            float mx = (line.x1 + line.x2) / 2.f;
            float my = (line.y1 + line.y2) / 2.f;
            float dx = mx - sub.GetPos().x;
            float dy = my - sub.GetPos().y;
            float angle = std::atan2(dy, dx) * 180.f / 3.14159f;
            if (angle < 0) angle += 360.f;

            if (IsScanned(prevAngle, scanAngle, angle)) {
                // 선분의 모든 점을 blip으로 저장
                blips.push_back({ line.x1, line.y1, 1.5f, line.color });
                blips.push_back({ line.x2, line.y2, 1.5f, line.color });
            }
        }

        // 타이머 감소 및 제거
        for (auto& b : blips) b.timer -= dt;
        blips.erase(
            std::remove_if(blips.begin(), blips.end(), [](const EchoBlip& b) { return b.timer <= 0; }),
            blips.end()
        );
    }

    void Draw(sf::RenderWindow& window, Submarine& sub, World& world) {
        float scale = radius / 300.f;

        DrawCircle(window, center, radius, sf::Color(0, 255, 0), 2.f);
        DrawCircle(window, center, radius / 2, sf::Color(0, 150, 0), 1.f);

        // 십자선
        sf::RectangleShape hLine({ radius * 2, 1.f });
        hLine.setOrigin({ radius, 0.5f });
        hLine.setPosition(center);
        hLine.setFillColor(sf::Color(0, 150, 0));
        window.draw(hLine);

        sf::RectangleShape vLine({ 1.f, radius * 2 });
        vLine.setOrigin({ 0.5f, radius });
        vLine.setPosition(center);
        vLine.setFillColor(sf::Color(0, 150, 0));
        window.draw(vLine);

        // echo 표시 - 선분 단위로
        for (auto& b : blips) {
            float alpha = b.timer / 1.5f;
            uint8_t a = (uint8_t)(alpha * 255);

            float rx = b.wx - sub.GetPos().x;
            float ry = b.wy - sub.GetPos().y;

            sf::CircleShape dot(2.f);
            dot.setOrigin({ 2.f, 2.f });
            dot.setPosition({ center.x + rx * scale, center.y + ry * scale });
            dot.setFillColor(sf::Color(b.color.r, b.color.g, b.color.b, a));
            window.draw(dot);
        }

        // 스캔 라인
        float rad = scanAngle * 3.14159f / 180.f;
        sf::Vertex scanLine[] = {
            sf::Vertex{center, sf::Color(0, 255, 0)},
            sf::Vertex{
                sf::Vector2f(
                    center.x + std::cos(rad) * radius,
                    center.y + std::sin(rad) * radius
                ),
                sf::Color::Transparent
            }
        };
        window.draw(scanLine, 2, sf::PrimitiveType::Lines);

        // 잠수함 점
        sf::CircleShape dot(4.f);
        dot.setOrigin({ 4.f, 4.f });
        dot.setPosition(center);
        dot.setFillColor(sf::Color(0, 255, 0));
        window.draw(dot);
    }

private:
    bool IsScanned(float prev, float curr, float target) {
        if (prev < curr)
            return target >= prev && target <= curr;
        else
            return target >= prev || target <= curr;
    }

    void DrawCircle(sf::RenderWindow& window, sf::Vector2f pos, float r, sf::Color color, float thickness) {
        sf::CircleShape circle(r);
        circle.setOrigin({ r, r });
        circle.setPosition(pos);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(thickness);
        window.draw(circle);
    }
};