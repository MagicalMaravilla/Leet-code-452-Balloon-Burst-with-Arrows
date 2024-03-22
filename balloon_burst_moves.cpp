#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <unordered_set>

// Represents each balloon with its properties
struct Balloon {
    std::string color; // Visual identifier
    int value;         // Associated score value
    int start;         // Start position on an imaginary line
    int end;           // End position on an imaginary line
    int clusterId;     // ID to identify which cluster the balloon belongs to

    // Constructor to initialize a balloon instance
    Balloon(std::string col, int val, int s, int e, int cid)
        : color(std::move(col)), value(val), start(s), end(e), clusterId(cid) {}
};

// Utility function to generate a random number within a specified range
int getRandomNumber(int min, int max) {
    static std::random_device rd; // Seed with a real random value, if available
    static std::mt19937 eng(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distr(min, max); // Define the range
    return distr(eng);
}

// Generates balloons with initial random positions and assigns them to clusters
std::vector<Balloon> generateBalloonsWithClusters(const std::vector<std::pair<std::string, int>>& balloonTemplates, int clusterCount) {
    std::vector<Balloon> balloons;
    for (const auto& templateBalloon : balloonTemplates) {
        int start = getRandomNumber(1, 100); // Random starting position
        int end = getRandomNumber(start + 1, start + 50); // End position ensuring the balloon has a length
        int clusterId = getRandomNumber(1, clusterCount); // Randomly assign a cluster ID
        balloons.emplace_back(templateBalloon.first, templateBalloon.second, start, end, clusterId);
    }
    return balloons; // Return the vector of generated balloons
}

// Dynamically generates clusters based on balloon positions
std::vector<Balloon> generateAdaptiveClusters(std::vector<Balloon>& balloons, int maxClusterSize) {
    std::sort(balloons.begin(), balloons.end(), [](const Balloon& a, const Balloon& b) {
        return a.start < b.start; // Sort balloons by their start position
    });

    int currentClusterId = 1; // Initialize the first cluster ID
    int balloonsInCurrentCluster = 0; // Counter for balloons in the current cluster
    int lastEndPosition = 0; // Track the end position of the last balloon in a cluster

    for (auto& balloon : balloons) {
        // Form a new cluster if the current one is full or balloons are too far apart
        if (balloonsInCurrentCluster >= maxClusterSize || (balloon.start - lastEndPosition > 10 && balloonsInCurrentCluster > 0)) {
            currentClusterId++; // Move to the next cluster
            balloonsInCurrentCluster = 0; // Reset the counter for the new cluster
        }
        balloon.clusterId = currentClusterId; // Assign the current cluster ID to the balloon
        balloonsInCurrentCluster++; // Increment the counter
        lastEndPosition = balloon.end; // Update the last position tracker
    }
    return balloons; // Return the re-clustered balloons
}

// Simulates the movement of balloons
void moveBalloons(std::vector<Balloon>& balloons) {
    for (auto& balloon : balloons) {
        int moveVariability = getRandomNumber(-5, 10); // Determine the movement distance with added randomness
        bool moveRight = getRandomNumber(0, 1) == 1; // Randomly decide the direction of the movement

        // Move the balloon based on the determined direction and distance
        if (moveRight) {
            balloon.start += moveVariability;
            balloon.end += moveVariability;
        } else {
            balloon.start -= moveVariability;
            balloon.end -= moveVariability;
        }

        // Ensure that balloons remain within the specified bounds after movement
        balloon.start = std::max(1, std::min(balloon.start, 100));
        balloon.end = std::max(1, std::min(balloon.end, 100));
    }
}

// Calculates the minimum number of arrows required to burst all balloons and calculates the total value gained from bursting these balloons
int findMinArrowShotsAndCalculateValue(std::vector<Balloon>& balloons, int& totalValue) {
    if (balloons.empty()) return 0; // If there are no balloons, no arrows are needed.

    // Re-sort the balloons by their end positions to prepare for the arrow calculation.
    // This ensures we can find the optimal points to shoot arrows to burst maximum balloons.
    std::sort(balloons.begin(), balloons.end(), [](const Balloon& a, const Balloon& b) {
        return a.end < b.end;
    });

    std::unordered_set<int> poppedClusters; // Tracks clusters that have already been burst.
    int arrowCount = 0; // Initialize arrow count to 0.

    // Iterate through each balloon to determine the minimum number of arrows required.
    for (const auto& balloon : balloons) {
        // If the current balloon's cluster hasn't been popped yet,
        // it means an arrow hasn't been shot through this balloon/cluster.
        if (poppedClusters.find(balloon.clusterId) == poppedClusters.end()) {
            poppedClusters.insert(balloon.clusterId); // Mark the cluster as popped.
            arrowCount++; // Increment the arrow count as we need an arrow for this cluster.
            // Add the value of all balloons in this cluster to the total value.
            for (const auto& b : balloons) {
                if (b.clusterId == balloon.clusterId) {
                    totalValue += b.value;
                }
            }
        }
    }
    return arrowCount; // Return the total number of arrows needed.
}

int main() {
    // Define the balloon templates with colors and values.
    std::vector<std::pair<std::string, int>> balloonTemplates = {
        {"Red", 200}, {"Orange", 175}, {"Yellow", 150},
        {"Green", 100}, {"Blue", 75}, {"Indigo", 50}, {"Violet", 25},
        {"Pink", 220}, {"Cyan", 180}, {"Magenta", 160},
        {"Lime", 140}, {"Maroon", 120}, {"Navy", 100},
        {"Olive", 80}, {"Teal", 60}, {"Purple", 40}, {"Silver", 30}
    };

    // Iterate through different cluster sizes to test the simulation's behavior.
    for (int clusterSizeTest = 2; clusterSizeTest <= 5; clusterSizeTest++) {
        std::cout << "Testing with max cluster size: " << clusterSizeTest << std::endl;

        // Generate initial balloons with random clusters.
        auto balloons = generateBalloonsWithClusters(balloonTemplates, 3);
        // Dynamically form clusters based on balloon positions and the specified max cluster size.
        balloons = generateAdaptiveClusters(balloons, clusterSizeTest);

        // Simulate balloon movement and calculate arrows needed for several time steps.
        for (int step = 0; step < 5; ++step) {
            std::cout << "Time step: " << step << std::endl;
            moveBalloons(balloons); // Simulate balloon movement.

            int totalValue = 0;
            int arrowsNeeded = findMinArrowShotsAndCalculateValue(balloons, totalValue);

            std::cout << "Total arrows needed: " << arrowsNeeded << std::endl;
            std::cout << "Total value gained: $" << totalValue << std::endl << std::endl;
        }
        std::cout << "---------------------\n";
    }
    return 0;
}
