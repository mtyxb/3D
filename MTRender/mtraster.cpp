/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mtraster.h"
#include "mttexture.h"
#include <QDebug>

MTRaster::MTRaster()
    : tex(nullptr)
{
}

MTRaster::MTRaster(int width, int height, const MTVector4& bg)
 : img(width, height,QImage::Format_ARGB32)
 , zbuffer(width * height, float(std::numeric_limits<float>::max()))
 , tex(nullptr)
{
   clear(bg);
}

QImage MTRaster::framebuffer()
{
    return img;
}

void MTRaster::clear(const MTVector4 &bg)
{
    img.fill(QColor(bg[0] * 255, bg[1] * 255, bg[2] * 255, bg[3] * 255));
    std::fill_n(zbuffer.begin(), zbuffer.size(), float(std::numeric_limits<float>::max()));
}

void MTRaster::texture(MTTexture *texture)
{
    tex = texture;
}

void MTRaster::pixel(int x, int y, const MTVector4& color)
{
    if (x < 0 || y < 0 || x >= img.width() || y >= img.height())
    {
        return;
    }

    QColor c(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);
    if (!c.isValid())
    {
        qDebug() << color.to_string() << c;
        return;
    }
    img.setPixelColor(x, y, c);
}

//@see: https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm
void MTRaster::line(const MTVector3 pts[2], const MTVector3 texcoord[2])
{
    int x0, y0, x1, y1;
    x0 = pts[0][0]; y0 = pts[0][1];
    x1 = pts[1][0]; y1 = pts[1][1];

    MTVector3 ta = texcoord[0];
    MTVector3 tb = texcoord[1];

    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(ta, tb);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;

    MTVector3 tc;
    MTVector4 color;

    for (int x=x0; x<=x1; x++)
    {
        float t = (float)(x - x0) / (x1 - x0);
        tc = mt_clamp_vector(ta, tb, t);
        pixel(steep ? y : x, steep ? x : y, tex->texture(tc));
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

void MTRaster::line(const MTVector3& a, const MTVector3& b,
             const MTVector3& ta, const MTVector3& tb)
{
    MTVector3 pts[] = {a, b};
    MTVector3 texs[] = {ta, tb};
    line(pts, texs);
}

void MTRaster::line(const MTVector2& a, const MTVector2& b,
             const MTVector3& ta, const MTVector3& tb)
{
    MTVector3 pts[] = {MTVector3{a[0], a[1], 0.f}, MTVector3{b[0], b[1], 0.f}};
    MTVector3 texs[] = {ta, tb};
    line(pts, texs);
}

void MTRaster::triangle(bool with_texture, const MTVector3 pts[3], const MTVector3 texcoord[3])
{
    int mx = pts[0][0] + 0.5;
    int my = pts[0][1] + 0.5;
    int mz = pts[0][2] + 0.5;
    int rx = pts[1][0] + 0.5;
    int ry = pts[1][1] + 0.5;
    int rz = pts[1][2] + 0.5;
    int sx = pts[2][0] + 0.5;
    int sy = pts[2][1] + 0.5;
    int sz = pts[2][2] + 0.5;

    MTVector3 tm, tr, ts;

    if (my == ry  && my == sy)
    {
        return;
    }
    if (mx == rx  && mx == sx)
    {
        return;
    }

    if ((ry <= my && ry >= sy) || (ry >= my && ry <= sy))
    {
        std::swap(rx, mx); std::swap(ry, my); std::swap(rz, mz);
        tm = texcoord[1]; tr = texcoord[0]; ts = texcoord[2];
    }
    else if ((sy <= my && sy >= ry) || (sy >= my && sy <= ry))
    {
        std::swap(sx, mx); std::swap(sy, my); std::swap(sz, mz);
        tm = texcoord[2]; tr = texcoord[1]; ts = texcoord[0];
    }
    else
    {
        tm = texcoord[0]; tr = texcoord[1]; ts = texcoord[2];
    }

    int nx, ny, nz;
    if (my == ry)
    {
        triangle_simplify(with_texture, mx, my, mz,  rx, ry, rz, sx, sy, sz, tm, tr, ts);
    }
    else if (my == sy)
    {
        triangle_simplify(with_texture, mx, my, mz, sx, sy, sz, rx, ry, rz, tm, ts, tr);
    }
    else
    {
       double t = (my - ry) / (double)(sy - ry);
       nx = rx + (sx - rx) * t;
       ny = my;
       nz = rz + (sz - rz) * t;

       MTVector3 tn = mt_clamp_vector(tr, ts, t);
       triangle_simplify(with_texture, mx, my, mz, nx, ny, nz, rx, ry, rz, tm, tn, tr);
       triangle_simplify(with_texture, mx, my, mz, nx, ny, nz, sx, sy, sz, tm, tn, ts);
    }
}

void MTRaster::triangle_simplify(bool with_texture, int mx, int my, int mz,
                                 int nx, int ny, int nz,
                                 int rx, int ry, int rz,
                                 const MTVector3 &tm,
                                 const MTVector3 &tn,
                                 const MTVector3 &tr)
{
    MTVector3 M(mx, my, mz);
    MTVector3 N(nx, ny, nz);
    MTVector3 R(rx, ry, rz);

    MTVector3 pc;


    double t;
    float ax, az;  //[M, R]
    float bx, bz;  //[N, R]

    int step = (my < ry ? 1 : -1);
    for (int i = my; (my < ry ? i <= ry : ry <= i); i += step)
    {
        t = (i - my) / (double)(ry - my);
        ax = mx + (rx - mx) * t;
        az = mz + (rz - mz) * t;
        bx = nx + (rx - nx) * t;
        bz = nz + (rz - nz) * t;
        MTVector3 pts[2] = {MTVector3{ax, (float)i, az}, MTVector3{bx, (float)i, bz}};
        MTVector3 tcs[2];
        tcs[0] = mt_clamp_vector(tm, tr, t);
        tcs[1] = mt_clamp_vector(tn, tr, t);
        line_of_triangle(with_texture, pts, tcs);
    }
}

void MTRaster::line_of_triangle(bool with_texture, const MTVector3 pts[2],
                const MTVector3 tcs[2])
{
    int x0, y0, x1, y1, z0, z1;
    x0 = pts[0][0]; y0 = pts[0][1]; z0 = pts[0][2];
    x1 = pts[1][0]; y1 = pts[1][1]; z1 = pts[1][2];

    float tcx0, tcy0, tcx1, tcy1, tcz0, tcz1;
    tcx0 = tcs[0][0]; tcy0 = tcs[0][1]; tcz0 = tcs[0][2];
    tcx1 = tcs[1][0]; tcy1 = tcs[1][1]; tcz1 = tcs[1][2];

    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
        std::swap(tcx0, tcx1);
        std::swap(tcy0, tcy1);
        std::swap(tcz0, tcz1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;

    MTVector3 tc;
    int px, py;
    float z;
    float t;

    for (int x=x0; x<=x1; x++)
    {
        px = steep ? y : x;
        py = steep ? x : y;

        t = (x - x0)/(float)(x1 - x0);
        tc[0] = tcx0 + (tcx1 - tcx0) * t;
        tc[1] = tcy0 + (tcy1 - tcy0) * t;
        tc[2] = tcz0 + (tcz1 - tcz0) * t;
        z = z0 + (z1 - z0) * t;

        if (z < zbuffer[px + py * img.width()])
        {
            pixel(px, py, with_texture ? tex->texture(tc) : MTVector4{tc[0], tc[1], tc[2], 1.f});
            zbuffer[px + py * img.width()] = z;
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}
