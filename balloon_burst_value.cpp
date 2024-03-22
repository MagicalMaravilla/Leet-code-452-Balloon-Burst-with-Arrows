#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random> // Include the random header

struct Balloon {
    std::string color;
    int value;
    int start;
    int end;

    // Constructor for easy initialization
    Balloon(std::string col, int val, int start_pos, int end_pos)
        : color(std::move(col)), value(val), start(start_pos), end(end_pos) {}
};

// Utility function to generate a random integer within a specified range
int getRandomNumber(int min, int max) {
    static std::random_device rd; // Obtain a random number from hardware
    static std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(min, max); // Define the range

    return distr(eng); // Generate and return a random number within the range
}

int findMinArrowShotsAndCalculateValue(std::vector<Balloon>& balloons, int& totalValue) {
    if (balloons.empty()) return 0;

    // Sort balloons by their end position
    std::sort(balloons.begin(), balloons.end(), [](const Balloon& a, const Balloon& b) {
        return a.end < b.end;
    });

    int arrowPos = balloons[0].end;
    int arrowCount = 1;
    totalValue = balloons[0].value;

    for (size_t i = 1; i < balloons.size(); ++i) {
        if (balloons[i].start <= arrowPos) {
            totalValue += balloons[i].value;
        } else {
            arrowCount++;
            arrowPos = balloons[i].end;
            totalValue += balloons[i].value;
        }
    }

    return arrowCount;
}

int main() {
    // Define balloon colors and values
    std::vector<std::pair<std::string, int>> balloonTemplates{
        {"Red", 200},
        {"Orange", 175},
        {"Yellow", 150},
        {"Green", 100},
        {"Blue", 75},
        {"Indigo", 50},
        {"Violet", 25}
    };

    std::vector<Balloon> balloons;
    // Generate balloons with random position ranges
    for (const auto& templateBalloon : balloonTemplates) {
        int start = getRandomNumber(1, 50); // Random start
        int end = getRandomNumber(start + 1, 100); // Ensure end is greater than start
        balloons.emplace_back(templateBalloon.first, templateBalloon.second, start, end);
    }

    int totalValue = 0;
    int arrowsNeeded = findMinArrowShotsAndCalculateValue(balloons, totalValue);

    std::cout << "Total arrows needed: " << arrowsNeeded << std::endl;
    std::cout << "Total value gained: $" << totalValue << std::endl;

    return 0;
}
