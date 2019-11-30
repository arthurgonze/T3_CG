#ifndef VERTICE_H
#define VERTICE_H

class Vertice
{
private:
    double x;
    double y;
    double z;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Vertice(double valorX, double valorY, double valorZ);
    virtual ~Vertice();

    /// GETTERS
    inline double getX() { return x; };
    inline double getY() { return y; };
    inline double getZ() { return z; };

    /// SETTERS
    inline void setX(double valorX) { x = valorX; };
    inline void setY(double valorY) { y = valorY; };
    inline void setZ(double valorZ) { z = valorZ; };
};

#endif // VERTICE_H
