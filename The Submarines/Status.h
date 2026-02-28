#pragma once

#include <iostream>

struct Point {
    int x, y;
};

class Submarine {
public:
    Point pos;
    int battery;
    int oxygen;

    Submarine() :pos({ 0,0 }), battery(100), oxygen(100) {}
};

void ShowStatus(Submarine sub) {
    std::cout << "--- [ 시스템 상태 ] ---" << std::endl;
    std::cout << "좌표: (" << sub.pos.x << ", " << sub.pos.y << ")" << std::endl;
    std::cout << "전력: " << sub.battery << "% | 산소: " << sub.oxygen << "%" << std::endl;
    std::cout << "-----------------------" << std::endl;
}