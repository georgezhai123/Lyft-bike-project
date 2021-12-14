#pragma once
#include <vector>
using namespace std;



class DisjointSets{
public:
  void addelements(int num);
  int find(unsigned int elem);
  void setunion(unsigned int a, unsigned int b);
  int size(unsigned int elem);

  vector<int> elems;
};