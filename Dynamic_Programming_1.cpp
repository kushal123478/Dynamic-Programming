/* Author : Kushal Patel
Date : 2/7/2019
 Purpose: Practice C++ and planning algorithms*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <limits>
#include <algorithm>
using namespace std;


class DynamicProgramming {
// This class is defined for solving the shorted path problem using Dynamic programming algorithm


private:
	struct RawData {
		// Stores the data required to solve dynamic programming
		int vertex_count, start_vertex, end_vertex;
		vector<int> start;
		vector<int> end;
		vector<double> dist;
	};
	RawData data;
	vector<vector <double>> costArray;
	double optimalCost;
	vector<int> optimalPath;

public:
	void getdata(string filename) {
		// Takes in the filename and captures the data required from the file
		
		// creating filestream and opening the file
		fstream fs; 
		fs.open("input.txt", fstream::in);
		if (!fs.is_open()) {
			cout << "Could not open file!";
		}

		// declaring the variables and getting ready for file parsing
		char c;
		char line[20];
		int i = 0;
		int b_start, b_end;
		float b_dist;

		// parsing the file and storing all the required data in struct data of the class DynProg
		while (fs.is_open()) {
			fs >> data.vertex_count >> data.start_vertex >> data.end_vertex;

			while (fs.getline(line, ' ')) {
				fs >> b_start >> b_end >> b_dist;
				data.start.push_back(b_start);
				data.end.push_back(b_end);
				data.dist.push_back(b_dist);
				//cout << data.start[i] << " " << data.end[i] << " " << data.dist[i] << endl;
				i++;
			}
			fs.close();	
		}
	};

	void createCostArray() {
		vector < vector <double>> costarr(data.vertex_count+1, vector<double>(data.vertex_count+1, 0));
		
		// Updating the cost array
		for (int i = 0; i < data.start.size(); i++) {
			costarr[data.start[i]][data.end[i]] = data.dist[i];
		}
		// Filling others costs to infinity
		for (int i = 0; i < costarr.size(); i++) {
			
			for (int j = 0; j < costarr[i].size(); j++) {
				if (i != j && costarr[i][j] == 0) {
					costarr[i][j] = numeric_limits<double>::infinity();
				}
			}
		}
		costArray = costarr;
	};

	void findOptimal() {
		vector<vector<double>> V(data.vertex_count+1, vector<double>(data.vertex_count+1, numeric_limits<double>::infinity()));
		vector<vector<int>> B(data.vertex_count+1, vector<int>(data.vertex_count+1, 0));
		V[data.start_vertex][1] = 0;
		double Infi = numeric_limits<double>::infinity();
		
		for (int k = 2; k < data.vertex_count+1; k++) {
			
			for (int i = 1; i < data.vertex_count + 1; i++) {
				vector<int> x;
				int delta = 0;
				
				for (int kau = 1; kau < data.vertex_count; kau++) {
					if (costArray[i][kau] != Infi && costArray[i][kau] != 0) {
						delta++;
						x.push_back(kau);
					}
				}

				vector<double> tempV(delta, 0);

				for (int j = 0; j < delta; j++) {
					tempV[j] = costArray[i][x[j]] + V[x[j]][k - 1];
				}

				vector<double> tempV_n;
				tempV_n = tempV;
				
				for (int i1 = 1; i1 < k; i1++) {
					tempV_n.push_back(V[i][i1]);
				}

				V[i][k] = tempV_n[min_element(tempV_n.begin(), tempV_n.end()) - tempV_n.begin()];
				
				int mina = min_element(tempV.begin(), tempV.end()) - tempV.begin();
				
				B[i][k] = x[mina];
			}
		}
		
		vector<double> fc = V[V.size()-1];
		optimalCost = fc[min_element(fc.begin(), fc.end()) - fc.begin()];
		
		optimalPath.push_back(data.end_vertex);
		int rota = data.end_vertex;
		while (rota != data.start_vertex) {
			optimalPath.push_back(B[rota][B.size()-1]);
			rota = B[rota][B.size() - 1];
		}

	}

	void showOptPath() {
		cout << "The optimal path is::" << endl;
		for (int t = 0; t < optimalPath.size(); t++) {
			cout << optimalPath[t] << " ";
		}
		cout << endl;
	}
	void showOptCost() {
		cout << "The optimal cost to go from " << data.start_vertex << " to " << data.end_vertex << " is " << optimalCost << endl;
	}
};

int main() {
	DynamicProgramming dynprog;
	dynprog.getdata("input.txt");
	dynprog.createCostArray();
	dynprog.findOptimal();
	dynprog.showOptCost();
	dynprog.showOptPath();

	return 0;
}