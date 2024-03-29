#ifndef __CAMERA_H__
#define __CAMERA_H__

/*
Generic camera class by Nghia Ho
*/

class Camera
{
private:
    float m_x, m_y, m_z;   // Position
    float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
    float m_yaw, m_pitch; // Various rotation angles
    float m_strafe_lx, m_strafe_lz; // Always 90 degree to direction vector
public:
    Camera() { Init(); }
    ~Camera() {}

    void Init();
    void Refresh();
    void SetPos(float x, float y, float z);
    void GetPos(float &x, float &y, float &z);
    inline float pega_x() { return m_x; }
    inline float pega_y() { return m_y; }
    inline float pega_z() { return m_z; }
    inline float pega_pitch() { return m_pitch*100; }
    inline float pega_yaw() { return m_yaw*100; }
    void GetDirectionVector(float &x, float &y, float &z);
    void SetYaw(float angle);
    void SetPitch(float angle);

    // Navigation
    void Move(float incr, bool flyMode = true);
    void Strafe(float incr);
    void Fly(float incr);
    void RotateYaw(float angle);
    void RotatePitch(float angle);
};

#endif
