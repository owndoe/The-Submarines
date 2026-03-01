#pragma once
#include <cmath>
#include <iostream>

struct Point {
    int x, y;
};

class Submarine {
private:
    Point pos;
    float battery;
    float oxygen;

public:
    Submarine() : pos({ 0, 0 }), battery(100.f), oxygen(100.f) {}

    void ShowStatus() {
        std::cout << "--- [ 시스템 상태 ] ---" << std::endl;
        std::cout << "좌표: (" << this->pos.x << ", " << this->pos.y << ")" << std::endl;
        std::cout << "전력: " << this->battery << "% | 산소: " << this->oxygen << "%" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }

    void Move(float dx, float dy);
    Point GetPos() const { return pos; }
    float GetBattery() const { return battery; }
    float GetOxygen() const { return oxygen; }
};