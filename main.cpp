#include <bits/stdc++.h>

using namespace std;

class Compare
{
public:
	

	int calcDistance(vector<int> &src, vector<int> &des)
	{
		return sqrt(pow(des[1] - src[1], 2) + pow(des[0] - src[0], 2));
	}
	bool operator()(vector<int> below, vector<int> above)
	{
		return below[2] > above[2];
	}	
};

int minDistance = INT_MAX;
vector<vector<int>> minPath;

struct Car
{
	int chargeCapacity;
	int chargeLeft;
	int mileage;
	int distanceFromSource;
	int distanceLeft = chargeLeft * mileage;
	int distanceCapacity = chargeCapacity * mileage;
};

void travel(vector<int> &src, vector<int> &destination, vector<vector<int>> &currentPath, Car &car){

	int distance = sqrt(pow(destination[1] - src[1], 2) + pow(destination[0] - src[0], 2));
	src = destination;
	currentPath.push_back(src);
	car.distanceCapacity -= distance;
	car.distanceLeft -= distance;
	
}

int calcDistance(vector<int> &src, vector<int> &des)
{
	return sqrt(pow(des[1] - src[1], 2) + pow(des[0] - src[0], 2));
}

void tracePath(vector<vector<vector<int>>>& weightMatrix, vector<int> &destination, vector<int> &currentNode)
{
	int hops = weightMatrix[currentNode[0]][currentNode[1]][0];
	
	int distanceTotal = calcDistance(currentNode, destination);
	vector<vector<int>> path;
	path.push_back(destination);
	path.push_back(currentNode);

	vector<int> prevNode = {weightMatrix[currentNode[0]][currentNode[1]][1], weightMatrix[currentNode[0]][currentNode[1]][2]};
	vector<int> curNode = {currentNode[0], currentNode[1]};

	for (int i = hops; i > 0; i--){
		
		distanceTotal += calcDistance(curNode, prevNode);
		path.push_back(prevNode);
		if(i == 1)
			break;
		curNode = prevNode;
		prevNode = {weightMatrix[curNode[0]][curNode[1]][1], weightMatrix[curNode[0]][curNode[1]][2]};
	}

	if(distanceTotal < minDistance){
		minPath = path;
		minDistance = distanceTotal;
	}
	
}

void explorePaths2(vector<vector<int>> &chargingStns, vector<int> &currentNode, vector<int> &destination, int hop, vector<vector<vector<int>>> &weightMatrix, Car &car)
{
	
	vector<vector<int>> nodesInRange;

	int distanceFromDes = calcDistance(currentNode, destination);
	if (distanceFromDes <= car.distanceCapacity)
	{
		tracePath(weightMatrix, destination, currentNode);
	}

	for (int i = 0; i < chargingStns.size(); i++){
		int distance = calcDistance(currentNode, chargingStns[i]);

		if (distance <= car.distanceCapacity && weightMatrix[chargingStns[i][0]][chargingStns[i][1]][0] == INT_MAX)
		{
			nodesInRange.push_back(chargingStns[i]);
		}
	}

	hop++;

	for (int i = 0; i < nodesInRange.size(); i++){

		if(weightMatrix[nodesInRange[i][0]][nodesInRange[i][1]][0] == INT_MAX){
			weightMatrix[nodesInRange[i][0]][nodesInRange[i][1]][0] = hop;
			weightMatrix[nodesInRange[i][0]][nodesInRange[i][1]][1] = currentNode[0];
			weightMatrix[nodesInRange[i][0]][nodesInRange[i][1]][2] = currentNode[1];
		}
	}

	for (int i = 0; i < nodesInRange.size(); i++){
		explorePaths2(chargingStns, nodesInRange[i], destination, hop, weightMatrix, car);
		// nodesInRange.pop();
	}
}

int main()
{
	Car myCar = {
		.chargeCapacity= 20,
		.chargeLeft = 10,
		.mileage = 1,
		// .distanceFromSource = 50,
		.distanceCapacity = 40
	};

	

	vector<int> source = {0,0};
	vector<int> destination = {100, 100};

	// Input source and destination coordinates
	cout << "Enter the coordinates of the source (x y): " << "\n";
	cin >> source[0] >> source[1];
	cout << "Enter the coordinates of the destination (x y): " << "\n";
	cin >> destination[0] >> destination[1];

	vector <vector<int>> chargingStns;

	int numStations;
	cout << "Enter the number of charging stations: ";
	cin >> numStations;

	// Input the coordinates of each charging station
	cout << "Enter the coordinates of each charging station (x y):" << endl;
	for (int i = 0; i < numStations; ++i)
	{
		int x, y;
		cout << "Charging Station " << (i + 1) << ": ";
		cin >> x >> y;
		chargingStns.push_back({x, y});
	}

	vector<vector<vector<int>>> weightMatrix(destination[0] + 1, vector<vector<int>>(destination[1] + 1, vector<int>(3, INT_MAX)));

	explorePaths2(chargingStns, source, destination, 0, weightMatrix, myCar);

	if(minDistance == INT_MAX) {
		cout << "No possible route found"
			  << "\n";
		return 0;
	}

	cout << "MIN DISTANCE -> " << minDistance << "\n";
	cout << "MIN PATH ";

	for (int i = minPath.size() - 1; i >= 0; i--){
		cout << " -> " << minPath[i][0] << ", " << minPath[i][1];
	}

	return 0;
}

