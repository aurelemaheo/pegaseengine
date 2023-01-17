#ifndef DATASTRUCT_H
#define DATASTRUCT_H

template <typename T>
struct Point
{
  int _x;
  int _y;

  Point(int x, int y)
  {
    _x = x;
    _y = y;
  }

  Point()
  {
    _x = 0;
    _y = 0;
  }
  
};

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

private:
  Node *n;

  Quadtree *topLeft;
  Quadtree *topRight;
  Quadtree *nearLeft;
  Quadtree *nearRight;

public:
    Quadtree()
    {
        n           = NULL;
        topLeft     = NULL;
        topRight    = NULL;
        nearLeft    = NULL;
        nearRight   = NULL;
    }
 
};

#endif //DATASTRUCT_H
