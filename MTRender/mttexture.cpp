/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mttexture.h"

MTTexture::MTTexture()
{

}

bool MTTexture::load(const char *filename)
{
    QImage temp;
    bool ret = temp.load(filename);
    img = temp;
    return ret;
}

MTVector4 MTTexture::texture(const MTVector3& texture_coord)
{
    int x = texture_coord[0] * (img.width() - 1) + 0.5f;
    int y = texture_coord[1] * (img.height() - 1) + 0.5f;
    if (x < 0 || y < 0 || x >= img.width() || y >= img.height())
    {
        return MTVector4{0.f, 0.f, 0.f, 1.f};
    }
    QColor color = img.pixelColor(x, y);
    return MTVector4{(float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF()};
}
