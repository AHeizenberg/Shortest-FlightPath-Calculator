
#include <limits>
#include<list>
#include<vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stdint.h>
#include <climits>
#include<map>


using namespace std;  

#define Node pair<int,int> 

void table(vector<int> dsts, vector<int> dist, vector<int> parent);
void printPath(vector<int> parent, int j, int end, int &hops);
vector<string> create_vertex(const char *file);
void shortestPath(int src, vector<int> dest);

// This vector is where we will store all the airport codes.
// We will use the index of an airport code 
//as a numerical representation for that code
// as it is much easier to create an adj_list 
//and implement the shortest path algo with numerical values for nodes
vector<string> vertex;

// adj list will be a vector of pair<int,int> -> Node
// Here the index will be the src vertex
// the second int of the pair a neighbour vertex of src vertex
// and the first int of the pair will be the weight (aka travel distane) between the src and destination vertex.
// Since a src vertex can have many neighbours all those will be stored in vector 
vector<Node> *adj_list;

// This method creates a vertex vector with all the different Airport codes
// Not that there are no repeats in this array only unique airport codes
vector<string> create_vertex(const char *file){
    
    ifstream map;
    // open provided file
    map.open(file);

    vector<string> result;
    string word;

    // Format in file -> Src Dst Weight
    while(map >> word){
        // item not found -> Src 
        if( (find(result.begin(), result.end(), word) != result.end()) == false ){
            result.push_back(word);
        }
        // item not found -> Dst
        map >> word;
        if( (find(result.begin(), result.end(), word) != result.end()) == false ){
            result.push_back(word);
        }
        // We dont care about storing weight here so just skip it
        map >> word;

    }
    return result;
}


// Implementaiton of dijkstra's algorithm
// src is the source vertex (always YYC in our case)
// dest is a vector that includes all the destinations for which we require the shortest path
void shortestPath(int src, vector<int> dest)   
{  
    // create a parent vector to keep track of the parents
    // Here index represents the actual vertex
    // And the int stored for that index it's parent
    vector<int> parent;

    // to store the distance
    // index is the node and value is the distance from src to that node   
    vector<int> distance;

    // initally fill the parent and distance vectors
    // Here the size of these is the same as the size of vertex array
    for(int i = 0; i < vertex.size(); i++){
        // The source vertex always has the distance 0
        if(i == src){distance.push_back(0);}
        // Make them all equal to negative as our nodes are all +'ve ints
        parent.push_back(-1);
        // Make the distances to all nodes from src initially infinity (largest INT value in this case)
        distance.push_back(INT_MAX);
    }

    // This is the vector is like a priority queue
    vector< pair<int,int> > mp;

    // Push source element to vector as we will assess all its neighbours first
    mp.push_back(make_pair(0,src));

    cout << "Destination\tCost\tHops\tShortest Path" << endl;
    int runs = 0;

    // While we still have destination vertex's to consider
    while (dest.size() > 0) { 
        runs ++;
        // The vector mp is sorted in descending order according to weight
        // So the last element is the smallest weight vertex and we that for the shortest path
        int curr_node = mp.back().second;
        // Since we are accessing this node we can remove it from the queue vector
        mp.pop_back();

        // Check all the neighbours of the current node and try to mininze their weights if possible 
        // This process is known as relaxation 
        for (int i = 0; i < adj_list[curr_node].size(); i++) {  
            
            pair<int,int> nbr = adj_list[curr_node][i];
            int nbr_vertex = nbr.first;
            int nbr_weight = nbr.second;
            int nbr_distance = distance[nbr_vertex];
            int curr_distance = distance[curr_node];

            // Check if the path from the current vertex to destination is shorter than the current path to the neighbouring vertex
            if (curr_distance + nbr_weight < nbr_distance) {  
                
                // since we have choosen this route update the parrent of neighbour to the current vertex 
                parent[nbr_vertex] = curr_node;

                // Update the distance of the neighbour
                distance[nbr_vertex] = curr_distance + nbr_weight;

                //push this vertex to the queue so it can be considered
                // Here we have a sorted vector for the queue so it doesnt matter if we have repeated vectors 
                // As the one with the smallest distance will always be considered before
                mp.push_back(make_pair(distance[nbr_vertex], nbr_vertex));
                
            }  
        }
        // Sort the vector in descending order using lambda function
        // lambda function (lines 146-148) based from:
        // https://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
        sort(mp.begin(), mp.end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
            return left.first > right.first;
        });
        // Check if the current assessed vertex was one of the destination vertex's
        for(int i = 0; i < dest.size(); i++){
            if(dest[i] == curr_node){
                // if so print out the path to it and remove it from the destination vector
                int hops = 0;
                cout << vertex[curr_node] << "\t\t" << distance[dest[i]] << "\t";
                printPath(parent,curr_node,curr_node,hops);
                dest.erase(remove(dest.begin(), dest.end(), curr_node), dest.end());
            }
        }
    }
    cout << "runs " << runs << endl;  
}

// Recursive function that prints the shortest path
// and the num of hops from a source vertex to a destination vertex
void printPath(vector<int> parent, int j, int end, int &hops){   
     // Base Case as the source will have no parent it's value will always be -1
    if (parent[j]== -1){
        // print the num of hops taken to get to source (base case) and print the airport code of the source
        cout << hops << "\t" << vertex[j] << "-->";
        return;
    }
    // increment hops before each recursive call
    hops++;
    // recursively call this function until we get the full path (aka we reach the base case -> src vertex)
    printPath(parent, parent[j], end, hops);

    // This is just print formating
    if(vertex[j] == vertex[end]){
    cout << vertex[j] << endl;
    } else {
       cout << vertex[j] << "-->"; 
    }
}

int main(int argc, const char *argv[])  
{  
    // create the vertex vector from the given text file
    vertex = create_vertex(argv[1]);

    // vector to store inputs
    vector<int> inputs;
    // adj_list; type -> vector<vector<Node>>
    adj_list = new vector<Node>[vertex.size()];

    //collect the 3 inputs and store the numerical representations of these airport codes into inputs vector
    for(int i = 0; i < 3; i++){
        string code;
        cout << "Enter canadian destation no." << i+1 << " (airport code): ";
        cin >> code;

        vector<string>::iterator it = find(vertex.begin(), vertex.end(), code);
        int num = distance(vertex.begin(), it);
        inputs.push_back(num);

    }

    // create file stream
    ifstream map_file;

    // open provided file
    map_file.open(argv[1]);

    // for(int i = 0; i < vertex.size(); i++){
    //     cout << i << " " << vertex[i] << endl;
    // }

    // check if file is opened
    if(!map_file){
        cerr << "Not able to open provided file";
        exit(1);
    }

    string word;

    while(map_file >> word){
        //store the source airport code
        string src = word;
        //cout << "Src: " << src << " ";

        // store the destination airport code
        map_file >> word;
        string dst = word;
        //cout << "dst: " << dst << " ";
       
        int weight;
         // store the weight (distance) between the two
        map_file >> word;

        // Convert the string into int and store it into weight
        stringstream ss;
        ss << word;
        ss >> weight;

        // Get the index (aka numerical representation of vertex) of the current src airport code
        vector<string>::iterator it = find(vertex.begin(), vertex.end(), src);
        int src_num = distance(vertex.begin(), it);

        // Get the index (aka numerical representation of vertex) of the current destination airport code
        it = find(vertex.begin(), vertex.end(), dst);
        int dst_num = distance(vertex.begin(), it); 

        // Undirected graph representation using adj_list
        // Therefore create two way links
        adj_list[src_num].push_back(make_pair(dst_num, weight));
        adj_list[dst_num].push_back(make_pair(src_num, weight));       

        //cout << "weight: " << weight << " ";
        //cout << endl;
    }
    
    // locate YYC's index as in our case YYC will be the start location
    vector<string>::iterator start = find(vertex.begin(), vertex.end(), "YYC");
    int start_num = distance(vertex.begin(), start); 

    // Find and print the shortest paths of all the inputs
    shortestPath(start_num,inputs);

    return 0;

}
