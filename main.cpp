#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include<math.h>
using namespace std;


class Node{
protected:
    static int nextID;
public:
    int id;
    bool selected;
    int x;
    int y;
    map<pair<int,int>,int> angles;
    Node(){

    }

    Node( int xIn, int yIn){
        selected = false;
        x = xIn;
        y = yIn;
        id = nextID++;
    }

    void printAngles(){
        cout << "Node #" << id << " has " << angles.size() << " angles" << endl;
        for (std::map<pair<int,int>,int>::iterator it=angles.begin(); it!=angles.end(); ++it){
            cout << id << "," << it->first.first << "," << it->first.second << " = " << it->second << endl;
        }
    }

};

int Node::nextID =0;

class Box{
public:
    pair<int,int> id; //x,y
    int maxX;
    int minX;
    int maxY;
    int minY;
    int boxSize = 10;
    Node node;
    Box(int xNum, int yNum){
        id = make_pair(xNum,yNum);
        maxX = (xNum + 1) * boxSize;
        minX = (xNum) * boxSize;
        maxY = (yNum + 1) * boxSize;
        minY = yNum * boxSize;
        int x = rand()%(maxX-minX + 1) + minX;
        int y = rand()%(maxY-minY + 1) + minY;

        node = Node(x,y);
    }
    Box(int xNum, int yNum, int boxSizeInX, int boxSizeInY){
        id = make_pair(xNum,yNum);
        maxX = (xNum + 1) * boxSizeInX;
        minX = xNum * boxSizeInX;
        maxY = (yNum + 1) * boxSizeInY;
        minY = yNum * boxSizeInY;
        int x = rand()%(maxX-minX + 1) + minX;
        int y = rand()%(maxY-minY + 1) + minY;
        node = Node(x,y);
    }
};

class Graph{

public:
    int maxX;
    int minX;
    int maxY;
    int minY;
    vector<Box> boxList;
    map<int,vector<Node>> edgeList;

    Graph(int num){
        //generate range of graph
        minX = 0;
        maxX = 1900;
        minY = 0;
        maxY = 936;
        int boxX = maxX/sqrt(num);
        int boxY = maxY/sqrt(num);
        cout <<"box size " << boxX << "," << boxY << endl;
        //generate each box
        int limit = sqrt(num);
        for(int y = 0; y < limit; y++){
            for(int x = 0; x < limit; x++){
                Box box = Box(x,y,boxX,boxY);
                boxList.push_back(box);
            }
        }
        //generate edges between all nodes
        for(int k = 0; k < boxList.size(); k++){
            int from = boxList.at(k).node.id;
            //node id = pair<x,y>
            //check [x-1, y-1], [x-1, y], [x,y-1],[x-1,y+1],[y+1, x-1], [x+1, y+1], [x+1, y], [x,y+1]
            //corners
            //bottom left
            if(k == 0){
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                Node four = boxList.at(k+1+sqrt(num)).node;//x+1, y+1
                Node five = boxList.at(k+1).node; //x+1, y
                edgeList[from].push_back(three);
                edgeList[from].push_back(four);
                edgeList[from].push_back(five);
            }
                //bottom right
            else if(k == sqrt(num)-1){
                Node one = boxList.at(k-1).node; //x-1, y
                Node two = boxList.at(k + sqrt(num)-1).node; //x-1, y+1
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                edgeList[from].push_back(one);
                edgeList[from].push_back(two);
                edgeList[from].push_back(three);

            }
                //top left
            else if(k == num-sqrt(num)){
                Node five = boxList.at(k+1).node; //x+1, y
                Node six = boxList.at(k+1-sqrt(num)).node;//x+1, y-1
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                edgeList[from].push_back(five);
                edgeList[from].push_back(six);
                edgeList[from].push_back(seven);
            }
                //top right
            else if(k == num-1){
                Node one = boxList.at(k-1).node; //x-1, y
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                Node eight = boxList.at(k-1-sqrt(num)).node;//x-1, y-1
                edgeList[from].push_back(one);
                edgeList[from].push_back(seven);
                edgeList[from].push_back(eight);
            }
                //bottom
            else if(k > 0 && k < sqrt(num)-1){
                Node one = boxList.at(k-1).node; //x-1, y
                Node two = boxList.at(k + sqrt(num)-1).node; //x-1, y+1
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                Node four = boxList.at(k+1+sqrt(num)).node;//x+1, y+1
                Node five = boxList.at(k+1).node; //x+1, y
                edgeList[from].push_back(one);
                edgeList[from].push_back(two);
                edgeList[from].push_back(three);
                edgeList[from].push_back(four);
                edgeList[from].push_back(five);
            }
                //top
            else if(k > (num-sqrt(num)) && k < (num-1) ){
                Node one = boxList.at(k-1).node; //x-1, y
                Node five = boxList.at(k+1).node; //x+1, y
                Node six = boxList.at(k+1-sqrt(num)).node;//x+1, y-1
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                Node eight = boxList.at(k-1-sqrt(num)).node;//x-1, y-1
                edgeList[from].push_back(one);
                edgeList[from].push_back(five);
                edgeList[from].push_back(six);
                edgeList[from].push_back(seven);
                edgeList[from].push_back(eight);
            }
                //left
            else if(k > 0 && k % (int)sqrt(num) == 0 && k < num-sqrt(num)){
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                Node four = boxList.at(k+1+sqrt(num)).node;//x+1, y+1
                Node five = boxList.at(k+1).node; //x+1, y
                Node six = boxList.at(k+1-sqrt(num)).node;//x+1, y-1
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                edgeList[from].push_back(three);
                edgeList[from].push_back(four);
                edgeList[from].push_back(five);
                edgeList[from].push_back(six);
                edgeList[from].push_back(seven);
            }
                //right
            else if(k > sqrt(num)-1 && k % (int)sqrt(num) == sqrt(num) - 1 && k < num-1){
                Node one = boxList.at(k-1).node; //x-1, y
                Node two = boxList.at(k + sqrt(num)-1).node; //x-1, y+1
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                Node eight = boxList.at(k-1-sqrt(num)).node;//x-1, y-1
                edgeList[from].push_back(one);
                edgeList[from].push_back(two);
                edgeList[from].push_back(three);
                edgeList[from].push_back(seven);
                edgeList[from].push_back(eight);
            }
                //inner
            else{
                Node one = boxList.at(k-1).node; //x-1, y
                Node two = boxList.at(k + sqrt(num)-1).node; //x-1, y+1
                Node three = boxList.at(k+sqrt(num)).node; //x, y+1
                Node four = boxList.at(k+1+sqrt(num)).node;//x+1, y+1
                Node five = boxList.at(k+1).node; //x+1, y
                Node six = boxList.at(k+1-sqrt(num)).node;//x+1, y-1
                Node seven = boxList.at(k-sqrt(num)).node;//x, y-1
                Node eight = boxList.at(k-1-sqrt(num)).node;//x-1, y-1
                edgeList[from].push_back(one);
                edgeList[from].push_back(two);
                edgeList[from].push_back(three);
                edgeList[from].push_back(four);
                edgeList[from].push_back(five);
                edgeList[from].push_back(six);
                edgeList[from].push_back(seven);
                edgeList[from].push_back(eight);
            }
        }
        //generate angle at Node between 2 adjacent nodes
        for(int l = 0; l < boxList.size(); l++){ //for each node
            int b = boxList.at(l).node.id;
//            cout << "node " << b << " has " << edgeList[b].size() << " edges" << endl;
            for(int m = 0; m < edgeList[b].size();m++){
                Node a = edgeList[b].at(m);
                int abX = (boxList.at(b).node.x -a.x );
                int abY = (boxList.at(b).node.y - a.y);
                for(int n = 0; n < edgeList[b].size();n++){
                    if(m != n){
                        Node c = edgeList[b].at(n);
                        int bcX = (c.x -boxList.at(b).node.x );
                        int bcY = (c.y -boxList.at(b).node.y);
                        float dot = (abX * bcX + abY * bcY);
                        float cross = (abX * bcY - abY * bcX);
                        float alpha = atan2(cross, dot);
                        int angle = 180 - abs((int) floor(alpha * 180. / M_PI + 0.5));
                        if(angle == 59 || angle == 61){
                            angle = 60;
                        }
                        if(angle == 89 || angle == 91){
                            angle = 90;
                        }
                        if(angle == 119 || angle == 121){
                            angle = 120;
                        }
                        if(angle == 134 || angle == 136){
                            angle = 135;
                        }
                        boxList.at(b).node.angles[make_pair(a.id,c.id)] = angle;
                    }
                }
            }
//            cout << "node " << b << " has " << boxList.at(b).node.angles.size() << " angles" << endl;
        }


    }
    void printBoxList(){
        for(int i = 0; i< boxList.size(); i++){
            cout << "(" <<boxList.at(i).id.first << "," <<boxList.at(i).id.second << ") x:[" << boxList.at(i).minX << "," << boxList.at(i).maxX << "], y:[" << boxList.at(i).minY << "," << boxList.at(i).maxY << "]" <<endl;
        }

    }
    void printNodeList(){
        for(int i = 0; i< boxList.size(); i++){
            cout << boxList.at(i).node.id << ") x:" << boxList.at(i).node.x << ", y:" << boxList.at(i).node.y << endl;
        }
    }
    void printEdgeList(){
        for(int i = 0; i <edgeList.size();i++){
            for(int j = 0; j < edgeList[i].size(); j++){
                cout << i << "-> " << edgeList[i].at(j).id << endl;
            }
        }
    }
    void printAngles() {
        for (int i = 0; i < boxList.size(); i++) {
            boxList.at(i).node.printAngles();
        }
    }

    vector<vector<Node>> findShapesBrute(int angle) {
        int numSides = 0;
        vector<vector<Node>> ans;
        switch (angle) {
            case 60:
                cout << "triangle" << endl;
                numSides = 3;
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 1; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + 1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
//                        cout << node0.id << " " << node1.id << " " << node2.id << " " << endl;
                        for (const auto &entry: node0.angles) {
//                            cout << node0.id << ") "<< entry.first.first << " -> " << entry.first.second << "=" << abs(entry.second) << endl;
                            if (entry.first.first == node1.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
//                            cout << node1.id << ") "<< entry.first.first << " -> " << entry.first.second << "=" << abs(entry.second) << endl;
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
//                            cout << node2.id << ") "<< entry.first.first << " -> " << entry.first.second << "=" << abs(entry.second) << endl;
                            if (entry.first.first == node0.id && entry.first.second == node1.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            ans.push_back(group);
                        }
                    }
                }
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 1; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + 1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())+1).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node1.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            ans.push_back(group);
                        }
                    }
                }
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 1; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())+1).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node1.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            ans.push_back(group);
                        }
                    }
                }
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 1; x < sqrt(boxList.size()); x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())-1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node1.id &&
                                abs(entry.second) != 60) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            ans.push_back(group);
                        }
                    }
                }
                break;
            case 90:
                cout << "rectangle" << endl;
                numSides = 4;
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 1; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + 1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node3 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 1).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node2.id &&
                                abs(entry.second) != 90) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node3.id &&
                                abs(entry.second) != 90) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node3.id &&
                                abs(entry.second) != 90) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node3.angles) {
                            if (entry.first.first == node2.id && entry.first.second == node1.id &&
                                abs(entry.second) != 90) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            group.push_back(node3);
                            ans.push_back(group);
                        }
                    }

                }
                break;
            case 108:
                cout << "pentagon" << endl;
                numSides = 5;
                break;
            case 120:
                cout << "hexagon" << endl;
                numSides = 6;
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 2; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + 1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + 2).node;
                        Node node3 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node4 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 1).node;
                        Node node5 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 2).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node3.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node5.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node3.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node4.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node4.angles) {
                            if (entry.first.first == node3.id && entry.first.second == node5.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node5.angles) {
                            if (entry.first.first == node4.id && entry.first.second == node2.id &&
                                abs(entry.second) != 120) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            group.push_back(node3);
                            group.push_back(node4);
                            group.push_back(node5);
                            ans.push_back(group);
                        }
                    }
                }
                for (int yh = 0; yh < sqrt(boxList.size())-2; yh++) { //for every row
                    for (int xh = 0; xh < sqrt(boxList.size()) - 1; xh++) { //for every col
                        bool shapeh = true;
                        Node node0h = boxList.at(xh + (yh * sqrt(boxList.size()))).node;
                        Node node1h = boxList.at(xh + (yh * sqrt(boxList.size())) + 1).node;
                        Node node2h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node3h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) + 1).node;
                        Node node4h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) +sqrt(boxList.size())).node;
                        Node node5h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) + sqrt(boxList.size()) +1).node;
                        for (const auto &entry: node0h.angles) {
                            if (entry.first.first == node1h.id && entry.first.second == node2h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node1h.angles) {
                            if (entry.first.first == node0h.id && entry.first.second == node3h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node2h.angles) {
                            if (entry.first.first == node0h.id && entry.first.second == node4h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node4h.angles) {
                            if (entry.first.first == node2h.id && entry.first.second == node5h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node5h.angles) {
                            if (entry.first.first == node4h.id && entry.first.second == node3h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node3h.angles) {
                            if (entry.first.first == node5h.id && entry.first.second == node1h.id &&
                                abs(entry.second) != 120) {
                                shapeh = false;
                            }
                        }
                        if (shapeh) {
                            vector<Node> group;
                            group.push_back(node0h);
                            group.push_back(node1h);
                            group.push_back(node2h);
                            group.push_back(node3h);
                            group.push_back(node4h);
                            group.push_back(node5h);
                            ans.push_back(group);
                        }
                    }
                }
                break;
            case 135:
                cout << "octagon" << endl;
                numSides = 8;
                for (int y = 0; y < sqrt(boxList.size()) - 1; y++) { //for every row
                    for (int x = 0; x < sqrt(boxList.size()) - 3; x++) { //for every col
                        bool shape = true;
                        Node node0 = boxList.at(x + (y * sqrt(boxList.size()))).node;
                        Node node1 = boxList.at(x + (y * sqrt(boxList.size())) + 1).node;
                        Node node2 = boxList.at(x + (y * sqrt(boxList.size())) + 2).node;
                        Node node3 = boxList.at(x + (y * sqrt(boxList.size())) + 3).node;
                        Node node4 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node5 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 1).node;
                        Node node6 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 2).node;
                        Node node7 = boxList.at(x + (y * sqrt(boxList.size())) + sqrt(boxList.size()) + 3).node;
                        for (const auto &entry: node0.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node4.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node1.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node2.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node2.angles) {
                            if (entry.first.first == node1.id && entry.first.second == node3.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node3.angles) {
                            if (entry.first.first == node2.id && entry.first.second == node7.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node4.angles) {
                            if (entry.first.first == node0.id && entry.first.second == node5.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node5.angles) {
                            if (entry.first.first == node4.id && entry.first.second == node6.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node6.angles) {
                            if (entry.first.first == node5.id && entry.first.second == node7.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        for (const auto &entry: node7.angles) {
                            if (entry.first.first == node6.id && entry.first.second == node3.id &&
                                abs(entry.second) != 135) {
                                shape = false;
                            }
                        }
                        if (shape) {
                            vector<Node> group;
                            group.push_back(node0);
                            group.push_back(node1);
                            group.push_back(node2);
                            group.push_back(node3);
                            group.push_back(node4);
                            group.push_back(node5);
                            group.push_back(node6);
                            group.push_back(node7);
                            ans.push_back(group);
                        }
                    }
                }
                for (int yh = 0; yh < sqrt(boxList.size())-3; yh++) { //for every row
                    for (int xh = 0; xh < sqrt(boxList.size()) - 1; xh++) { //for every col
                        bool shapeh = true;
                        Node node0h = boxList.at(xh + (yh * sqrt(boxList.size()))).node;
                        Node node1h = boxList.at(xh + (yh * sqrt(boxList.size())) + 1).node;
                        Node node2h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size())).node;
                        Node node3h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) + 1).node;
                        Node node4h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) +sqrt(boxList.size())).node;
                        Node node5h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) + sqrt(boxList.size()) +1).node;
                        Node node6h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) +sqrt(boxList.size())+ sqrt(boxList.size())).node;
                        Node node7h = boxList.at(xh + (yh * sqrt(boxList.size())) + sqrt(boxList.size()) + sqrt(boxList.size())+ sqrt(boxList.size()) +1).node;
                        for (const auto &entry: node0h.angles) {
                            if (entry.first.first == node1h.id && entry.first.second == node2h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node1h.angles) {
                            if (entry.first.first == node0h.id && entry.first.second == node3h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node2h.angles) {
                            if (entry.first.first == node0h.id && entry.first.second == node4h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node4h.angles) {
                            if (entry.first.first == node2h.id && entry.first.second == node6h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node6h.angles) {
                            if (entry.first.first == node4h.id && entry.first.second == node7h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node7h.angles) {
                            if (entry.first.first == node5h.id && entry.first.second == node6h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node5h.angles) {
                            if (entry.first.first == node7h.id && entry.first.second == node3h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        for (const auto &entry: node3h.angles) {
                            if (entry.first.first == node5h.id && entry.first.second == node1h.id &&
                                abs(entry.second) != 135) {
                                shapeh = false;
                            }
                        }
                        if (shapeh) {
                            vector<Node> group;
                            group.push_back(node0h);
                            group.push_back(node1h);
                            group.push_back(node2h);
                            group.push_back(node3h);
                            group.push_back(node4h);
                            group.push_back(node5h);
                            group.push_back(node6h);
                            group.push_back(node7h);
                            ans.push_back(group);
                        }
                    }
                }
                break;
        }
        return ans;
    }
};

int main() {
    Graph *graph = new Graph(10000);
    vector<vector<Node>> ans =graph->findShapesBrute(60);
    if(ans.size() > 0)
        cout << ans.at(0).at(0).id << ans.at(0).at(1).id << ans.at(0).at(2).id;
//    graph->printBoxList();
//    graph->printNodeList();
//    graph->printEdgeList();
//    graph->printAngles();

    return 0;
}

