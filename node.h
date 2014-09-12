/* 
 * File:   node.h
 * Author: ozielcarneiro
 *
 * Created on March 7, 2013, 7:34 PM
 */

#include <string>
#include <vector>

#ifndef NODE_H
#define	NODE_H

using namespace std;

class node {
public:
    node();
    node(const node& orig);
    virtual ~node();
    double factor(int *parentsAssign, int value);
    double factor(int value);
    double factor();
    void addParent(node* parent);
    void removeParent(node* parent);
    void addChild(node* child);
    node* getChild(int pos);
    string getName();
    void setName(string name);
    void initTable();
    void setTable(double *table);
    void setTableValue(int pos, double value);
    void setValue(int value);
    void listParents();
    void printTable();
    int getNparents();
    int getNchildren();
    int getValue();
    void setObserved(bool observed);
    bool isObserved();
    int getLevel();
    string toBinary(int value, int digits);
private:
    string name;
    vector<node*> parents;
    int nparents;
    vector<node*> children;
    int nchildren;
    double *table;
    int value;
    bool observed;
    int level;
};

#endif	/* NODE_H */

