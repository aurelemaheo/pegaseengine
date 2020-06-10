#ifndef DATASTRUCT_H
#define DATASTRUCT_H

struct Node
{
  int data;
  
  Node(int _data)
  {
    data = _data;
  }

};

class Quadtree
{

  Node *n;

  Quadtree *topLeft;
  Quadtree *topRight;
  Quadtree *nearLeft;
  Quadtree *nearRight;

 
};

#endif //DATASTRUCT_H
