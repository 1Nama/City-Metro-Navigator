#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int destination;
    double distance;
    double fare;
};

class Graph {
private:
    unordered_map<int, vector<Edge>> adjList;
    unordered_map<int, string> locationNames;
    unordered_map<string, int> locationIds;
    int nextId;

public:
    Graph() : nextId(1) {}  // Start IDs from 1

    void addLocation(const string& location) {
        if (locationIds.find(location) == locationIds.end()) {
            locationIds[location] = nextId;
            locationNames[nextId] = location;
            adjList[nextId] = vector<Edge>();
            nextId++;
        }
    }

    void addEdge(const string& source, const string& destination, double distance, double fare) {
        int srcId = locationIds[source];
        int destId = locationIds[destination];
        adjList[srcId].push_back({ destId, distance, fare });
        adjList[destId].push_back({ srcId, distance, fare });
    }

    void displayLocations() const {
        cout << "Locations:\n";
        for (const auto& location : locationNames) {
            cout << location.first << ": " << location.second << endl;
        }
        cout << endl; // Add a space after displaying locations
    }

    void findShortestPath(int start, int end) const {
        unordered_map<int, double> distances;
        unordered_map<int, int> previous;
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (const auto& location : adjList) {
            distances[location.first] = numeric_limits<double>::infinity();
            previous[location.first] = -1;
        }

        distances[start] = 0.0;
        minHeap.push(start);

        while (!minHeap.empty()) {
            int current = minHeap.top();
            minHeap.pop();

            if (current == end) {
                break;
            }

            for (const auto& edge : adjList.at(current)) {
                double newDist = distances[current] + edge.distance;
                if (newDist < distances[edge.destination]) {
                    distances[edge.destination] = newDist;
                    previous[edge.destination] = current;
                    minHeap.push(edge.destination);
                }
            }
        }

        if (distances[end] == numeric_limits<double>::infinity()) {
            cout << "No path from " << locationNames.at(start) << " to " << locationNames.at(end) << endl;
            return;
        }

        vector<int> path;
        for (int at = end; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        cout << "\nShortest path from " << locationNames.at(start) << " to " << locationNames.at(end) << ": \n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << locationNames.at(path[i]);
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << "\nDistance: " << distances[end] << " km\n" << endl;
    }

    void findCheapestPath(int start, int end) const {
        unordered_map<int, double> fares;
        unordered_map<int, int> previous;
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (const auto& location : adjList) {
            fares[location.first] = numeric_limits<double>::infinity();
            previous[location.first] = -1;
        }

        fares[start] = 0.0;
        minHeap.push(start);

        while (!minHeap.empty()) {
            int current = minHeap.top();
            minHeap.pop();

            if (current == end) {
                break;
            }

            for (const auto& edge : adjList.at(current)) {
                double newFare = fares[current] + edge.fare;
                if (newFare < fares[edge.destination]) {
                    fares[edge.destination] = newFare;
                    previous[edge.destination] = current;
                    minHeap.push(edge.destination);
                }
            }
        }

        if (fares[end] == numeric_limits<double>::infinity()) {
            cout << "No path from " << locationNames.at(start) << " to " << locationNames.at(end) << endl;
            return;
        }

        vector<int> path;
        for (int at = end; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        cout << "\nCheapest path from " << locationNames.at(start) << " to " << locationNames.at(end) << ": \n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << locationNames.at(path[i]);
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << "\nFare: " << fares[end] << " Rs\n" << endl;
    }
};

void displayMenu() {
    cout << "1. Display Locations\n";
    cout << "2. Find Shortest Path\n";
    cout << "3. Find Cheapest Path\n";
    cout << "4. Add Location\n";
    cout << "5. Exit\n";
}

int main() {
    Graph graph;

    // Welcome message
    cout << "Welcome to City Metro!\n\n";

    // Predefined locations and distances
    graph.addLocation("City Mall");
    graph.addLocation("Salon");
    graph.addLocation("Grocery Store");
    graph.addLocation("Restaurants");
    graph.addLocation("City Park");
    graph.addLocation("Stadium");

    graph.addEdge("City Mall", "Salon", 1.0, 0.5);
    graph.addEdge("City Mall", "Grocery Store", 2.0, 1.0);
    graph.addEdge("City Mall", "Restaurants", 3.0, 1.5);
    graph.addEdge("City Mall", "City Park", 4.0, 2.0);
    graph.addEdge("Salon", "City Park", 1.5, 1.0);
    graph.addEdge("Grocery Store", "Stadium", 2.5, 1.5);
    graph.addEdge("Restaurants", "Stadium", 3.5, 2.0);

    int choice;
    int start, end;
    string location, neighbour;
    double distance, fare;

    while (true) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                graph.displayLocations();
                break;
            case 2:
                cout << "Enter start location number: ";
                cin >> start;
                cout << "Enter end location number: ";
                cin >> end;
                graph.findShortestPath(start, end);
                break;
            case 3:
                cout << "Enter start location number: ";
                cin >> start;
                cout << "Enter end location number: ";
                cin >> end;
                graph.findCheapestPath(start, end);
                break;
            case 4:
                cout << "Enter new location: ";
                cin >> location;
                graph.addLocation(location);
                cout << "Enter number of neighbouring locations: ";
                int numNeighbours;
                cin >> numNeighbours;
                for (int i = 0; i < numNeighbours; ++i) {
                    cout << "Enter neighbour number " << i + 1 << ": ";
                    cin >> neighbour;
                    cout << "Enter distance to " << neighbour << " (km): ";
                    cin >> distance;
                    cout << "Enter fare to " << neighbour << " (Rs): ";
                    cin >> fare;
                    graph.addEdge(location, neighbour, distance, fare);
                }
                cout << endl; // Add a space after adding new location
                break;
            case 5:
                cout << "Exiting...\n";
                cout << "Thank you for using the City Metro system!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
