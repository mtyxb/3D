/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTRASTER_H
#define MTRASTER_H


#include "mtvector.h"
#include <QImage>
#include <vector>

class MTTexture;

class MTRaster
{
public:
    MTRaster();
    MTRaster(int width, int height, const MTVector4& bg);

    QImage framebuffer();
    void texture(MTTexture* texture);

    void clear(const MTVector4& bg);

    void line(const MTVector3 pts[2], const MTVector3 texcoord[2]);
    void line(const MTVector3& a, const MTVector3& b,
              const MTVector3& ta, const MTVector3& tb);
    void line(const MTVector2& a, const MTVector2& b,
              const MTVector3& ta, const MTVector3& tb);

    void triangle(bool with_texture, const MTVector3 pts[3], const MTVector3 texcoord[3]);

protected:
    void pixel(int x, int y, const MTVector4& color);
    void triangle_simplify(bool with_texture, int mx, int my, int mz,
                           int nx, int ny, int nz,
                           int rx, int ry, int rz,
                           const MTVector3& tm,
                           const MTVector3& tn,
                           const MTVector3& tr);

    void line_of_triangle(bool with_texture, const MTVector3 pts[2],
                    const MTVector3 tcs[2]);

private:
    QImage img;
    std::vector<float> zbuffer;
    MTTexture* tex;
};

#endif // MTRASTER_H
