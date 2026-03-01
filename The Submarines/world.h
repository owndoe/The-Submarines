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
        std::uniform_int_distribution<int> caveCount(2, 5);

        // 수면
        AddLine(-500.f, -300.f, 500.f, -300.f, sf::Color(0, 150, 255), 20);

        // 바닥 높이 생성
        const int segments = 10;
        const float startX = -500.f;
        const float endX = 500.f;
        const float baseY = 300.f;
        float segWidth = (endX - startX) / segments;

        std::vector<float> heights(segments + 1);
        for (auto& h : heights)
            h = baseY + heightDist(rng);

        // 바닥 선분
        for (int i = 0; i < segments; i++) {
            float x1 = startX + segWidth * i;
            float x2 = startX + segWidth * (i + 1);
            AddLine(x1, heights[i], x2, heights[i + 1], sf::Color(0, 255, 0), 10);
        }

        auto getFloorY = [&](float x) {
            int seg = (int)((x - startX) / segWidth);
            seg = std::clamp(seg, 0, segments - 1);
            float t = (x - (startX + segWidth * seg)) / segWidth;
            return heights[seg] * (1 - t) + heights[seg + 1] * t;
            };

        // 암초
        int rCount = reefCount(rng);
        std::uniform_real_distribution<float> reefX(startX + 50.f, endX - 80.f);
        for (int i = 0; i < rCount; i++) {
            float rx = reefX(rng);
            float rh = reefHeightDist(rng);
            float y0 = getFloorY(rx);
            float yTop = getFloorY(rx + 15.f) - rh;
            float y1 = getFloorY(rx + 30.f);
            AddLine(rx, y0, rx + 15.f, yTop, sf::Color(0, 255, 0), 5);
            AddLine(rx + 15.f, yTop, rx + 30.f, y1, sf::Color(0, 255, 0), 5);
        }

        // 동굴 생성
        int cCount = caveCount(rng);
        std::uniform_real_distribution<float> caveX(startX + 80.f, endX - 80.f);
        std::uniform_real_distribution<float> caveWidth(60.f, 150.f);
        std::uniform_real_distribution<float> caveDepth(80.f, 200.f);

        for (int i = 0; i < cCount; i++) {
            float cx = caveX(rng);
            float cw = caveWidth(rng);
            float cd = caveDepth(rng);
            float floorY = getFloorY(cx + cw / 2.f);

            // 동굴 입구 왼쪽 벽
            AddLine(cx, floorY, cx + 10.f, floorY - 30.f, sf::Color(0, 255, 0), 3);
            // 동굴 천장
            AddLine(cx + 10.f, floorY - 30.f, cx + cw / 2.f, floorY - cd, sf::Color(0, 255, 0), 8);
            AddLine(cx + cw / 2.f, floorY - cd, cx + cw - 10.f, floorY - 30.f, sf::Color(0, 255, 0), 8);
            // 동굴 입구 오른쪽 벽
            AddLine(cx + cw - 10.f, floorY - 30.f, cx + cw, floorY, sf::Color(0, 255, 0), 3);
        }
    }
}