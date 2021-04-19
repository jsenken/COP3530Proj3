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
        minX = (xNum) * boxSizeInX;
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
//        maxX = num * boxSize;
//        minX = 0;
//        maxY = num * boxSize;
//        minY = 0;
        minX = 0;
        maxX = 1900;
        minY = 0;
        maxY = 936;
        int boxX = maxX/num;
        int boxY = maxY/num;
        //generate each box
        //num -> x,y = x = sqrt(num), y = sqrt(num)
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
                int abX = (a.x - boxList.at(b).node.x);
                int abY = (a.y - boxList.at(b).node.y);
                for(int n = 0; n < edgeList[b].size();n++){
                    if(m != n){
                        Node c = edgeList[b].at(n);
                        int bcX = (boxList.at(b).node.x - c.x);
                        int bcY = (boxList.at(b).node.y - c.y);
                        float dot = (abX * bcX + abY * bcY);
                        float cross = (abX * bcY - abY * bcX);
                        float alpha = atan2(cross, dot);
                        int angle = (int) floor(alpha * 180. / M_PI + 0.5);
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

    int findAngleInNode(map<pair<int,int>,int> &map, int angle){
        for(auto& it: map){
            if(it.second == angle){
                return it.first.first;
            }
        }
        return -1;
    }
//    vector<Node> findCycleofAngles(int angle, int v, bool visited[], int root){
//        visited[v] = true;
//
//    }
//
//    vector<Node> findShapesBrute(int angle){
//        int numSides = 0;
//        vector<Node> ans;
//        switch(angle){
//            case 90:
//                cout << "rectangle" << endl;
//                numSides = 4;
//                break;
//            case 108:
//                cout << "pentagon" << endl;
//                numSides = 5;
//                break;
//            case 120:
//                cout << "hexagon" << endl;
//                numSides = 6;
//                break;
//            case 135:
//                cout << "octagon" << endl;
//                numSides = 8;
//                break;
//        }
//        for(int i = 0; i < boxList.size();i++){ //for every box
//            //check node for angle
//            map<pair<int,int>,int> map = boxList.at(i).node.angles;
//            //search map for angle
//            if(findAngleInNode(map, angle) > 0){
//
//            }
//        }
//
//    }
};

int main() {
    Graph *graph = new Graph(9);
    graph->printBoxList();
    graph->printNodeList();
    graph->printEdgeList();
    graph->printAngles();

    return 0;
}
