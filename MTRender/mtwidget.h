/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTWIDGET_H
#define MTWIDGET_H

#include <QWidget>
#include "mtpipeline.h"
#include "mttexture.h"

class MTWidget : public QWidget
{
    Q_OBJECT

public:
    MTWidget(QWidget *parent = nullptr);
    ~MTWidget();

    void paintEvent(QPaintEvent *event) override;


private:
    MTPipeline pipeline;
    MTPipelineState ps;
    MTTexture texture;
    float angle;
};
#endif // MTWIDGET_H
