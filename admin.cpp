#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <climits>
using namespace std;
 map<string, vector<pair<string, int> > > graph;
string filename = "E:\\VS Code\\Fleet\\graph_data.txt";
//---------------------------------LOGIN------------------------------------//
void Admin_Login(string name, string password) {
    cin.ignore();
    cout << "Enter Admin name: ";
    getline(cin, name);

    cout << "Enter password: ";
    getline(cin,password);

    if (name=="Admin123"&&password=="Admin@#123") {
        cout << "Login Successful!" << endl;
    } else {
        cout << "Login Unsuccessful! Invalid Credentials!" << endl;
        name = ""; // Reset logged-in status if login fails
    }
}

//-------------------------------------Route Management-------------------------------------//

#include <chrono>
#include <thread>


void trackRoute(const map<string, vector<pair<string, int> > >& graph, const string& startCity, const string& endCity) {
    if (graph.find(startCity) == graph.end() || graph.find(endCity) == graph.end()) {
        cout << "One of the cities not found in the graph.\n";
        return;
    }


    int totalDistance = 0;
    for (const auto& neighbor : graph.at(startCity)) {
        if (neighbor.first == endCity) {
            totalDistance = neighbor.second;
            break;
        }
    }
    if (totalDistance == 0) {
        cout << "No direct route found between " << startCity << " and " << endCity << ".\n";
        return;
    }


    for (int progress = 0; progress <= 100; progress += 10) {
        cout << "Tracking route from " << startCity << " to " << endCity << ": " << progress << "% completed.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Slow down for visibility
    }

    cout << "Destination " << endCity << " reached!\n";
}

bool routeExists(const map<string, vector<pair<string, int> > >& graph, const string& city1, const string& city2) {
    if (graph.find(city1) != graph.end()) {
        for (const auto& neighbor : graph.at(city1)) {
            if (neighbor.first == city2) {
                return true;
            }
        }
    }
    return false;
}

void addRoute(map<string, vector<pair<string, int> > >& graph, const string& city1, const string& city2, int distance, bool suppressWarning = false) {
    if (routeExists(graph, city1, city2)) {
        if (!suppressWarning) {
            cout << "Warning: Route between " << city1 << " and " << city2 << " already exists. Not adding duplicate route." << endl;
        }
    } else {
        graph[city1].push_back(make_pair(city2, distance));
        graph[city2].push_back(make_pair(city1, distance));
        if (!suppressWarning) {
            cout << "Route added between " << city1 << " and " << city2 << "." << endl;
        }
    }
}


void displayGraph(const map<string, vector<pair<string, int> > >& graph) {
    for (const auto& city : graph) {
        cout << city.first << " to ";
        for (const auto& neighbor : city.second) {
            cout << "(" << neighbor.first << ", " << neighbor.second << " kilometers) "<<endl;
        }
        cout << endl;
    }
}


void saveGraphToFile(const map<string, vector<pair<string, int> > >& graph, const string& filename) {
    ofstream outfile(filename);
    if (outfile.is_open()) {
        for (const auto& city : graph) {
            for (const auto& neighbor : city.second) {
                outfile << city.first << " " << neighbor.first << " " << neighbor.second << endl;
            }
        }
        outfile.close();
        cout << "Graph saved to file." << endl;
    } else {
        cout << "Error opening file to save graph." << endl;
    }
}


void loadGraphFromFile(map<string, vector<pair<string, int> > >& graph, const string& filename) {
    ifstream infile(filename);
    if (infile.is_open()) {
        string city1, city2;
        int distance;
        while (infile >> city1 >> city2 >> distance) {

            addRoute(graph, city1, city2, distance, true);
        }
        infile.close();
        cout << "Graph loaded from file." << endl;
    } else {
        cout << "Error opening file to load graph, or file doesn't exist." << endl;
    }
}


void dijkstra(const map<string, vector<pair<string, int> > >& graph, const string& startCity, const string& endCity) {

    if (graph.find(startCity) == graph.end()) {
        cout << "Start city not found in the graph.\n";
        return;
    }
    if (graph.find(endCity) == graph.end()) {
        cout << "End city not found in the graph.\n";
        return;
    }


    priority_queue<pair<int, string>, vector<pair<int, string> >, greater<pair<int, string> > > pq;


    map<string, int> distances;
    

    map<string, string> parents;


    for (const auto& city : graph) {
        distances[city.first] = INT_MAX;
    }
    distances[startCity] = 0;

   
    pq.push(make_pair(0, startCity));

    while (!pq.empty()) {
        string currentCity = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();


        for (const auto& neighbor : graph.at(currentCity)) {
            string nextCity = neighbor.first;
            int edgeWeight = neighbor.second;
            int newDist = currentDist + edgeWeight;


            if (newDist < distances[nextCity]) {
                distances[nextCity] = newDist;
                parents[nextCity] = currentCity;
                pq.push(make_pair(newDist, nextCity));
            }
        }
    }


    if (distances[endCity] == INT_MAX) {
        cout << "No path found between " << startCity << " and " << endCity << ".\n";
    } else {
        cout << "Shortest distance between " << startCity << " and " << endCity << ": " << distances[endCity] << " kilometers.\n";

        
        vector<string> path;
        for (string at = endCity; at != ""; at = parents[at]) {
            path.push_back(at);
            if (at == startCity) break;  
        }

        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; --i) {
            cout << path[i];
            if (i > 0) cout << " -> ";
        }
        cout << endl;
    }
}



// int main() {
//     map<string, vector<pair<string, int> > > graph;
//     string filename = "E:\\VS Code\\Fleet\\graph_data.txt";

//     // Load graph from file if it exists
//     loadGraphFromFile(graph, filename);

//     int mainChoice;
//     do {
//         cout << "Main Menu:\n";
//         cout << "1. Route Management\n";
//         cout << "2. Fleet Management\n";
//         cout << "3. Exit\n";
//         cout << "Enter your choice: ";
//         cin >> mainChoice;

//         if (mainChoice == 1) {
//             int routeChoice;
//             do {
//                 cout << "\nRoute Management Menu:\n";
//                 cout << "1. Add Route\n";
//                 cout << "2. Display Routes\n";
//                 cout << "3. Find Shortest Path (Dijkstra's Algorithm)\n";
//                 cout << "4. Exit to Main Menu\n";
//                 cout << "Enter your choice: ";
//                 cin >> routeChoice;

//                 if (routeChoice == 1) {
//                     string city1, city2;
//                     int distance;
//                     cout << "Enter first city: ";
//                     cin >> city1;
//                     cout << "Enter second city: ";
//                     cin >> city2;
//                     cout << "Enter distance between " << city1 << " and " << city2 << ": ";
//                     cin >> distance;

//                     // Add the route
//                     addRoute(graph, city1, city2, distance);
//                 } else if (routeChoice == 2) {
//                     // Display the graph
//                     displayGraph(graph);
//                 } else if (routeChoice == 3) {
//                     // Find shortest path
//                     string startCity, endCity;
//                     cout << "Enter the start city: ";
//                     cin >> startCity;
//                     cout << "Enter the end city: ";
//                     cin >> endCity;
//                     dijkstra(graph, startCity, endCity);
//                 }

//             } while (routeChoice != 4);
//         } else if (mainChoice == 2) {
//             int fleetChoice;
//             do {
//                 cout << "\nFleet Management Menu:\n";
//                 cout << "1. Add Vehicle\n";
//                 cout << "2. Remove Vehicle\n";
//                 cout << "3. Display All Vehicles\n";
//                 cout << "4. Exit to Main Menu\n";
//                 cout << "Enter your choice: ";
//                 cin >> fleetChoice;

//                 if (fleetChoice == 1) {
//                     addVehicle();
//                 } else if (fleetChoice == 2) {
//                     removeVehicle();
//                 } else if (fleetChoice == 3) {
//                     displayAllVehicles();
//                 }

//             } while (fleetChoice != 4);
//         }
//     } while (mainChoice != 3);

//     // Save the graph to file before exiting
//     saveGraphToFile(graph, filename);

//     return 0;
// }