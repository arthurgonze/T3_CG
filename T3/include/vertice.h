#ifndef VERTICE_H
#define VERTICE_H

class Vertice
{
private:
    double x;
    double y;
    double z;
public:
    /// CONSTRUTOR & DESTRUTOR
    Vertice(double valor_x, double valor_y, double valor_z);
    virtual ~Vertice();

    /// GETTERS
    inline double pega_x() { return x; };
    inline double pega_y() { return y; };
    inline double pega_z() { return z; };

    /// SETTERS
    inline void define_x(double valor_x) { x = valor_x; };
    inline void define_y(double valor_y) { y = valor_y; };
    inline void define_z(double valor_z) { z = valor_z; };
};

#endif // VERTICE_H
