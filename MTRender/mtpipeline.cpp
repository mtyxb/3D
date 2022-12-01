/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include "mtpipeline.h"
#include "mttexture.h"
#include "mtraster.h"
#include <QDebug>

MTPipelineState::MTPipelineState()
{

}

MTPipeline::MTPipeline()
{

}

void MTPipeline::point(MTPipelineState& state,
                        const MTShaderInput& input,
                        MTShaderOutput& output)
{
   state.mv_matrix = state.view_matrix * state.model_matrix;
   state.total_matrix = state.viewport_matrix * state.project_matrix * state.view_matrix * state.model_matrix;
   vertex(state, input, output);
   fragment(state, input, output);
}

void MTPipeline::vertex(MTPipelineState &state, const MTShaderInput &input, MTShaderOutput &output)
{
    MTVector4 pos = {input.pos[0], input.pos[1], input.pos[2], 1.f};
    pos.divide_by_w();

    MTVector4 vp_pos = state.total_matrix * pos;
    vp_pos.divide_by_w().simplify();
    output.pos = MTVector3{vp_pos[0], vp_pos[1], vp_pos[2]};

    output.view_pos = state.mv_matrix * pos;
    output.view_pos.divide_by_w().simplify();
}

void MTPipeline::fragment(MTPipelineState &state, const MTShaderInput &input, MTShaderOutput &output)
{
    if (state.with_texture)
    {
        output.color = state.texture->texture(input.texture_or_color);
        output.texture = input.texture_or_color;
    }
    else
    {
        output.color = MTVector4{input.texture_or_color[0], input.texture_or_color[1], input.texture_or_color[2], 1.f};
    }
}

void MTPipeline::triangle(MTPipelineState &state, const MTShaderInput *vertexs, int count)
{
    state.mv_matrix = state.view_matrix * state.model_matrix;
    state.total_matrix = state.viewport_matrix * state.project_matrix * state.view_matrix * state.model_matrix;

    MTShaderOutput output;
    int triangle_count = count  / 3;
    MTVector3 pts[3];
    MTVector3 tcs[3];
    MTVector3 colors[3];
    MTVector4 view_pts[3];
    for (int i = 0; i < triangle_count; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            const MTShaderInput& input = *(vertexs + i * 3 + j);
            vertex(state, input, output);
            fragment(state, input, output);
            pts[j] = output.pos;
            tcs[j] = output.texture;
            colors[j] = MTVector3{output.color[0], output.color[1], output.color[2]};
            view_pts[j] = output.view_pos;
        }

        MTVector4 a = view_pts[0] - view_pts[1];
        MTVector4 b = view_pts[1] - view_pts[2];
        MTVector3 vn = MTVector3{a[0], a[1], a[2]}.cross(MTVector3{b[0], b[1], b[2]}).normalize().simplify();
        MTVector3 vc = MTVector3{0, 0, 1};
        if (vn.dot(vc) <= 0.f)
        {
            continue;
        }

        state.raster->triangle(state.with_texture, pts, state.with_texture ? tcs : colors);
    }
}
