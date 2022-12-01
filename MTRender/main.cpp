/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mtwidget.h"
#include <QApplication>
#include <QDebug>
#include <QMatrix4x4>
#include "mtvector.h"
#include "mtmatrix.h"

bool mt_testcase();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MTWidget w;
    w.showMaximized();

    //qDebug() << "TESTCASE:" << mt_testcase();

    return a.exec();
}



