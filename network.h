/* 
 * File:   network.h
 * Author: ozielcarneiro
 *
 * Created on March 8, 2013, 4:27 PM
 */

#include <string>
#include <vector>
#include "node.h"

#ifndef NETWORK_H
#define	NETWORK_H

using namespace std;

class network {
public:
    network();
    network(const network& orig);
    virtual ~network();
    void addNode(string name);
    void removeNode(string name);
    void addLink(string parent, string child);
    void removeLink(string parent, string child);
    node* searchNode(string name);
    void listNodes();
    double pjoint(int *assign);
    void assign(int *assign);
    node* nodeAt(int pos);
    double pCnode(int pos,int value);
    double pSamplingNode(int pos,int value);
    double gibbsSampling(int pos, int value, int t);
    void topologicalSort();
private:
    double toss();
    vector<node> nodes;
    int* topOrder;
};

#endif	/* NETWORK_H */

