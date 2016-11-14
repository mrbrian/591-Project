#include "camera.h"

Matrix4x4 *Camera::calc_proj_matrix()
{
    Matrix4x4 *result = new Matrix4x4();
    Matrix4x4 &m_projection = *result;

    m_projection[0][0] = 1 / (aspect * tan(fov / 2));
    m_projection[1][1] = 1 / (aspect * tan(fov / 2));
    m_projection[2][2] = (far + near) / (far - near);
    m_projection[2][3] = -2 * (far * near) / (far - near);
    m_projection[3][2] = 1;
    m_projection[3][3] = 0;

    return result;
}

Matrix4x4 *Camera::calc_view_matrix()
{
    Matrix4x4 *result = new Matrix4x4();
    Matrix4x4 &m_projection = *result;

    m_projection[0][0] = 1 / (aspect * tan(fov / 2));
    m_projection[1][1] = 1 / (aspect * tan(fov / 2));
    m_projection[2][2] = (far + near) / (far - near);
    m_projection[2][3] = -2 * (far * near) / (far - near);
    m_projection[3][2] = 1;
    m_projection[3][3] = 0;

    return result;
}
