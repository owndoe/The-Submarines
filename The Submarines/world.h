#pragma once
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

struct Line {
    float x1, y1, x2, y2;
    sf::Color color = sf::Color(0, 255, 0);
};

class World {
public:
    std::vector<Line> lines;
    unsigned int seed;

    World(unsigned int seed = 12345) : seed(seed) {
        Generate(seed);
    }

    void Generate(unsigned int seed) {
        lines.clear();
        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> heightDist(-50.f, 50.f);
        std::uniform_real_distribution<float> reefHeightDist(50.f, 180.f);
        std::uniform_int_distribution<int> reefCount(3, 7);

        // 수면
        AddLine(-500.f, -300.f, 500.f, -300.f, sf::Color(0, 150, 255), 20);

        // 바닥 - 랜덤 높이
        const int segments = 10;
        const float startX = -500.f;
        const float endX = 500.f;
        const float baseY = 300.f;
        float segWidth = (endX - startX) / segments;

        // 각 꼭짓점 높이 미리 생성
        std::vector<float> heights(segments + 1);
        for (auto& h : heights)
            h = baseY + heightDist(rng);

        // 꼭짓점들을 선분으로 연결
        for (int i = 0; i < segments; i++) {
            float x1 = startX + segWidth * i;
            float x2 = startX + segWidth * (i + 1);
            AddLine(x1, heights[i], x2, heights[i + 1], sf::Color(0, 255, 0), 10);
        }

        // 암초 랜덤 생성
        int count = reefCount(rng);
        std::uniform_real_distribution<float> reefX(startX + 50.f, endX - 80.f);

        auto getFloorY = [&](float x) {
            int seg = (int)((x - startX) / segWidth);
            seg = std::clamp(seg, 0, segments - 1);
            float t = (x - (startX + segWidth * seg)) / segWidth;
            return heights[seg] * (1 - t) + heights[seg + 1] * t;
            };

        for (int i = 0; i < count; i++) {
            float rx = reefX(rng);
            float rh = reefHeightDist(rng);

            float y0 = getFloorY(rx);
            float yTop = getFloorY(rx + 15.f) - rh;
            float y1 = getFloorY(rx + 30.f);

            AddLine(rx, y0, rx + 15.f, yTop, sf::Color(0, 255, 0), 5);
            AddLine(rx + 15.f, yTop, rx + 30.f, y1, sf::Color(0, 255, 0), 5);
        }
    }

    void AddLine(float x1, float y1, float x2, float y2, sf::Color color, int divisions) {
        for (int i = 0; i < divisions; i++) {
            float t1 = (float)i / divisions;
            float t2 = (float)(i + 1) / divisions;
            Line seg;
            seg.x1 = x1 + (x2 - x1) * t1;
            seg.y1 = y1 + (y2 - y1) * t1;
            seg.x2 = x1 + (x2 - x1) * t2;
            seg.y2 = y1 + (y2 - y1) * t2;
            seg.color = color;
            lines.push_back(seg);
        }
    }
};