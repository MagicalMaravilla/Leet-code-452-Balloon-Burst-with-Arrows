// Imports and name spaces
#include <iostream>
#include <vector>
#include <algorithm>

// main algo function
int findMinArrowShots(std::vector<std::pair<int, int>>& points) {
    //Handling an empty input
    if (points.empty()) return 0;
    
    // Sorting balloons by their end position
    std::sort(points.begin(), points.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second;
    });

// Initialize counters 
    int arrowPos = points[0].second;
    int arrowCount = 1;

// Iterating through balloons
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].first <= arrowPos) {
            continue;
        } else {
            arrowCount++;
            arrowPos = points[i].second;
        }
    }

// Returning results
    return arrowCount;
}


// Main function and test cases
int main() {
    // Test case 1: Basic scenario
    std::vector<std::pair<int, int>> balloons1{{10,16}, {2,8}, {1,6}, {7,12}};
    std::cout << "Test case 1 - Expected: 2, Actual: " << findMinArrowShots(balloons1) << std::endl;

    // Test case 2: Overlapping balloons
    std::vector<std::pair<int, int>> balloons2{{1,2}, {2,3}, {3,4}, {4,5}};
    std::cout << "Test case 2 - Expected: 2, Actual: " << findMinArrowShots(balloons2) << std::endl;

    // Test case 3: Non-overlapping balloons
    std::vector<std::pair<int, int>> balloons3{{1,2}, {4,5}, {8,10}};
    std::cout << "Test case 3 - Expected: 3, Actual: " << findMinArrowShots(balloons3) << std::endl;

    // Test case 4: Edge case - Empty list of balloons
    std::vector<std::pair<int, int>> balloons4{};
    std::cout << "Test case 4 - Expected: 0, Actual: " << findMinArrowShots(balloons4) << std::endl;

    // Test case 5: Balloons with the same start and end points
    std::vector<std::pair<int, int>> balloons5{{1,1}, {2,2}, {3,3}};
    std::cout << "Test case 5 - Expected: 3, Actual: " << findMinArrowShots(balloons5) << std::endl;

    // Test case 6: Balloons nested within each other
    std::vector<std::pair<int, int>> balloons6{{1,10}, {2,9}, {3,8}};
    std::cout << "Test case 6 - Expected: 1, Actual: " << findMinArrowShots(balloons6) << std::endl;

    return 0;
}