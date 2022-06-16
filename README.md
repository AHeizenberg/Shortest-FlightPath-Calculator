# Shortest Flight Path Calculator :airplane:
The following is an implementation of the dijkstra's shortest path algorithm. In this implementation I alter the algorithm to calculate the shortest and fastest flight path between a take off and landing location given by the user. This is a completely dynamic program and can be used with any text based map.

## How to compile:

Commands:

    g++ shortest.cpp
    ./a.out map.txt


You can compile the file using the commands shown above. When you run the file please make sure to provide a text file that has an appropriate representation of a simplified map like the one provided in the repo. To make it easier for you I have provided a map.txt file which has this representation of the Canadian map.

## How to use the program:

When you run the program you will be prompted to enter 3 Canadian destinations using their airport code. Please note that in order for the program to run as desired these inputs must be correct 3 digit Canadian airport codes. Once you have entered the three correct airport codes the program will output a table showing the cost (total distance from source to destination), the number of hops (from source to destination) and the shortest path from the source to destination. In my program I have set the source to always be YYC (Calgary’s airport code) as we are situated in Calgary so all results will be from source YYC to whatever Canadian destinations that have been provided as inputs.
