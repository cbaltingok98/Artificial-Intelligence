//
//  geneticAlgorithm.cpp
//  Artificial Intelligence
//
//  Created by Celik Berk Altingok on 3/7/20.
//  Copyright © 2020 Celik Berk Altingok. All rights reserved.
//
//
// Program will create a file called 'pathResult.txt' and will write everyting into that file!
// All of the generated paths and their costs will be displayed. Shortest path for each generation will be displayed ass well.
// At the bottom of the txt file you will find the result of this program. The shortest path and information about the path.

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

struct shortestInfo { // information about the shortest path found
    int populationNum; // from which population this path is found ex 16, 32, 64
    int generationNum; // what is the generation number ex, if population is 32, generation number will be between 0 - 200
    int generationInterval; // if population 16 this will be 100, if pop 32 this will be 200 and so on
    int bestCost; // cost of the path
    int totalGenerated = 0; // total number of path generations
    vector<int>path; // the shortest path
};

class Generic {
private:
    ofstream outputFile;
    vector<vector<int>> pathCost = {{0, 2, 11, 3, 18, 14, 20, 12, 5}, {2, 0, 13, 10 ,5, 3, 8, 20, 17}, {11, 13, 0, 5, 19, 21, 2, 5, 8}, {3, 10, 5, 0, 6, 4, 12, 15, 1}, {18, 5, 19, 6, 0, 12, 6, 9, 7}, {14, 3, 21, 4, 12, 0, 19, 7, 4}, {20, 8, 2, 12, 6, 19, 0, 21, 13}, {12, 20, 5, 15, 9, 7, 21, 0, 6}, {5, 17, 8, 1, 7, 4, 13, 6, 0}}; // path cost between the cities/points
    
    vector<int> city = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // default path/ starting path. this will randomly be mixed
    vector<int> newCity; // newCity and tempPath(line:42) will hold the randomly selected two paths to generate a new path.
    vector<vector<int>> generatedCities; // if population 16, this vector will have 16 randomly genareted paths. same for other populations
    //////
    vector<int> numOfPopulation = {16, 32, 64}; // first population will be 16 path, second 32 third 64. This is to see how the number of the population affects the outcome of the program.
    vector<int> numOfGeneration = {100, 200, 400}; // for population 16, there will be 100 generation. meaning that i will generate 16 paths 100 times then 32 paths for 200 times then 64 path for 400 times.
    vector<int> shortestPath; // this is the container that will save the shortes map after all the generation
    vector<int> queue1; // this will hold the duplicate values for one of child path generated from two parents
    vector<int> queue2; // this will hold the duplicate values for the other child path generated from two parents
    vector<int> tempPath; // this will hold a path that is randomly selected from the population
    int currentPopulation; // iterator for numOfPopulation
    int currentGeneration; // iterator for numOfGeneration
    int shortestPathCost; // cost of the shortest path found
    int count; // counter to display the generation number of that particular path
    shortestInfo*info = new shortestInfo;
public:
    Generic();
    ~Generic();
    void createNewPathCombination();
    void calculatePathCost();
    void geneticPaths();
    void mutateChild();
    void displayStats();
};

Generic::Generic(){
    currentPopulation = 0;
    currentGeneration  = 0;
    count = 0;
    shortestPathCost = INT_MAX;
    outputFile.open("pathResult.txt"); // results will be saved in this file.
    if(!outputFile){
        cout << "Error Creating file!" << endl;
    }
}
Generic::~Generic(){
    
}
void Generic::createNewPathCombination(){
    int randomNum, randomNum2, temp;
    srand (time(NULL));
    generatedCities.clear();
    newCity = city; // city is the default path. line:33
    outputFile << "Number of new parents : " << numOfPopulation.at(currentPopulation) << endl;
    
    for(int j = 0; j < numOfPopulation.at(currentPopulation); j++){ // two random indexes will be picked and swaped to create a new path.
        for(int i = 0; i < newCity.size(); i++){
            randomNum = rand() % 9 + 0;
            randomNum2 = rand() % 9 + 0;
            temp = newCity.at(randomNum);
            newCity.at(randomNum) = newCity.at(randomNum2);
            newCity.at(randomNum2) = temp;
        }
        temp = newCity.at(0); // number at the index 0
        newCity.push_back(temp); // number at the index zero will be pushed to the end of the path too to have the same starting and the ending point
        generatedCities.push_back(newCity); // all of the generated paths will be pushed to this vector
        newCity = city;
    }

    calculatePathCost();
    geneticPaths();
}

void Generic::calculatePathCost(){
    int currentPoint;
    int shortestCost = INT_MAX;
    int cost = 0;
    
    for(int i = 0; i < generatedCities.size(); i++){    /// CALCULATING THE PATH COST
        currentPoint = generatedCities.at(i).at(0);
        outputFile << generatedCities.at(i).at(0) << " -> ";
        
        for(int j = 1; j < generatedCities.at(i).size(); j++){
            cost += pathCost.at(currentPoint - 1).at((generatedCities.at(i).at(j)) - 1);
            outputFile << generatedCities.at(i).at(j) << " -> ";
            currentPoint = generatedCities.at(i).at(j);
        }                                               /// CALCULATING THE PATH COST
        outputFile << " Cost is : " << cost << endl; // SAVE TO FILE
        if(cost < shortestCost){ // IF LESS THAN THE CURRENT SHORTEST PATH, NEW PATH WILL BE THE SHORTEST PATH
            shortestCost = cost;
            shortestPath = generatedCities.at(i);
            if(shortestCost < shortestPathCost){ // INFORMATION ABOUT THE SHORTEST PATH WILL BE SAVED
                shortestPathCost = shortestCost;
                info->bestCost = shortestPathCost;
                info->generationNum = count;
                info->populationNum = numOfPopulation.at(currentPopulation);
                info->generationInterval = numOfGeneration.at(currentGeneration);
                info->path = shortestPath;
            }
        }
        cost = 0;
    }
    outputFile << "Shortest path : "; // THIS IS TO DISPLAY THE SHORTES PATH FOUND FOR THE CURRENT GENERATION
    for(int j: shortestPath){
        outputFile << j << " -> ";
    }
    outputFile << " Cost : " << shortestCost << endl;
}

void Generic::mutateChild(){
    int probability = rand() % 9 + 1;
    int pick, pick2, temp;
    
    if(probability < 3){ // %20 Chance of Mutation
        int numOfChange = rand() % 7 + 1; // random num of chances will be made
        for(int i = 0; i < numOfChange; i++){ // two random index will be swaped. starting and ending index is not included
            pick = rand() % 8 + 1;
            pick2 = rand() % 8 + 1;
            temp = newCity.at(pick);
            newCity.at(pick) = newCity.at(pick2);
            newCity.at(pick) = temp;
        }
    }
}

void Generic::geneticPaths(){
    int pick, pick2;
    int subStrSize;
    int temp;
    int left, right;
    
    vector<int> hodl;
    vector<vector<int>> newSetOfPaths;
    vector<int> keepTrack(10, 0);
    
    count = 0;
    
    for(int i = 0; i < numOfGeneration.at(currentGeneration); i++){ // for { 100, 200, 400 }
        for(int k = 0; k < numOfPopulation.at(currentPopulation); k++){ // for {16, 32 , 64 }
            
            info->totalGenerated++;
            
            pick = rand() % numOfPopulation.at(currentPopulation) + 1; // pick between 1-16 or 1-32 or 1-64  // RANDOMYL PICKING A PATH
            pick2 = rand() % numOfPopulation.at(currentPopulation) + 1; // pick between 1-16 or 1-32 or 1-64 // RANDOMLY PICKING A PATH
            subStrSize = rand() % 4 + 1; // SIZE OF THE STRING THAT WILL BE PICKED FROM THE PATH ( 1 - 4 )
            
            while(pick == pick2){
                pick2 = rand() % numOfPopulation.at(currentPopulation) + 1; // making sure we are not picking the same paths
            }
            
            newCity = generatedCities.at(pick - 1);
            tempPath = generatedCities.at(pick2 - 1);

            
            left = rand() % 8 + 1; // starting or the end point for substr to be chosen
            
            if(left + subStrSize > 8){ // if the chosen index plus the size of the substr greater than path size. chosen substr will get the left of the index other wise right of the chosen index(left)
                right = left;
                left = abs(left - subStrSize);
            } else {
                right = left + subStrSize;
            }

            for(int j = left; j <= right; j++){ // swaping the values
                temp = newCity.at(j);
                newCity.at(j) = tempPath.at(j);
                tempPath.at(j) = temp;
            }
            
            for(int j = left; j <= right; j++){ // looking for duplicates.      // 2 - 4 - 5 - 4 - 7 .... 4 will be pushed into a queue and 0 will
                for(int p = 0; p < newCity.size() - 1; p++){                    // take place as a flag -> 2 - 0 - 5 - 0 - 7
                    if(newCity.at(p) == newCity.at(j) && p != j){               // queue = 4,
                        queue1.push_back(newCity.at(p));                        // same operation will be done for both of the paths
                        newCity.at(p) = 0;                            // path1 = 1 - 2 - [1 - 2] - 3 - 1  just an example ignore ending point and substr
                    }                                                 // path2 = 3 - 5 - [5 - 4] - 4 - 3  [ ] this is the swaped substr i am not going to change them
                    if(tempPath.at(p) == tempPath.at(j) && p != j){   // lets say these are the paths after swaping values, in my program i chose path1
                        queue2.push_back(tempPath.at(p));            // to be my new child. queue1 = 2, queue2 = 5, 4,
                        tempPath.at(p) = 0;                          // path1 = 0 - 0 - 1 - 2 - 3 - 1 i have two values in queue2 which is 5 and 4 in ordr
                    }                                                // path2 = 3 - 0 - 5 - 4 - 0 - 3
                }                                                    // since my path1 will be my new child, i dont care about path2
            }                                                        // path1 will becom 5 - 4 - 1 - 2 - 3 - 1  then i will say end of the string = start
            for(int j = 0; j < newCity.size() - 1 && !queue2.empty(); j++){ // so path1 becomes 5 - 4 - 1 - 2 - 3 - 5 and this is my new child
                if(newCity.at(j) == 0) {
                    newCity.at(j) = queue2.front();
                    queue2.erase(queue2.begin());
                }
            }
            newCity.at(newCity.size() - 1) = newCity.at(0);
            
            mutateChild();
            queue1.clear();
            queue2.clear();
        
            newSetOfPaths.push_back(newCity); // hold new child paths
        }
        
            count++;
            generatedCities.clear();
            generatedCities = newSetOfPaths;
            newSetOfPaths.clear();
            outputFile << endl << "Generation : " << count << endl;
            calculatePathCost();
    }
    
    currentPopulation++;
    currentGeneration++;
    
    if(currentGeneration > 2 || currentPopulation > 2){ // end the program
        displayStats();
        return;
    }
    createNewPathCombination();
}

void Generic::displayStats(){ // Display the result!
    
    outputFile << endl << "////////////////////// STATS //////////////////////" << endl;
    outputFile << "Number of total generated paths : " << info->totalGenerated << endl;
    outputFile << "From the population of : " << info->populationNum << endl;
    outputFile << "From the generation of : " << info->generationInterval << endl;
    outputFile << "Generation Number : " << info->generationNum << endl;
    outputFile << "Path : ";
    
    for(int i: info->path){
        outputFile << i << " -> ";
    }
    
    outputFile << endl << "Cost : " << info->bestCost << endl;
}

int main(){
    Generic gen;
    gen.createNewPathCombination();
    cout << "Program Compiled!" << endl;
    cout << "Check the output file for the results!" << endl;
    cout << "Results are displayed with labels at the bottom of the file! (SCROLL DOWN TO BOTTOM)" << endl;
    return 0;
}
