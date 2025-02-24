/***********************************************************
  Synopsis: read from city.name and flight.txt
            store the graph in a adjacency matrix
***********************************************************/

#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <climits>
#include <queue>

#include "minheap_pair.h"

using namespace std;

class Graph{
     public:
          Graph(int d);
          ~Graph(){};
	  int get(){return n;}
	  int (* getG())[140]{return adj;}
	  
          void print();
          void addEdge(int node1, int node2){ adj[node1][node2] = 1; };
     private: 
          int n;
          int adj[140][140]; // You may want to use adjacent list implementation for sparse graph instead
};

Graph::Graph(int d){
    for (int i=0; i < d; i++){
        for (int j=0; j < d; j++){
            adj[i][j] = 0;
        }
    }
    n = d;
};

void Graph::print(){
    for (int i=0; i < n; i++){
        cout << i << " -> ";
        for (int j=0; j < n; j++){
            if (adj[i][j]){
                cout << j << " ";
            }
        }
        cout << endl;
    }

};

void routeSearch(Graph graph);
void routeSearch_1(Graph graph, int city_A, int city_B, int num_connection);
void routeSearch_2(Graph graph, int city_A, int city_B, int city_C, int city_D);
void Dijkstra(Graph graph, int s, int d[], int p[]);


/*to map a city name with an unique integer */
//map<const string, int, strCmp> city;
map<const string, int> city;

int main(int argc, char *argv[]){
   int i,j, node1, node2,n;
   string line;


   //cout << "Please enter the number of cities in your graph: " << endl;
   //cout << "---------------------------------------------------" << endl;
   n = 140;
   //cin >> n;
   Graph graph(n);
 
   char lineChar[256];

   ifstream cityFile;
   cityFile.open("city.name", ios::in);

   /*map a city name with an unique integer */
   for (i = 0; i < n; i++){
      std::getline(cityFile, line);
      line.erase(std::prev(line.end()));
      city[line] = i;
   }

   cityFile.close();

   ifstream flightFile;
   flightFile.open("flight.txt", ios::in);
  
   while (flightFile.getline(lineChar, 256)){
      // if line constains From: 
      line = lineChar;
      if (line.find("From:", 0) == 0 ){
          line.erase(0,7);
	  //printf("%s\n", line.c_str());
          if(city.find(line) != city.end()){
	  	//cout << "locate!" << line <<  line.length() << endl;
	  }
	  else{
	  	//cout << "NO!" << line <<  line.length() << endl;
		continue;
	  }
          node1 = city[line];
      } else {
	  line.erase(0,7);

          node2 = city[line];
          graph.addEdge(node1,node2);
      }
   }
   flightFile.close();
   
   /* print the graph */
   cout << endl << "The graph generated can be represented by the following adjacent matrix : " << endl;
   cout << "-----------------------------------------------------------------------------------" << endl;
   graph.print();

   /*print the map representation of cities*/
   map<const string, int>::iterator it;  
   for(it = city.begin(); it != city.end(); it++){
	cout << it->first << ": " << it->second << endl; 
   }

   routeSearch(graph);
}

void routeSearch(Graph graph){
	int choice;
	cout << "Please choose the type of the questions: " << endl;
	cout << "1: From city 'A' to city 'B' with less than x connestions?" << endl;
	cout << "2: Route with the samllest number of connections from city 'A' to city 'D' through city 'B' and 'C'?" << endl;	
	cin >> choice;

	int city_A, city_B, city_C, city_D;
	int num_connection;

	switch (choice){
		case 1:
			cout << "Please enter the city A: ";
			cin >> city_A;

			cout << "Please enter the city B: ";
			cin >> city_B;

			cout << "number of connections: ";
			cin >> num_connection;

			routeSearch_1(graph, city_A, city_B, num_connection);
			break;	
		case 2:
			cout << "Please enter the city A: ";
			cin >> city_A;

			cout << "Please enter the city D: ";
			cin >> city_D;

			cout << "Please enter the city B: ";
			cin >> city_B;

			cout << "Please enter the city C: ";
			cin >> city_C;

			routeSearch_2(graph, city_A, city_B, city_C, city_D);
			break;	
		
		default:
			cout << "incorrect choice" << endl;	
	}


}
//Dijkstra algorithm for finding the SSSP
void Dijkstra(Graph graph, int s, int d[], int p[]) {
	int n = graph.get();
	int (*mat)[140] = graph.getG();
	for(int i = 0; i < graph.get(); i++){
		d[i] = INT_MAX;
		p[i] = -1;	
	}
	d[s] = 0;
	//create a minheap
	MinPriorityQueue pq;
	for(int i = 0; i < n; i++){
		pq.insert(d[i], i);
	}
	while(!pq.is_empty()){
		int u = pq.extract_min();
		//Graph is unconnected
		if (d[u] == INT_MAX)
			break;
		for(int i = 0; i < n; i++){
			if(mat[u][i]){
				if(d[i] > d[u] + 1){
					d[i] = d[u] + 1;
					pq.decrease_key(i, d[i]);
					p[i] = u;
				}
			}
		}
	}		
}
//Task 1
void routeSearch_1(Graph graph, int city_A, int city_B, int num_connection) {
	int n = graph.get();
	int d[n], p[n];
	vector<int> ans;
	Dijkstra(graph, city_A, d, p);
	if(d[city_B] <= num_connection){ //there are less than x connections
		int cur_city = city_B;
		int i;
		while(cur_city != -1) {
			ans.insert(ans.begin(), cur_city);
			cur_city = p[cur_city];
		}
		for(i = 0; i < ans.size(); i++){
			if(i != ans.size() - 1){
				cout << "city" << ans[i] << " to ";
			}
			else{
				cout << "city" << ans[i];
			}
		}
		cout << endl;
		cout << "Total connection: " << i-1 << endl;
	}
	else{
		//can not fly from A to B with less than x connections
		cout << "No such route." << endl; 
	}
}

//Task 2
void routeSearch_2(Graph graph, int city_A, int city_B, int city_C, int city_D) {
    int n = graph.get();
    int d_AB[n], p_AB[n];
    int d_CD[n], p_CD[n];
    vector<int> ans_AB, ans_CD;

    // Find shortest paths from city_A to cities B and C
    Dijkstra(graph, city_A, d_AB, p_AB);

    // Find shortest paths from cities B and C to city_D
    Dijkstra(graph, city_D, d_CD, p_CD);

    // Combine the two paths (A -> B and A -> C -> D) to form the final route
    if (d_AB[city_B] != INT_MAX && d_CD[city_C] != INT_MAX) {
        int cur_city = city_B;
        while (cur_city != city_A) {
            ans_AB.insert(ans_AB.begin(), cur_city);
            cur_city = p_AB[cur_city];
        }

        cur_city = city_C;
        while (cur_city != city_D) {
            ans_CD.push_back(cur_city);
            cur_city = p_CD[cur_city];
        }

        ans_AB.insert(ans_AB.end(), ans_CD.begin(), ans_CD.end());

        // Print the final route
        cout << "Route from City " << city_A << " to Cities " << city_D << " through Cities " << city_B << " and " << city_C << ": ";
        for (int city : ans_AB) {
            cout << "city" << city << " -> ";
        }
        cout << "city" << city_D << endl;
        cout << "Total connections: " << ans_AB.size() - 1 << endl;
    } else {
        // No route found from A to D via B and C
        cout << "No such route." << endl;
    }
}
}

