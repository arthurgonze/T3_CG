#ifndef T3_CG_SKYBOX_H
#define T3_CG_SKYBOX_H

#include "vertice.h"
class Skybox
{
private:
    Vertice v1, v2, v3, v4, v5, v6, v7, v8;
public:
//    Skybox() = default;
    Skybox(const Vertice &v1, const Vertice &v2, const Vertice &v3, const Vertice &v4, const Vertice &v5,
           const Vertice &v6, const Vertice &v7, const Vertice &v8) : v1(v1), v2(v2), v3(v3), v4(v4), v5(v5),
                                                                      v6(v6), v7(v7), v8(v8) {}
    virtual ~Skybox() = default;

    inline Vertice &pega_v1() { return v1; }
    inline Vertice &pega_v2() { return v2; }
    inline Vertice &pega_v3() { return v3; }
    inline Vertice &pega_v4() { return v4; }
    inline Vertice &pega_v5() { return v5; }
    inline Vertice &pega_v6() { return v6; }
    inline Vertice &pega_v7() { return v7; }
    inline Vertice &pega_v8() { return v8; }

    inline void define_v1(const Vertice &v1) { this->v1 = v1; }
    inline void define_v2(const Vertice &v2) { this->v2 = v2; }
    inline void define_v3(const Vertice &v3) { this->v3 = v3; }
    inline void define_v4(const Vertice &v4) { this->v4 = v4; }
    inline void define_v5(const Vertice &v5) { this->v5 = v5; }
    inline void define_v6(const Vertice &v6) { this->v6 = v6; }
    inline void define_v7(const Vertice &v7) { this->v7 = v7; }
    inline void define_v8(const Vertice &v8) { this->v8 = v8; }
};

#endif //T3_CG_SKYBOX_H
