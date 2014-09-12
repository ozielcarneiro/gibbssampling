/* 
 * File:   network.cpp
 * Author: ozielcarneiro
 * 
 * Created on March 8, 2013, 4:27 PM
 */

#include <vector>
#include <string>
#include <iostream>
#include <cmath>


#include "network.h"
#include "node.h"

network::network() {
}

network::network(const network& orig) {
}

network::~network() {
}

node* network::searchNode(string name) {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes.at(i).getName().compare(name) == 0)
            return &(nodes.at(i));
    }
    return NULL;
}

void network::addNode(string name) {
    node newNode;
    newNode.setName(name);
    nodes.push_back(newNode);
}

void network::listNodes() {
    for (int i = 0; i < nodes.size(); i++) {
        node x = nodes.at(i);
        cout << i + 1 << ": " << x.getName() << "-" << x.getNparents() << " parents" << endl;
    }
}

void network::addLink(string parent, string child) {
    node* p = searchNode(parent);
    node* c = searchNode(child);

    (*c).addParent(p);
    (*p).addChild(c);
}

double network::pjoint(int* assign) {
    double pjoint = 1;
    for (int i = 0; i < nodes.size(); i++) {
        nodes.at(i).setValue(assign[i]);
        pjoint = pjoint * (nodes.at(i).factor(assign[i]));
    }
    return pjoint;
}

double network::pCnode(int pos, int value) {
    nodes.at(pos).setObserved(0);
    vector< vector<int> > assignlist;
    vector<int> assignArray;
    for (int i = 0; i < nodes.size(); i++) {
        assignArray.push_back(nodes.at(i).getValue());
    }
    assignlist.push_back(assignArray);
    assignArray.clear();
    for (int i = 0; i < nodes.size(); i++) {
        if (!nodes.at(i).isObserved()) {
            int listSize = assignlist.size();
            for (int j = 0; j < listSize; j++) {
                ;
                for (int k = 0; k < nodes.size(); k++) {
                    if (k == i) {
                        assignArray.push_back(!(assignlist.at(j).at(k)));
                    } else {
                        assignArray.push_back(assignlist.at(j).at(k));
                    }
                }
                assignlist.push_back(assignArray);
                assignArray.clear();
            }
        }
    }


    double pEvidence = 0;
    double pnode = 0;

    int aarray[nodes.size()];

    for (int i = 0; i < assignlist.size(); i++) {
        for (int k = 0; k < nodes.size(); k++) {
            aarray[k] = assignlist.at(i).at(k);
            //cout << assignArray[k] <<";";
        }
        //cout << endl;
        double pjointv = pjoint(aarray);
        if (aarray[pos] == value) {
            pnode += pjointv;
            pEvidence += pjointv;
        } else {
            pEvidence += pjointv;
        }
    }

    return pnode / pEvidence;

}

double network::gibbsSampling(int pos, int value, int t) {
    int size = nodes.size();
    int sample[2][size];

    srand((unsigned int) time(NULL));
    //Forward Sampling
    topologicalSort();
    for (int i = 0; i < size; i++) {
        int j = topOrder[i];
        if (nodes.at(j).isObserved()) {
            sample[0][j] = nodes.at(j).getValue();
        } else {
            sample[0][j] = (toss() <= (nodes.at(j).factor(1)));
            nodes.at(j).setValue(sample[0][j]);
        }
    }

    //GIBBS SAMPLING
    double sumMix = 0;
    double sum = 0;
    int mixSize = t*0.02;
    int mixStart = 0;
    for (int i = 1; i <= t; i++) {
        assign(sample[0]);
        for (int j = 0; j < 7; j++) {
            if ((*(nodeAt(j))).isObserved()) {
                sample[1][j] = sample[0][j];
            } else {
                sample[1][j] = (toss() <= (pSamplingNode(j, 1)));
                (*(nodeAt(j))).setValue(sample[1][j]);
            }
        }
        //MIXING PROCESS
        if (i <= mixStart + mixSize) {
            sumMix += pSamplingNode(pos, value);
        } else {
            sum += pSamplingNode(pos, value);
        }
        if (i == mixStart + mixSize * 2) {
            if (abs(sumMix / mixSize - sum / mixSize) < 0.001) {
                sum += sumMix;
            }else{
                sumMix = sum;
                sum = 0;
                mixStart += mixSize;
            }
        }
        //UPDATE OF PREVIOUS SAMPLE
        for (int j = 0; j < 7; j++) {
            sample[0][j] = sample[1][j];
        }
    }
    return sum/(t-mixStart);
}

double network::pSamplingNode(int pos, int value) {

    nodes.at(pos).setValue(value);


    double pEvidence = 0;
    double pnode = 0;


    pnode = nodes.at(pos).factor();
    for (int i = 0; i < nodes.at(pos).getNchildren(); i++) {
        pnode *= (*(nodes.at(pos).getChild(i))).factor();
    }

    nodes.at(pos).setValue(!value);


    pEvidence = nodes.at(pos).factor();
    for (int i = 0; i < nodes.at(pos).getNchildren(); i++) {
        pEvidence *= (*(nodes.at(pos).getChild(i))).factor();
    }
    pEvidence += pnode;

    return pnode / pEvidence;
}

void network::assign(int* assign) {
    for (int i = 0; i < nodes.size(); i++) {

        nodes.at(i).setValue(assign[i]);
    }
}

node* network::nodeAt(int pos) {

    return &(nodes.at(pos));
}

void network::topologicalSort() {
    topOrder = new int[nodes.size()];
    int level = 0;
    int count = 0;
    while (count < nodes.size()) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes.at(i).getLevel() == level) {
                topOrder[count] = i;
                count++;
            }
        }
        level++;
        if (level > nodes.size())

            break;
    }

}

double network::toss() {
    for (int i = 0; i < ((double) rand()) / ((double) RAND_MAX)*1000; i++) {
        rand();
    }

    return ((double) rand()) / ((double) RAND_MAX);
}