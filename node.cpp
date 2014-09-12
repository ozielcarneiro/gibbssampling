/* 
 * File:   node.cpp
 * Author: ozielcarneiro
 * 
 * Created on March 7, 2013, 7:34 PM
 */

#include "node.h"
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;

node::node() {
    nparents = 0;
    value = 0;
    observed = 0;
    nchildren = 0;
    level = 0;
}

node::node(const node& orig) {
    this->name = orig.name;
    this->parents = orig.parents;
    this->table = orig.table;
    this->observed = orig.observed;
    this->value = orig.value;
    this->nparents = orig.nparents;
    this->children = orig.children;
    this->nchildren = orig.nchildren;
    this->level = orig.level;
}

node::~node() {
}

double node::factor(int* parentsAssign, int value) {
    int pos = 0;
    for (int i = 0; i < parents.size(); i++) {
        pos += pow(parentsAssign[i]*(parents.size()-i), 2);
    }
    if (value == 1) {
        return table[pos];
    } else {
        return 1 - table[pos];
    }
}

double node::factor(int value){
    int pos = 0;
    for (int i = 0; i < parents.size(); i++) {
        int pvalue = (*(parents.at(i))).getValue();
        int ploc = parents.size()-(i+1);
        pos += pvalue*pow(2,ploc);
    }
    if (value == 1) {
        return table[pos];
    } else {
        return 1 - table[pos];
    }
}

double node::factor(){
    int pos = 0;
    for (int i = 0; i < parents.size(); i++) {
        int pvalue = (*(parents.at(i))).getValue();
        int ploc = parents.size()-(i+1);
        pos += pvalue*pow(2,ploc);
    }
    if (value == 1) {
        return table[pos];
    } else {
        return 1 - table[pos];
    }
}

string node::getName() {
    return name;
}

void node::setName(string name) {
    this->name = name;
}

void node::initTable() {
    int size = pow(parents.size(), 2);
    this->table = new double[size];
}

void node::setTable(double* table) {
    int size = pow(parents.size(), 2);
    this->table = new double[size];
    for (int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }
}

void node::setTableValue(int pos, double value) {
    table[pos] = value;
}

void node::addParent(node* parent) {
    parents.push_back(parent);
    nparents++;
}

void node::addChild(node* child){
    children.push_back(child);
    nchildren++;
}

node* node::getChild(int pos){
    return children.at(pos);
}

void node::listParents() {
    for (int i = 0; i < parents.size(); i++) {
        cout << (*(parents.at(i))).getName() << "; ";
    }
    cout << endl;
}

int node::getNparents() {
    return nparents;
}

int node::getNchildren(){
    return nchildren;
}

void node::printTable() {
    int size = pow(parents.size(), 2);
    if (size == 0) {
        cout << "T: " << table[0] << endl;
    } else {
        for (int i = 0; i < size; i++) {
            cout << toBinary(i, parents.size()) << ": " << table[i] << endl;
        }
    }
}

void node::setValue(int value){
    this->value = value;
}

int node::getValue(){
    return value;
}

void node::setObserved(bool observed){
    this->observed = observed;
}

bool node::isObserved(){
    return observed;
}

int node::getLevel(){
    if(nparents==0){
        level = 0;
        return level;
    }else{
        int maxPlevel = 0;
        for (int i = 0; i < parents.size(); i++) {
            int plevel = (*(parents.at(i))).getLevel();
            if(plevel>maxPlevel){
                maxPlevel = plevel;
            }
        }
        level = maxPlevel + 1;
        return level;
    }
}


string node::toBinary(int value, int digits) {
    ;
    stringstream out("");
    int x = value;
    for (int i = digits - 1; i >= 0; i--) {
        x = x - pow(2, i);
        if (x < 0) {
            out << "0";
            x = x + pow(2, i);
        } else {
            out << "1";
        }
    }

    return out.str();

}