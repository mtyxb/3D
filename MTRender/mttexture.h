/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTTEXTURE_H
#define MTTEXTURE_H

#include <QImage>
#include "mtvector.h"

class MTTexture
{
public:
    MTTexture();
    bool load(const char* filename);
    MTVector4 texture(const MTVector3& texture_coord);

private:
    QImage img;
};

#endif // MTTEXTURE_H
