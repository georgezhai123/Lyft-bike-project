/* Your code here! */
#include "dsets.h"
using namespace std;

void DisjointSets::addelements(int num) {
    for (int i = 1; i <= num; i++) {
        elems.push_back(-1); // <-1 ,-1, -1, -1, -1, -1>
    }
    return;
}

int DisjointSets::find(unsigned int elem) {
    if (elems[elem] < 0) {
        return elem;
    } else {
        int found = find(elems[elem]);
        //path compression
        elems[elem] = found;
        return found ;
    }
}

void DisjointSets::setunion(unsigned int a, unsigned int b) {
    // while(elems[a] >= 0) {
    //     a = elems[a];
    // }
    // while(elems[b] >= 0) {
    //     b = elems[b];
    // }
    a = find(a);
    b = find(b);
    if (a == b) {
        return;
    }
    // if a is bigger in size
    if (elems[a] <= elems[b]) {
        elems[a] = elems[a] + elems[b];
        elems[b] = a;
    // if b is bigger in size
    } else {
        elems[b] = elems[a] + elems[b];
        elems[a] = b; // a points to b 
    }
}

int DisjointSets::size(unsigned int elem) {
    //finding the label
    while(elems[elem] >= 0 )  {
        elem = elems[elem];
    }
    return -1*elems[elem];
}