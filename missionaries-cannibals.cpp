//
//  Artificial Intelligence
//  Missionaries and Cannibals Implementation
//
//  Created by Celik Berk Altingok on 2/13/20.
//  Copyright © 2020 Celik Berk Altingok. All rights reserved.
//

// I am starting with a root node that has the state : {3, 3, 0, 0}. First two index is the island 1, last two index
// is the island 3.
// { 3, 3, 0, 0 }
//   M  C  M  C
//   -1  |  1
// island is -1, island 2 is 1.
// starting at the root node I am creating 5 childNodes and another five childNodes for the successful nodes until i
// reach to the goal state.
// Breadth First Search
// first in queue is root node, only the uniqueue and legal nodes will be added to the queue
// Legal means, num of M >= C is Legal, num of M < C is illegal.

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

struct Node {
    vector<int> state; // node's state
    vector<Node*> nodes; // child nodes of this node
    int boat; // location of the boat
    bool legal; // if state is legal such as, num of M is >= C then legal(true), else illegal(false)
    string log;
};

class Tree {
private:
    Node* head; // Root Node
    vector<Node*> fullHistory; // All created nodes
    vector<Node*> uniqueHistory; // All of the unique nodes
    vector<Node*> passedNodes; // queue of passed nodes, Nodes that are unique and legal ,front of the queue will be erased to move to a different node
    vector<Node*> legalStates; // Record of the passed, legal, states.
    vector<int> goalState = {0, 0, 3, 3}; // Goal State
    int numOfTotalNodes; // Num of created nodes
public:
    Tree(); // Constructor
    void iterate(); // move to the next node in the queue
    bool createNewState(Node*tempNode); // create new states as nodes as a child to the parent(node at the front of the queue) node
    void print(); // Print information
    ~Tree(); // Deconstructor
};

Tree::Tree(){
    head = new Node;
    head->state = {3, 3, 0, 0}; // First index M, secon C, first two index (0 & 1) is the //first island, index 2 & 3 is the second island which is initially empty
    head->boat = -1; // -1  left island, +1 right island, will be multiplied by -1 to move the boat
    head->legal = true; // Initial node (3,3,0,0) is legal so marked as legal
    numOfTotalNodes = 1;
    uniqueHistory.push_back(head); // since head node is unique, push to history
    passedNodes.push_back(head); // since head node is legal and unique, push to passedNodes
    legalStates.push_back(head); // since head node is legal, push to legalstates
    fullHistory.push_back(head); // no matter what, push all nodes to full history
}
Tree::~Tree(){
    for(Node*n: fullHistory){ // since all the created nodes address's saved in fullhistory, use it to delete all
        delete n;
    }
}
void Tree::print(){ // Show how many nodes created and what kind
    
    cout << "Full Hist Count : " << fullHistory.size() << endl;
   
    cout << "Unique Count : " << uniqueHistory.size() << endl;
    
    cout << "Legal & Unique Count : " << legalStates.size() << endl;
//    cout << numOfTotalNodes << endl;
}
void Tree::iterate(){ // iterate through the nodes that are in a queue
    Node*tempNode = head; // start with the root node
    bool found = false; // booleand to indicate whether we have reached to our goal or not.
    
    while(tempNode != nullptr && !found){
        
        found = createNewState(tempNode); // if state == goalstate then return TRUE else FALSE
        
        passedNodes.erase(passedNodes.begin()); // Remove the node in front of the queue
        tempNode = passedNodes.front(); // tempNode equals to the node in front of the queue
        /// ONLY LEGAL AND UNIQUE NODES WILL BE CHECKED
    }
}
bool Tree::createNewState(Node* tempNode){
    // tempNode is always the parent node of new nodes
    Node*newState = new Node;
    newState->state = tempNode->state; // Node to hold the copy of the state of the parent node
    tempNode->nodes.push_back(newState); // Add new node to TempNode's child node array.
    
    bool boatMoved, flag;
    int leftIndexM, rightIndexM;
    int leftIndexC, rightIndexC;
    bool check1, check2, check3, check4;
    // Example State : state = {3, 3, 0, 0}
    //                          M  C  M  C
    // Island        :           -1 | 1
    // Indicies      :          0  1  2  3
    leftIndexM = 0;
    rightIndexM = 2;
    leftIndexC = 1;
    rightIndexC = 3;
    
    int pickCombination = 1; // there are five combinations that are being perforem for each state
    
    //////////////// COMBINATIONS
    /// 1 PASS 2M, 0C
    /// 2 PASS 0M, 2C
    /// 3 PASS 1M, 0C
    /// 4 PASS 0M, 1C
    /// 5 PASS 1M, 1C
    /// IF NOT POSSIBLE TO PASS, BOAT WILL CHANGE IT'S SHORE AND TRY ONE MORE TIME, IF NOT
    /// POSSIBLE AGAIN, IT WILL MOVE ON TO THE NEXT COMBINATION
    ////////////////
    /// IF NUM OF M < C IN ANY OF THE SIDES OF THE ISLAND, NODE WILL BE TAGGED AS ILLEGAL,
    /// OTHERWISE IT WILL BE MARKED AS LEGAL
    ////////////////
    /// EACH NODE'S ADDRESS IS STORED IN 'FULL HISTORY', FIRST ONE STORED IN 'HISTORY',
    /// EVERY NEW NODE WILL BE ADDED TO 'FULL HISTORY'
    /// EVERY NEW NODE WITH UNIQUE STATE WILL BE ADDED TO 'HISTORY'
    /// EVERY NEW NODE WITH UNIQUE AND LEGAL STATE WILL BE ADDED TO 'PASSEDNODES'
    /// EVERY NEW NODE WILL BE ADDED TO THE ARRAY OF THE PARENT NODE AS A CHILD NODE
    
    while(pickCombination <= 5){
        fullHistory.push_back(newState); // keep a track of all the nodes created to delete later
        cout << "--------------- Node# : " << numOfTotalNodes << endl;
        for(int n: newState->state){ // state from parent node, each child node starts with parent state
            cout << n << ", ";
        } // display parent state to later on compare with the new one. Visually helps, NO OTHER USE
        cout << endl;
        newState->boat = tempNode->boat; // new node will pick up from where parent left the boat.
        newState->legal = true; // start as true, if certain criteria found, it will be false;
        numOfTotalNodes++; //num of total nodes
        boatMoved = false; // boolean to indicate if boat is moved
        flag = false;
        switch (pickCombination) {
            case 1: ////   PICK 2M, 0C
                while(!boatMoved){
                    if(flag){
                        boatMoved = true;
                    }
                    if(newState->boat == -1){ //Move from left to right island
                        if(newState->state.at(leftIndexM) >= 2){ // check if there is enough M (2M in this case) to move
                            // if there is not enough to move from left to right, boat will be moved to right as empty
                            /////////////////////
                            newState->state.at(leftIndexM) -= 2; // move 2 M from left
                            newState->state.at(rightIndexM) += 2; // to right
                            newState->boat *= -1; // move boat
                            boatMoved = true;
                            cout << "Move 2M, 0C from left to right. Boat move right" << endl;
                        }
                    } else {//Move from right to left island
                        if(newState->state.at(rightIndexM) >= 2){ // check if there is enough M (2M in this case) to move
                            //if there is not enough to move from right to left, boat will be moved to left as empty
                            newState->state.at(rightIndexM) -= 2;
                            newState->state.at(leftIndexM) += 2;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 2M, 0C from right to left. Boat move left" << endl;
                        }
                    }
                    if(!boatMoved){ // IF NOT ABLE TO MOVE ANY ITEM, BOAT WILL BE MOVED TO THE OTHER SIDE TO
                        // TRY ONE LAST TIME. MAX NUM OF TRIES IS 2. THIS CONDITION CAN ONLY BE EXECUTED ONCE
                        // IF GOES HERE, MEANS ALREADY TRIED ONCE, SO TRY THE SECOND ONE THEN MOVE ON
                        if(newState->boat == -1){
                            cout << "Empty Boat moved Right" << endl;
                        } else {
                            cout << "Empty Boat moved Left" << endl;
                        }
                        flag = true; // INDICATES THAT BOAT IS MOVED, IF NO ITEM CAN BE MOVED, GET OUT FROM THIS CASE
                        //AND TRY OTHER CONDITIONS
                        newState->boat *= -1; // MOVE BOAT
                    }
                }
                check1 = newState->state.at(leftIndexM) < newState->state.at(leftIndexC); // CHECK IF NUM OF M LESS THAN C
                check2 = newState->state.at(rightIndexM) < newState->state.at(rightIndexC);
                check3 = check1 && newState->state.at(leftIndexM) == 0; // ISLAND 1, if true, means num M is less than C but M is 0, so it is legal
                check4 = check2 && newState->state.at(rightIndexM) == 0; // ISLAND 2, if true, means num of M less than C but M is 0, so it is legal
                if(check1){ // check if num of M less than C in ISLAND 1
                    if(check1 && check3){ // IF M < C BUT M IS 0 THAN IT IS LEGAL: ISLAND 1
                        newState->legal = true;
                    }else {
                        newState->legal = false;
                    }
                } else if(check2){ // check if num of M < C in ISLAND 2
                    if(check2 && check4){ // IF M < C BUT M IS 0 THAN IT IS LEGAL: ISLAND 2
                        newState->legal = true;
                    } else {
                        newState->legal = false;
                    }
                }
                break;
            case 2: //// PICK 0M, 2C
                while(!boatMoved){
                    if(flag){
                        boatMoved = true;
                    }
                    if(newState->boat == -1){ // Move from left to right
                        if(newState->state.at(leftIndexC) >= 2){
                            newState->state.at(leftIndexC) -= 2;
                            newState->state.at(rightIndexC) += 2;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 2C, 0M from left to right. Boat move right" << endl;
                        }
                    } else { // Move from right to left
                        if(newState->state.at(rightIndexC) >= 2){
                            newState->state.at(rightIndexC) -= 2;
                            newState->state.at(leftIndexC) += 2;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 2C, 0M from right to left. Boat move left" << endl;
                        }
                    }
                    if(!boatMoved){
                        if(newState->boat == -1){
                            cout << "Empty Boat moved Right" << endl;
                        } else {
                            cout << "Empty Boat moved Left" << endl;
                        }
                        flag = true;
                        newState->boat *= -1;
                    }
                }
                check1 = newState->state.at(leftIndexM) < newState->state.at(leftIndexC);
                check2 = newState->state.at(rightIndexM) < newState->state.at(rightIndexC);
                check3 = check1 && newState->state.at(leftIndexM) == 0; // if true, means num M is less than C but M is 0, so its legal
                check4 = check2 && newState->state.at(rightIndexM) == 0; // if true, means num of M less than C but M is 0, so its legal
                if(check1){ // check if num of M less than C in any of the islands
                    if(check1 && check3){
                        newState->legal = true;
                    }else {
                        newState->legal = false;
                    }
                } else if(check2){
                    if(check2 && check4){
                        newState->legal = true;
                    } else {
                        newState->legal = false;
                    }
                }
                break;
            case 3: ////  PICK 1M, 0C
                while(!boatMoved){
                    if(flag){
                        boatMoved = true;
                    }
                    if(newState->boat == -1){ // Move from left to right
                        if(newState->state.at(leftIndexM) >= 1){
                            newState->state.at(leftIndexM) -= 1;
                            newState->state.at(rightIndexM) += 1;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 1M, 0C from left to right. Boat move right" << endl;
                        }
                    } else { // Move from right to left
                        if(newState->state.at(rightIndexM) >= 1){
                            newState->state.at(rightIndexM) -= 1;
                            newState->state.at(leftIndexM) += 1;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 1M, 0C from right to left. Boat move left" << endl;
                        }
                    }
                    if(!boatMoved){
                        if(newState->boat == -1){
                            cout << "Empty Boat moved Right" << endl;
                        } else {
                            cout << "Empty Boat moved Left" << endl;
                        }
                        flag = true;
                        newState->boat *= -1;
                    }
                }
                check1 = newState->state.at(leftIndexM) < newState->state.at(leftIndexC);
                check2 = newState->state.at(rightIndexM) < newState->state.at(rightIndexC);
                check3 = check1 && newState->state.at(leftIndexM) == 0; // if true, means num M is less than C but M is 0, so its legal
                check4 = check2 && newState->state.at(rightIndexM) == 0; // if true, means num of M less than C but M is 0, so its legal
                if(check1){ // check if num of M less than C in any of the islands
                    if(check1 && check3){
                        newState->legal = true;
                    }else {
                        newState->legal = false;
                    }
                } else if(check2){
                    if(check2 && check4){
                        newState->legal = true;
                    } else {
                        newState->legal = false;
                    }
                }
                break;
            case 4: ////  PICK 1C, 0M
                while(!boatMoved){
                    if(flag){
                        boatMoved = true;
                    }
                    if(newState->boat == -1){ // Move from left to right
                        if(newState->state.at(leftIndexC) >= 1){
                            newState->state.at(leftIndexC) -= 1;
                            newState->state.at(rightIndexC) += 1;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 0M, 1C from left to right. Boat move right" << endl;
                        }
                    } else { // Move from right to left
                        if(newState->state.at(rightIndexC) >= 1){
                            newState->state.at(rightIndexC) -= 1;
                            newState->state.at(leftIndexC) += 1;
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 0M, 1C from right to left. Boat move left" << endl;
                        }
                    }
                    if(!boatMoved){
                        if(newState->boat == -1){
                            cout << "Empty Boat moved Right" << endl;
                        } else {
                            cout << "Empty Boat moved Left" << endl;
                        }
                        flag = true;
                        newState->boat *= -1;
                    }
                }
                check1 = newState->state.at(leftIndexM) < newState->state.at(leftIndexC);
                check2 = newState->state.at(rightIndexM) < newState->state.at(rightIndexC);
                check3 = check1 && newState->state.at(leftIndexM) == 0; // if true, means num M is less than C but M is 0, so its legal
                check4 = check2 && newState->state.at(rightIndexM) == 0; // if true, means num of M less than C but M is 0, so its legal
                if(check1){ // check if num of M less than C in any of the islands
                    if(check1 && check3){
                        newState->legal = true;
                    }else {
                        newState->legal = false;
                    }
                } else if(check2){
                    if(check2 && check4){
                        newState->legal = true;
                    } else {
                        newState->legal = false;
                    }
                }
                break;
            case 5: //// PICK 1M, 1C
                while(!boatMoved){
                    if(flag){
                        boatMoved = true;
                    }
                    if(newState->boat == -1){ // Move from left to right
                        if(newState->state.at(leftIndexM) >= 1 && newState->state.at(leftIndexC >= 1)){
                            newState->state.at(leftIndexM) -= 1; // move M
                            newState->state.at(rightIndexM) += 1; // move M
                            newState->state.at(leftIndexC) -= 1; // move C
                            newState->state.at(rightIndexC) += 1; // move C
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 1M, 1C from left to right. Boat move right" << endl;
                        }
                    } else { // Move from right to left
                        if(newState->state.at(rightIndexM) >= 1 && newState->state.at(rightIndexC >= 1)){
                            newState->state.at(rightIndexM) -= 1; // move M
                            newState->state.at(leftIndexM) += 1; // move M
                            newState->state.at(rightIndexC) -= 1; // move C
                            newState->state.at(leftIndexC) += 1; // move C
                            newState->boat *= -1;
                            boatMoved = true;
                            cout << "Move 1M, 1C from right to left. Boat move left" << endl;
                        }
                    }
                    if(!boatMoved){
                        if(newState->boat == -1){
                            cout << "Empty Boat moved Right" << endl;
                        } else {
                            cout << "Empty Boat moved Left" << endl;
                        }
                        flag = true;
                        newState->boat *= -1;
                    }
                }
                check1 = newState->state.at(leftIndexM) < newState->state.at(leftIndexC);
                check2 = newState->state.at(rightIndexM) < newState->state.at(rightIndexC);
                check3 = check1 && newState->state.at(leftIndexM) == 0; // if true, means num M is less than C but M is 0, so its legal
                check4 = check2 && newState->state.at(rightIndexM) == 0; // if true, means num of M less than C but M is 0, so its legal
                if(check1){ // check if num of M less than C in any of the islands
                    if(check1 && check3){
                        newState->legal = true;
                    }else {
                        newState->legal = false;
                    }
                } else if(check2){
                    if(check2 && check4){
                        newState->legal = true;
                    } else {
                        newState->legal = false;
                    }
                }
                break;
                
            default:
                cout << "Invalid Combination Choice" << endl;
                break;
        }
        for(int n: newState->state){ //DISPLAY CURRENT NODE'S STATE, FOR VISUAL HELP, NO OTHER USE!
            cout << n << ", ";
        }
        cout << "" << endl; // NEW LINE
        bool duplicate = false;
        for(Node*item : uniqueHistory) { // check history if this state existed before, HISTORY CONTAINS UNIQUE STATES
            if(item->state == newState->state){
                duplicate = true;
                cout << "Duplicate Found" << endl;
                break;
            }
        }
        if(!duplicate) { // if this state is unique, add it to the history
            uniqueHistory.push_back(newState);
            cout << "Pushed to history" << endl;
        }
        
        if(newState->legal && !duplicate){ // if state is legal and unique, add it to passedNodes QUEUE
            passedNodes.push_back(newState);
            cout << "Pushed to PASSED" << endl;
            legalStates.push_back(newState); // add legal unique state to the records. FOR DISPLAYING PURPOSES
        }
        if(newState->state == goalState){ // check if current state is equal to the goal state
            cout << "---------------------------" << endl;
            cout << "Goal State Has Been Found!!" << endl;
            cout << "---------------------------" << endl;
            return true; // return true and stop searching
        }
        newState = new Node; // create a new node for a new combination
        newState->state = tempNode->state;
        tempNode->nodes.push_back(newState); // add new node as a child to parent
        
        pickCombination++; // Go to the next combination
    }
    return false; // return false if goal is not found.
}


int main() {
    Tree tree;
    tree.iterate(); // start searching and creating
    tree.print(); // print information
    return 0;
}
