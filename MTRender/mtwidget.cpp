/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mtwidget.h"
#include "mtraster.h"
#include <QPainter>
#include <QDebug>
#include <windows.h>


MTWidget::MTWidget(QWidget *parent)
    : QWidget(parent)
    , angle(0.f)
{
    texture.load(R"(D:\ogl\MTRender\cube.png)");
}

MTWidget::~MTWidget()
{
}

static const MTShaderInput C_CUBE_VERTEXS_WITH_TEXTURE[] = {
       {{ -0.25f, -0.25f,  0.25f}, {0.0f, 1.0f }},
       {{ -0.25f, -0.25f, -0.25f}, {0.0f, 0.0f }},
       {{ 0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f }},

       {{ 0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f  }},
       {{ 0.25f, -0.25f,  0.25f}, { 1.0f, 1.0f  }},
       {{ -0.25f, -0.25f,  0.25f}, {0.0f, 1.0f }},

       {{ 0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f }},
       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f }},
       {{ 0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f }},

       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f }},
       {{ 0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f }},
       {{ 0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f }},

       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f }},
       {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 0.0f }},
       {{ 0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f }},

      {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 0.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 0.0f, 1.0f }},
      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f }},


      {{ -0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f }},
      {{ -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f }},

      {{ -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f }},
      {{ -0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f }},

      {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 1.0f }},
      {{  0.25f,  0.25f, -0.25f}, { 1.0f, 1.0f }},
      {{  0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f }},

      {{  0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f  }},
      {{  -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f }},
      {{  -0.25f,  0.25f, -0.25f}, { 0.0f, 1.0f }},

      {{  -0.25f, -0.25f,  0.25f}, { 0.0f, 0.0f }},
      {{  0.25f, -0.25f,  0.25f}, { 1.0f, 0.0f  }},
      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f  }},

      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f  }},
      {{  -0.25f,  0.25f,  0.25f}, { 0.0f, 1.0f }},
      {{ -0.25f, -0.25f,  0.25f}, { 0.0f, 0.0f  }}
};


static const MTShaderInput C_CUBE_VERTEXS_WITH_COLOR[] = {
       {{ -0.25f, -0.25f,  0.25f}, {0.0f, 1.0f, 1.0f }},
       {{ -0.25f, -0.25f, -0.25f}, {0.0f, 0.0f, 1.0f }},
       {{ 0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},

       {{ 0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f  }},
       {{ 0.25f, -0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f  }},
       {{ -0.25f, -0.25f,  0.25f}, {0.0f, 1.0f, 1.0f }},

       {{ 0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},
       {{ 0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f, 1.0f }},

       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},
       {{ 0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f }},
       {{ 0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f, 1.0f }},

       {{ 0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},
       {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
       {{ 0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f }},

      {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 0.0f, 1.0f, 1.0f }},
      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f }},

      {{ -0.25f,  0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},
      {{ -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f }},

      {{ -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
      {{ -0.25f, -0.25f,  0.25f}, { 0.0f, 1.0f, 1.0f }},
      {{ -0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f, 1.0f }},

      {{ -0.25f,  0.25f, -0.25f}, { 0.0f, 1.0f, 1.0f }},
      {{  0.25f,  0.25f, -0.25f}, { 1.0f, 1.0f, 1.0f }},
      {{  0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f, 1.0f }},

      {{  0.25f, -0.25f, -0.25f}, { 1.0f, 0.0f , 1.0f }},
      {{  -0.25f, -0.25f, -0.25f}, { 0.0f, 0.0f, 1.0f }},
      {{  -0.25f,  0.25f, -0.25f}, { 0.0f, 1.0f, 1.0f }},

      {{  -0.25f, -0.25f,  0.25f}, { 0.0f, 0.0f, 1.0f }},
      {{  0.25f, -0.25f,  0.25f}, { 1.0f, 0.0f , 1.0f }},
      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f , 1.0f }},

      {{  0.25f,  0.25f,  0.25f}, { 1.0f, 1.0f , 1.0f }},
      {{  -0.25f,  0.25f,  0.25f}, { 0.0f, 1.0f, 1.0f }},
      {{ -0.25f, -0.25f,  0.25f}, { 0.0f, 0.0f , 1.0f }}
};

static double stat_fps()
{
    static unsigned long long fps_counter = 0;
    static unsigned long long fps_start = 0;
    static double fps = 0.;

    if (fps_counter % 100 == 0)
    {
        fps_start = GetTickCount();
        fps_counter = 0;
    }
    if (fps_counter > 0 && fps_start > 0)
    {
        double seconds = (1.0 * GetTickCount() - fps_start)/ 1000.;
        fps = fps_counter / seconds;
    }
    fps_counter++;
    return fps;
}

void MTWidget::paintEvent(QPaintEvent *)
{
    int w = width();
    int h = height();

    ps.project_matrix.identity();
    ps.project_matrix.perspective(90.f, w / (float)h, 0.01f, 1000.f);

    ps.view_matrix.identity();
    ps.view_matrix.lookat(MTVector3{0.f, 0.f, 30.f},MTVector3{0.f, 0.f, 0.f},MTVector3{0.f, 1.f, 0.f});

    ps.model_matrix.identity();
    ps.model_matrix.rotate(angle, 1.f, 1.f, 1.f);
    ps.model_matrix.scale(30.f, 30.f, 30.f);

    ps.viewport_matrix.identity();
    ps.viewport_matrix.viewport(0.f, 0.f, w, h, -1.f, 1.f);


    MTRaster raster(w, h, MTVector4{1.f, 1.f, 1.f, 1.f});
    ps.raster = &raster;

//    {
//    ps.with_texture = false;
//    ps.texture = nullptr;
//    raster.texture(nullptr);
//    auto & input = C_CUBE_VERTEXS_WITH_COLOR;
//    int count = sizeof(input)/sizeof(input[0]);
//    pipeline.triangle(ps, input, count);
//    }

    {
    ps.with_texture = true;
    ps.texture = &texture;
    raster.texture(ps.texture);
    auto & input = C_CUBE_VERTEXS_WITH_TEXTURE;
    int count = sizeof(input)/sizeof(input[0]);
    pipeline.triangle(ps, input, count);
    }


    QPainter p(this);
    p.drawImage(rect(), raster.framebuffer());

    double fps = stat_fps();
    QString str = QString("FPS=%1 angle=%2").arg((unsigned int)fps).arg(angle);
    p.drawText(100, 100, str);

    angle += 1.f;
    if (angle > 360.f)
    {
        angle -= 360.f;
    }
    update();
 }
