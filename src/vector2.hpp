#include <iostream>


class Vector2
{
  public:
    Vector2(double x, double y);

    // Check if vectors are similar
    bool operator==(const Vector2& vector);
    
    bool operator<(const Vector2& vector);   

  private:
    double _x;
    double _y;
};
