/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mtmatrix.h"
#include <QDebug>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <math.h>



static bool mt_is_equal(const QMatrix4x4& a, const MTMatrix4x4& b)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!mt_is_equal(a.data()[j * 4 + i], b[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

static bool mt_is_equal(const QVector3D& a, const MTVector3& b)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        if (!mt_is_equal(a[i], b[i]))
        {
            qDebug() << a[i] << " => " << b[i];
            return false;
        }
    }
    return true;
}


static bool mt_is_equal(const QVector4D& a, const MTVector4& b)
{
    for (unsigned int i = 0; i < 4; i++)
    {
        if (!mt_is_equal(a[i], b[i]))
        {
            qDebug() << a[i] << " => " << b[i];
            return false;
        }
    }
    return true;
}



static int COUNT = 0;

template <typename T>
static void mt_assert_equal(bool& result, T a, T b, const char* str)
{
    if (!mt_is_equal(a, b))
    {
        COUNT++;
        qDebug() << QString("FAILED:{%1} : {%2}: {%3} is not equal {%4}")
                    .arg(COUNT).arg(a).arg(b).arg(str);
        if (1 == COUNT)
        {
           result = false;
        }
    }
    else
    {
        qDebug() << QString("%1 OK").arg(str);
    }
}


template <typename T, typename S>
static void mt_assert_equal(bool& result, const T& a, const S& b, const char* str)
{
    if (!mt_is_equal(a, b))
    {
        COUNT++;
        qDebug() << QString("FAILED:{%1} : {%2}: is not equal")
                    .arg(COUNT).arg(str);
        if (1 == COUNT)
        {
           result = false;
        }
    }
    else
    {
        qDebug() << QString("%1 OK").arg(str);
    }
}

static void qt_vector_divide_by_w(QVector4D& t)
{
    if (1.0f != t[3] && 0.0f != t[3])
    {
        for (int i = 0; i < 3; i++)
        {
            t[i] /= t[3];
        }
        t[3] = 1.f;
    }
}

static void mt_test_projection(bool& result, float x, float y, float z)
{
    {
        struct QTPipelineState
        {
            QMatrix4x4  viewport_matrix;
            QMatrix4x4  project_matrix;
            QMatrix4x4  view_matrix;
            QMatrix4x4  model_matrix;
        };


        struct MTPipelineState
        {
            MTMatrix4x4  viewport_matrix;
            MTMatrix4x4  project_matrix;
            MTMatrix4x4  view_matrix;
            MTMatrix4x4  model_matrix;
        };

        int w = 1920;
        int h = 1027;

        qDebug() << QString("pos:{%1,%2,%3}").arg(x).arg(y).arg(z);

        QVector4D posa2;
        {
            QTPipelineState ps;
            ps.project_matrix.perspective(90.f, w / (float)h, 0.01f, 1000.f);
            ps.view_matrix.lookAt(QVector3D{0.f, 0.f, 100.f},QVector3D{0.f, 0.f, 0.f},QVector3D{0.f, 1.f, 0.f});
            ps.model_matrix.scale(10.f, 10.f, 10.f);
            ps.viewport_matrix.viewport(0.f, 0.f, w, h, -1.f, 1.f);
            QVector4D pos = QVector4D{x, y, z, 1.f};
            posa2 =   ps.viewport_matrix *  ps.project_matrix * ps.view_matrix * ps.model_matrix * pos;
            qt_vector_divide_by_w(posa2);
        }

        MTVector4 posb2;
        {
        MTPipelineState ps;
        ps.project_matrix.identity();
        ps.project_matrix.perspective(90.f, w / (float)h, 0.01f, 1000.f);

        ps.view_matrix.identity();
        ps.view_matrix.lookat(MTVector3{0.f, 0.f, 100.f},MTVector3{0.f, 0.f, 0.f},MTVector3{0.f, 1.f, 0.f});

        ps.model_matrix.identity();
        ps.model_matrix.scale(10.f, 10.f, 10.f);

        ps.viewport_matrix.identity();
        ps.viewport_matrix.viewport(0.f, 0.f, w, h, -1.f, 1.f);

        MTVector4 pos = MTVector4{x, y, z, 1.f};
        posb2 = ps.viewport_matrix * ps.project_matrix * ps.view_matrix * ps.model_matrix * pos;
        posb2.divide_by_w();
        posb2.simplify();
        }

        qDebug() << "posa2:" << posa2;
        qDebug() << "posb2:" << posb2.to_string();
        mt_assert_equal(result, posa2, posb2, "viewport&project&view&model");

    }
}

bool mt_testcase()
{
    bool result = true;


    mt_test_projection(result, 1.f, 0.f, 0.f);
    mt_test_projection(result, -1.f, 0.f, 0.f);

    mt_test_projection(result, -1.f, 0.f, -10.f);
    mt_test_projection(result, -1.f, 0.f, -910.f);
    mt_test_projection(result, -1.f, 0.f, -200000.f);
    mt_test_projection(result, -1.f, 0.f, 1000005.f);


    {
        QMatrix4x4 a;
        a.viewport(0.f, 0.f, 100.f, 100.f, 0.f, 1.f);

        MTMatrix4x4 b;
        b.viewport(0.f, 0.f, 100.f, 100.f, 0.f, 1.f);

        mt_assert_equal(result, a, b, "viewport");
    }


    {

        QMatrix4x4 a;
       // a.scale(100.f, 100.f, 100.f);
        a.translate(100.f, 100.f, 100.f);

        QVector3D va(1.f, 2.f, 3.f);
        QVector3D va2 = a * va;

        MTMatrix4x4 b;
       // b.scale(100.f, 100.f, 100.f);
        b.translate(100.f, 100.f, 100.f);

        MTVector3 vb{1.f, 2.f, 3.f};
        MTVector3 vb2 = b * vb;

        qDebug() << va << " => " << va2;
        qDebug() << vb.to_string() << " => " << vb2.to_string();

        mt_assert_equal(result, a, b, "matrix test");
        mt_assert_equal(result, va2, vb2, "matrix X vector");
    }



    //1 2
    //3 4
    //det = 1 * 4 - 2 * 3 = -2;
    MTMatrix2x2 m;
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    qDebug() << m.to_string();
    float det = m.det();
    qDebug() << det;
    qDebug() << m.transpose().to_string();
    //1 3
    //2 4



    {
    //1 2 3
    //2 5 3
    //3 2 1
    //det=1 * (5 * 1 - 3 * 2) + -2 * (2 * 1 - 3 * 2) + 3 * (2 * 3 - 3 * 5) = -20
    MTMatrix3x3 m;
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;

    m[1][0] = 2;
    m[1][1] = 5;
    m[1][2] = 3;

    m[2][0] = 3;
    m[2][1] = 2;
    m[2][2] = 1;
    qDebug() << m.to_string();
    float det = m.det();
    qDebug() << det;
    qDebug() << m.transpose().to_string();

    mt_assert_equal(result, det, -20.0f, "det");
    }

    {
    //1 2 3 4
    //2 3 3 1
    //3 4 1 3
    //4 5 5 4
    //det=1 * (3 * (1 * 4 - 3 * 5) - 4 * (3 * 4 - 1 * 5) + 5 * (3 * 3 - 1 * 1))-2 * (2 * (1 * 4 - 3 * 5) - 4 * (3 * 4 - 4 * 5) + 5 *(3 * 3 - 4 * 1)) + 3 * (2 * (3 * 4 - 1 * 5) - 3 * (3 * 4 - 4 * 5) + 5 * (3 * 1 - 4 * 3)) -4 * (2 *(3 * 3 - 1 * 1) - 3 *(3 * 3 - 4 * 1) + 4 *(3 * 1 - 4 * 3))=28
    MTMatrix4x4 m;
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[0][3] = 4;

    m[1][0] = 2;
    m[1][1] = 3;
    m[1][2] = 3;
    m[1][3] = 1;

    m[2][0] = 3;
    m[2][1] = 4;
    m[2][2] = 1;
    m[2][3] = 3;

    m[3][0] = 4;
    m[3][1] = 5;
    m[3][2] = 5;
    m[3][3] = 4;

    qDebug() << m.to_string();
    float det = m.det();
    qDebug() << det;
    qDebug() << m.transpose().to_string();
    mt_assert_equal(result, det, 28.0f, "det");
    }

    {
    QMatrix4x4 a;
    a.frustum(-100, +100, -100, +100, 1, 1000);
    qDebug() << a;

    MTMatrix4x4 b;
    b.frustum(-100, +100, -100, +100, 1, 1000);
    qDebug() << b.to_string();

    mt_assert_equal(result, a, b, "frustum");

    }

    {
    QMatrix4x4 a;
    a.perspective(90, 1.0, 1, 1000);
    qDebug() << a;

    MTMatrix4x4 b;
    b.perspective(90, 1.0, 1, 1000);
    qDebug() << b.to_string();

    mt_assert_equal(result, a, b, "perspective");

    }


    {
    QMatrix4x4 a;
    a.lookAt(QVector3D(100, 100, 100), QVector3D(30, 40, 50), QVector3D(0, 1, 0));
    qDebug() << a;

    MTMatrix4x4 b;
    b.lookat(MTVector3(100, 100, 100), MTVector3(30, 40, 50), MTVector3(0, 1, 0));
    qDebug() << b.to_string();

    mt_assert_equal(result, a, b, "lookat");

    }


    {
    QMatrix4x4 a;
    a.translate(100, 200, 300);
    a.scale(10, 20, 30);
    a.rotate(30, 1, 2, 3);
    a.translate(QVector3D(300, 200, 100));
    a.scale(QVector3D(20, 10, 30));
    a.rotate(20, QVector3D(3, 1, 2));
    qDebug() << a;

    MTMatrix4x4 b;
    b.translate(100, 200, 300);
    b.scale(10, 20, 30);
    b.rotate(30, 1, 2, 3);
    b.translate(MTVector3(300, 200, 100));
    b.scale(MTVector3(20, 10, 30));
    b.rotate(20, MTVector3(3, 1, 2));
    qDebug() << b.to_string();

    mt_assert_equal(result, a, b, "translate & scale & rotate");

    }

    return result;
}




////////////////////////////////////////////////////////////////////////


#if 0

"{{1,2},{3,4}}"
-2
"{{1,3},{2,4}}"
"{{1,2,3},{2,5,3},{3,2,1}}"
-20
"{{1,2,3},{2,5,2},{3,3,1}}"
"det OK"
"{{1,2,3,4},{2,3,3,1},{3,4,1,3},{4,5,5,4}}"
28
"{{1,2,3,4},{2,3,4,5},{3,3,1,5},{4,1,3,4}}"
"det OK"
QMatrix4x4(type:General
      0.01         0         0         0
         0      0.01         0         0
         0         0    -1.002    -2.002
         0         0        -1         0
)
"{{0.01,0,0,0},{0,0.01,0,0},{0,0,-1.002,-2.002},{0,0,-1,0}}"
"frustum OK"
QMatrix4x4(type:General
         1         0         0         0
         0         1         0         0
         0         0    -1.002    -2.002
         0         0        -1         0
)
"{{1,0,0,0},{0,1,0,0},{0,0,-1.002,-2.002},{0,0,-1,0}}"
"perspective OK"
QMatrix4x4(type:Translation,Rotation
  0.581238         0 -0.813733   23.2495
 -0.465519  0.820199 -0.332513  -2.21675
  0.667424  0.572078  0.476731  -171.623
         0         0         0         1
)
"{{0.581238,0,-0.813734,23.2495},{-0.465519,0.8202,-0.332513,-2.21675},{0.667424,0.572078,0.476731,-171.623},{0,0,0,1}}"
"lookat OK"
QMatrix4x4(type:Translation,Scale,Rotation
   158.053  -40.6754   115.639   2259.25
   202.794   129.255  -72.0963   6184.98
  -185.013   320.799   788.017   2155.77
         0         0         0         1
)
"{{158.053,-40.6754,115.639,2259.25},{202.794,129.255,-72.0963,6184.98},{-185.013,320.799,788.017,2155.77},{0,0,0,1}}"
"translate & scale & rotate OK"
TESTCASE: true

#endif

