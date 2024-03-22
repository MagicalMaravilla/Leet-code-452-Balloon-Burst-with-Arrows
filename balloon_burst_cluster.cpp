#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random> // Include the random header
#include <unordered_set> // For using unordered_set

// Balloon struct representing each balloon with properties
struct Balloon {
    std::string color;
    int value;
    int start;
    int end;
    int clusterId; // Attribute to identify the cluster of the balloon

    // Constructor for easy balloon initialization
    Balloon(std::string col, int val, int start_pos, int end_pos, int cluster_id)
        : color(std::move(col)), value(val), start(start_pos), end(end_pos), clusterId(cluster_id) {}
};

// Utility function to generate a random integer within a specified range
int getRandomNumber(int min, int max) {
    static std::random_device rd; // Obtain a random number from hardware
    static std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(min, max); // Define the range

    return distr(eng); // Generate and return a random number within the range
}

// Function to generate balloons with random positions and cluster IDs
std::vector<Balloon> generateBalloonsWithClusters(const std::vector<std::pair<std::string, int>>& balloonTemplates, int clusterCount) {
    std::vector<Balloon> balloons;
    for (const auto& templateBalloon : balloonTemplates) {
        int start = getRandomNumber(1, 50); // Random start position
        int end = getRandomNumber(start + 1, 100); // Ensure end is greater than start
        int clusterId = getRandomNumber(1, clusterCount); // Assign to a random cluster
        balloons.emplace_back(templateBalloon.first, templateBalloon.second, start, end, clusterId);
    }
    return balloons;
}

// Function to find the minimum number of arrow shots required and calculate the total value of popped balloons
int findMinArrowShotsAndCalculateValue(std::vector<Balloon>& balloons, int& totalValue) {
    if (balloons.empty()) return 0;

    // Sort balloons by their end position
    std::sort(balloons.begin(), balloons.end(), [](const Balloon& a, const Balloon& b) {
        return a.end < b.end;
    });

    std::unordered_set<int> poppedClusters; // Tracks the IDs of clusters already hit by an arrow
    int arrowCount = 0;
    totalValue = 0;

    for (const auto& balloon : balloons) {
        if (poppedClusters.find(balloon.clusterId) == poppedClusters.end()) {
            // This cluster hasn't been popped yet
            poppedClusters.insert(balloon.clusterId);
            arrowCount++;
            // Add the value of all balloons in this cluster
            for (const auto& b : balloons) {
                if (b.clusterId == balloon.clusterId) {
                    totalValue += b.value;
                }
            }
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

    // Specify the number of clusters
    int clusterCount = 3;

    // Generate balloons with random position ranges and cluster IDs
    std::vector<Balloon> balloons = generateBalloonsWithClusters(balloonTemplates, clusterCount);

    int totalValue = 0;
    int arrowsNeeded = findMinArrowShotsAndCalculateValue(balloons, totalValue);

    std::cout << "Total arrows needed: " << arrowsNeeded << std::endl;
    std::cout << "Total value gained: $" << totalValue << std::endl;

    return 0;
}
