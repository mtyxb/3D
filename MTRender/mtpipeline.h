/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTPIPELINE_H
#define MTPIPELINE_H

#include "mtvector.h"
#include "mtmatrix.h"


class MTTexture;
class MTRaster;

struct MTShaderInput
{
    MTVector3  pos;
    MTVector3  texture_or_color;
    MTVector3  normal;
};

struct MTShaderOutput
{
    MTVector3 pos;
    MTVector4 view_pos;
    MTVector3 texture;
    MTVector4 color;
};

struct MTPipelineState
{
    MTPipelineState();

    MTMatrix<float, 4>  total_matrix;
    MTMatrix<float, 4>  mv_matrix;
    MTMatrix4x4  viewport_matrix;
    MTMatrix4x4  project_matrix;
    MTMatrix4x4  view_matrix;
    MTMatrix4x4  model_matrix;
    MTTexture*   texture;
    MTRaster*    raster;
    bool  with_texture;
};

class MTPipeline
{
public:
    MTPipeline();

    void point(MTPipelineState& state,
                const MTShaderInput& input,
                MTShaderOutput& output);

    void triangle(MTPipelineState& state,
                const MTShaderInput* vertexs, int count);

    void vertex(MTPipelineState& state,
                const MTShaderInput& input,
                MTShaderOutput& output);
    void fragment(MTPipelineState& state,
                const MTShaderInput& input,
                MTShaderOutput& output);

};

#endif // MTPIPELINE_H
