/* 
 * File:   main.cpp
 * Author: ozielcarneiro
 *
 * Created on April 6, 2013, 3:45 PM
 */

#include <cstdlib>
#include <iostream>
#include "network.h"
#include "node.h"

using namespace std;

double toss();

/*
 * 
 */
int main(int argc, char** argv) {

    //Bayes Network Initialization
    network net;

    net.addNode("Burglary");
    net.addNode("Earthquake");
    net.addNode("Alarm");
    net.addNode("TV");
    net.addNode("Nap");
    net.addNode("JohnCall");
    net.addNode("MaryCall");

    net.addLink("Burglary", "Alarm");
    net.addLink("Earthquake", "Alarm");
    net.addLink("Alarm", "JohnCall");
    net.addLink("Alarm", "MaryCall");
    net.addLink("TV", "JohnCall");
    net.addLink("Nap", "MaryCall");

    (*(net.searchNode("Burglary"))).initTable();
    (*(net.searchNode("Burglary"))).setTableValue(0, 0.001);//No parent only one entry to table

    (*(net.searchNode("Earthquake"))).initTable();
    (*(net.searchNode("Earthquake"))).setTableValue(0, 0.002);

    (*(net.searchNode("Alarm"))).initTable();
    (*(net.searchNode("Alarm"))).setTableValue(0, 0.001);//Parent assignment Burglary=0 Earthquake=0
    (*(net.searchNode("Alarm"))).setTableValue(1, 0.29);//Parent assignment Burglary=0 Earthquake=1
    (*(net.searchNode("Alarm"))).setTableValue(2, 0.94);//Parent assignment Burglary=1 Earthquake=0
    (*(net.searchNode("Alarm"))).setTableValue(3, 0.95);//Parent assignment Burglary=1 Earthquake=1

    (*(net.searchNode("TV"))).initTable();
    (*(net.searchNode("TV"))).setTableValue(0, 0.7);

    (*(net.searchNode("Nap"))).initTable();
    (*(net.searchNode("Nap"))).setTableValue(0, 0.3);

    (*(net.searchNode("JohnCall"))).initTable();
    (*(net.searchNode("JohnCall"))).setTableValue(0, 0.05);
    (*(net.searchNode("JohnCall"))).setTableValue(1, 0.02);
    (*(net.searchNode("JohnCall"))).setTableValue(2, 0.9);
    (*(net.searchNode("JohnCall"))).setTableValue(3, 0.4);

    (*(net.searchNode("MaryCall"))).initTable();
    (*(net.searchNode("MaryCall"))).setTableValue(0, 0.01);
    (*(net.searchNode("MaryCall"))).setTableValue(1, 0.002);
    (*(net.searchNode("MaryCall"))).setTableValue(2, 0.7);
    (*(net.searchNode("MaryCall"))).setTableValue(3, 0.2);


    int assign[7];
    assign[0] = 1;
    assign[1] = 1;
    assign[2] = 1;
    assign[3] = 1;
    assign[4] = 1;
    assign[5] = 0;
    (*(net.nodeAt(5))).setObserved(1);
    assign[6] = 1;
    (*(net.nodeAt(6))).setObserved(1);
    net.assign(assign);

    //Actual Probability Distribution for Comparison
    //P(B|j0,m1) = 0.0291961
    //    cout << net.pCnode(0, 1) << endl;

    
    
    cout << net.gibbsSampling(0,1,100000) << endl;

    cout << (*(net.searchNode("Alarm"))).getLevel() << endl;

    net.topologicalSort();

    return 0;
}

double toss() {
    for (int i = 0; i < ((double) rand()) / ((double) RAND_MAX)*1000; i++) {
        rand();
    }

    return ((double) rand()) / ((double) RAND_MAX);
}