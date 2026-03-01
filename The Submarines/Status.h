#pragma once

#include <iostream>

struct Point {
    int x, y;
};

class Submarine {
private:
    Point pos;
    int battery;
    int oxygen;

public:
    Submarine() :pos({ 0,0 }), battery(100), oxygen(100) {}

    void ShowStatus() {
        std::cout << "--- [ 시스템 상태 ] ---" << std::endl;
        std::cout << "좌표: (" << this->pos.x << ", " << this->pos.y << ")" << std::endl;
        std::cout << "전력: " << this->battery << "% | 산소: " << this->oxygen << "%" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }

    void Move(int dx, int dy);
};