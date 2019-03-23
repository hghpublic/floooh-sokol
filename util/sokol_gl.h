#pragma once
/*
    sokol_gl.h -- OpenGL 1.x style rendering on top of sokol_gfx.h

    WIP! Progress will be slow.

    Do this:
        #define SOKOL_GL_IMPL
    before you include this file in *one* C or C++ file to create the
    implementation.

    The following defines are used by the implementation to select the
    platform-specific embedded shader code (these are the same defines as
    used by sokol_gfx.h and sokol_app.h):

    SOKOL_GLCORE33
    SOKOL_GLES2
    SOKOL_GLES3
    SOKOL_D3D11
    SOKOL_METAL

    ...optionally provide the following macros to override defaults:

    SOKOL_ASSERT(c)     - your own assert macro (default: assert(c))
    SOKOL_MALLOC(s)     - your own malloc function (default: malloc(s))
    SOKOL_FREE(p)       - your own free function (default: free(p))
    SOKOL_API_DECL      - public function declaration prefix (default: extern)
    SOKOL_API_IMPL      - public function implementation prefix (default: -)
    SOKOL_UNREACHABLE() - a guard macro for unreachable code (default: assert(false))

    Include the following headers before including sokol_gl.h:

        sokol_gfx.h

    FEATURE OVERVIEW:
    =================
    FIXME

    HOWTO:
    ======
    FIXME

    LICENSE
    =======

    zlib/libpng license

    Copyright (c) 2018 Andre Weissflog

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/
#define SOKOL_GL_INCLUDED (1)
#include <stdint.h>
#include <stdbool.h>

#if !defined(SOKOL_GFX_INCLUDED)
#error "Please include sokol_gfx.h before sokol_gl.h"
#endif

#ifndef SOKOL_API_DECL
    #define SOKOL_API_DECL extern
#endif

/*
    sgl_primitive_type

    Used in sgl_begin() to start rendering this type of primitive.

    NOTE: The values are *not* identical with sg_primitive_type!
    When adding new primitive types, check the implementation
    for how primitive types and states are merged into
    an index for looking up pipeline objects!
*/
typedef enum sgl_primitive_type_t {
    SGL_POINTS = 0,
    SGL_LINES,
    SGL_LINE_STRIP,
    SGL_TRIANGLES,
    SGL_TRIANGLE_STRIP,
    SGL_NUM_PRIMITIVE_TYPES,
} sgl_primitive_type_t;

/*
    sgl_state_t

    Used in sgl_enable() / sgl_disable()

    NOTE: States are merged with primitive types into a bit mask,
    used as index for looking up pipeline state objects. When
    adding new states, make sure to update the implementation!
*/
typedef enum sgl_state_t {
    SGL_STATE_DEPTHTEST,        /* default: false */
    SGL_STATE_BLEND,            /* default: false */
    SGL_STATE_CULLFACE,         /* default: false */
    SGL_STATE_TEXTURING,        /* default: false */
    SGL_NUM_STATES
} sgl_state_t;

/*
    sgl_matrixmode_t

    Used in sgl_matrix_mode(). The default matrix mode is SGL_MODELVIEW
*/
typedef enum sgl_matrix_mode_t {
    SGL_MATRIXMODE_MODELVIEW,
    SGL_MATRIXMODE_PROJECTION,
    SGL_MATRIXMODE_TEXTURE,
    SGL_NUM_MATRIXMODES
} sgl_matrix_mode_t;

/*
    sgl_error_t

    Errors are reset each frame after calling sgl_draw(),
    get the last error code with sgl_error()
*/
typedef enum sgl_error_t {
    SGL_NO_ERROR = 0,
    SGL_ERROR_VERTICES_FULL,
    SGL_ERROR_UNIFORMS_FULL,
    SGL_ERROR_COMMANDS_FULL,
    SGL_ERROR_STACK_OVERFLOW,
    SGL_ERROR_STACK_UNDERFLOW,
} sgl_error_t;

/*
    sgl_texture_t 

    This is simply an alias for sg_image.
*/
typedef sg_image sgl_texture_t;

typedef struct sgl_desc_t { 
    int max_vertices;   /* size for vertex buffer */
    int max_commands;   /* size of uniform- and command-buffers */
    sg_pixel_format color_format;
    sg_pixel_format depth_format;
    int sample_count;
} sgl_desc_t;

/* setup/shutdown */
SOKOL_API_DECL void sgl_setup(const sgl_desc_t* desc);
SOKOL_API_DECL void sgl_shutdown(void);
SOKOL_API_DECL sgl_error_t sgl_error(void);

/* render state functions (only valid outside begin/end) */
SOKOL_API_DECL void sgl_enable(sgl_state_t state);
SOKOL_API_DECL void sgl_disable(sgl_state_t state);
SOKOL_API_DECL bool sgl_is_enabled(sgl_state_t state);
SOKOL_API_DECL void sgl_viewport(int x, int y, int w, int h, bool origin_top_left);
SOKOL_API_DECL void sgl_scissor_rect(int x, int y, int w, int h, bool origin_top_left);
SOKOL_API_DECL void sgl_texture(sgl_texture_t tex);
SOKOL_API_DECL void sgl_texcoord_int_bits(int u_bits, int v_bits);

/* these functions only set the internal 'current texcoord / color' (valid inside or outside begin/end) */
SOKOL_API_DECL void sgl_t2f(float u, float v);
SOKOL_API_DECL void sgl_c4f(float r, float g, float b, float a);
SOKOL_API_DECL void sgl_c4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SOKOL_API_DECL void sgl_c1i(uint32_t rgba);

/* define primitives, each begin/end is one draw command */
SOKOL_API_DECL void sgl_begin(sgl_primitive_type_t mode);
SOKOL_API_DECL void sgl_v2f(float x, float y);
SOKOL_API_DECL void sgl_v3f(float x, float y, float z);
SOKOL_API_DECL void sgl_v2f_t2f(float x, float y, float u, float v);
SOKOL_API_DECL void sgl_v3f_t2f(float x, float y, float z, float u, float v);
SOKOL_API_DECL void sgl_v2f_c4f(float x, float y, float r, float g, float b, float a);
SOKOL_API_DECL void sgl_v2f_c4b(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SOKOL_API_DECL void sgl_v2f_c1i(float x, float y, uint32_t rgba);
SOKOL_API_DECL void sgl_v3f_c4f(float x, float y, float z, float r, float g, float b, float a);
SOKOL_API_DECL void sgl_v3f_c4b(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SOKOL_API_DECL void sgl_v3f_c1i(float x, float y, float z, uint32_t rgba);
SOKOL_API_DECL void sgl_v2f_t2f_c4f(float x, float y, float u, float v, float r, float g, float b, float a);
SOKOL_API_DECL void sgl_v2f_t2f_c4b(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SOKOL_API_DECL void sgl_v2f_t2f_c1i(float x, float y, float u, float v, uint32_t rgba);
SOKOL_API_DECL void sgl_v3f_t2f_c4f(float x, float y, float z, float u, float v, float r, float g, float b, float a);
SOKOL_API_DECL void sgl_v3f_t2f_c4b(float x, float y, float z, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SOKOL_API_DECL void sgl_v3f_t2f_c1i(float x, float y, float z, float u, float v, uint32_t rgba);
SOKOL_API_DECL void sgl_end(void);

/* matrix stack functions (only valid outside begin end)
    cm = column major
    rm = row major
*/
SOKOL_API_DECL void sgl_matrix_mode(sgl_matrix_mode_t mode);
SOKOL_API_DECL void sgl_load_identity(void);
SOKOL_API_DECL void sgl_load_matrix_cm(const float m[16]);
SOKOL_API_DECL void sgl_load_matrix_rm(const float m[16]);
SOKOL_API_DECL void sgl_mult_matrix_cm(const float m[16]);
SOKOL_API_DECL void sgl_mult_matrix_rm(const float m[16]);
SOKOL_API_DECL void sgl_rotate(float angle, float x, float y, float z);
SOKOL_API_DECL void sgl_scale(float x, float y, float z);
SOKOL_API_DECL void sgl_translate(float x, float y, float z);
SOKOL_API_DECL void sgl_frustum(float l, float r, float b, float t, float n, float f);
SOKOL_API_DECL void sgl_ortho(float l, float r, float b, float t, float n, float f);
SOKOL_API_DECL void sgl_ortho2d(float left, float right, float bottom, float top);
SOKOL_API_DECL void sgl_perspective(float fov_y, float aspect, float z_near, float z_far);
SOKOL_API_DECL void sgl_lookat(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z);
SOKOL_API_DECL void sgl_push_matrix(void);
SOKOL_API_DECL void sgl_pop_matrix(void);

/* render everything */
SOKOL_API_DECL void sgl_draw(void);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef SOKOL_GL_IMPL
#define SOKOL_GL_IMPL_INCLUDED (1)

#include <stddef.h> /* offsetof */
#include <string.h> /* memset */

#ifndef SOKOL_API_IMPL
    #define SOKOL_API_IMPL
#endif
#ifndef SOKOL_DEBUG
    #ifndef NDEBUG
        #define SOKOL_DEBUG (1)
    #endif
#endif
#ifndef SOKOL_ASSERT
    #include <assert.h>
    #define SOKOL_ASSERT(c) assert(c)
#endif
#ifndef SOKOL_MALLOC
    #include <stdlib.h>
    #define SOKOL_MALLOC(s) malloc(s)
    #define SOKOL_FREE(p) free(p)
#endif
#ifndef SOKOL_UNREACHABLE
    #define SOKOL_UNREACHABLE SOKOL_ASSERT(false)
#endif
#ifndef _SOKOL_PRIVATE
    #if defined(__GNUC__)
        #define _SOKOL_PRIVATE __attribute__((unused)) static
    #else
        #define _SOKOL_PRIVATE static
    #endif
#endif

#define _sgl_def(val, def) (((val) == 0) ? (def) : (val))
#define _SGL_INIT_COOKIE (0xABCDABCD)

#if defined(SOKOL_GLCORE33)
static const char* _sgl_vs_src =
    "#version 330\n"
    "uniform mat4 mvp;\n"
    "uniform vec2 uv_scale;\n"
    "in vec4 position;\n"
    "in vec2 texcoord0;\n"
    "in vec4 color0;\n"
    "out vec2 uv;\n"
    "out vec4 color;\n"
    "void main() {\n"
    "    gl_Position = mvp * position;\n"
    "    uv = uv_scale * texcoord0;\n"
    "    color = color0;\n"
    "}\n";
static const char* _sgl_fs_src =
    "#version 330\n"
    "uniform sampler2D tex;\n"
    "in vec2 uv;\n"
    "in vec4 color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = texture(tex, uv) * color;\n"
    "}\n";
#elif defined(SOKOL_GLES2) || defined(SOKOL_GLES3)
static const char* _sgl_vs_src =
    "uniform mat4 mvp;\n"
    "uniform vec2 uv_scale;\n"
    "attribute vec4 position;\n"
    "attribute vec2 texcoord0;\n"
    "attribute vec4 color0;\n"
    "varying vec2 uv;\n"
    "varying vec4 color;\n"
    "void main() {\n"
    "    gl_Position = mvp * position;\n"
    "    uv = uv_scale * texcoord0;\n"
    "    color = color0;\n"
    "}\n";
static const char* _sgl_fs_src =
    "precision mediump float;\n"
    "uniform sampler2D tex;\n"
    "varying vec2 uv;\n"
    "varying vec4 color;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(tex, uv) * color;\n"
    "}\n";
#elif defined(SOKOL_METAL)
static const char* _sgl_vs_src =
    "#include <metal_stdlib>\n"
    "using namespace metal;\n"
    "struct params_t {\n"
    "  float4x4 mvp;\n"
    "  float2 uv_scale;\n"
    "};\n"
    "struct vs_in {\n"
    "  float4 pos [[attribute(0)]];\n"
    "  float2 uv [[attribute(1)]];\n"
    "  float4 color [[attribute(2)]];\n"
    "};\n"
    "struct vs_out {\n"
    "  float4 pos [[position]];\n"
    "  float2 uv;\n"
    "  float4 color;\n"
    "};\n"
    "vertex vs_out _main(vs_in in [[stage_in]], constant params_t& params [[buffer(0)]]) {\n"
    "  vs_out out;\n"
    "  out.pos = params.mvp * in.pos;\n"
    "  out.uv = params.uv_scale * in.uv;\n"
    "  out.color = in.color;\n"
    "  return out;\n"
    "}\n";
static const char* _sgl_fs_src =
    "#include <metal_stdlib>\n"
    "using namespace metal;\n"
    "struct fs_in {\n"
    "  float2 uv;\n"
    "  float4 color;\n"
    "};\n"
    "fragment float4 _main(fs_in in [[stage_in]], texture2d<float> tex [[texture(0)]], sampler smp [[sampler(0)]]) {\n"
    "  return tex.sample(smp, in.uv) * in.color;\n"
    "}\n";
#elif defined(SOKOL_D3D11)
static const char* _sgl_vs_src =
    "cbuffer params: register(b0) {\n"
    "  float4x4 mvp;\n"
    "  float2 uv_scale;\n"
    "};\n"
    "struct vs_in {\n"
    "  float4 pos: POS;\n"
    "  float2 uv: TEXCOORD0;\n"
    "  float4 color: COLOR0;\n"
    "};\n"
    "struct vs_out {\n"
    "  float2 uv: TEXCOORD0;\n"
    "  float4 color: COLOR0;\n"
    "  float4 pos: SV_Position;\n"
    "};\n"
    "vs_out main(vs_in inp) {\n"
    "  vs_out outp;\n"
    "  outp.pos = mul(mvp, inp.pos);\n"
    "  outp.uv = uv_scale * inp.uv;\n"
    "  outp.color = inp.color;\n"
    "  return outp;\n"
    "};\n";
static const char* _sgl_fs_src =
    "Texture2D<float4> tex: register(t0);\n"
    "sampler smp: register(s0);\n"
    "float4 main(float2 uv: TEXCOORD0, float4 color: COLOR0): SV_Target0 {\n"
    "  return tex.Sample(smp, uv) * color;\n"
    "}\n";
#elif defined(SOKOL_DUMMY_BACKEND)
static const char* _sgl_vs_src = "";
static const char* _sgl_fs_src = "";
#endif

typedef struct {
    float pos[3];
    int16_t uv[2];  /* texcoords as packed fixed-point format, see sgl_texcoord_int_bits */
    uint32_t rgba;
} _sgl_vertex_t;

typedef struct {
    float v[4][4];
} _sgl_matrix_t;

typedef struct {
    _sgl_matrix_t mvp;  /* model-view-projection matrix */
    float uv_scale[2];  /* scaler for converting fixed-point texcoord to float */
} _sgl_uniform_t;

typedef enum {
    SGL_COMMAND_DRAW,
    SGL_COMMAND_VIEWPORT,
    SGL_COMMAND_SCISSOR_RECT,
} _sgl_command_type_t;

typedef struct {
    sgl_texture_t tex;
    uint16_t state_bits;    /* bit mask with primitive type and render states */
    int base_vertex;
    int num_vertices;
    int uniform_index;
} _sgl_draw_args_t;

typedef struct {
    int x, y, w, h;
    bool origin_top_left;
} _sgl_viewport_args_t;

typedef struct {
    int x, y, w, h;
    bool origin_top_left;
} _sgl_scissor_rect_args_t;

typedef union {
    _sgl_draw_args_t draw;
    _sgl_viewport_args_t viewport;
    _sgl_scissor_rect_args_t scissor_rect;
} _sgl_args_t;

typedef struct {
    _sgl_command_type_t cmd;
    _sgl_args_t args;
} _sgl_command_t;

/* number of pipelines: 3 bits for primitive type, 3 relevant render state bits */
#define _SGL_MAX_PIPELINES (64)
/* matrix stack depth */
#define _SGL_MAX_STACK_DEPTH (64)

typedef struct {
    uint32_t init_cookie;

    int num_vertices;
    int num_uniforms;
    int num_commands;
    int cur_vertex;
    int cur_uniform;
    int cur_command;
    _sgl_vertex_t* vertices;
    _sgl_uniform_t* uniforms;
    _sgl_command_t* commands;

    /* state tracking */
    int base_vertex;
    sgl_error_t error;
    bool in_begin;
    uint16_t state_bits;        /* bitmask with primitive type and render states */
    float u_scale, v_scale;
    int16_t u, v;
    uint32_t rgba;
    sgl_texture_t tex;

    /* sokol-gfx resources */
    sg_buffer vbuf;
    sg_image img;   /* a default white texture */
    sg_shader shd;
    sg_bindings bind;
    sg_pipeline pip[_SGL_MAX_PIPELINES];
    sg_pipeline_desc pip_desc;  /* template for lazy pipeline creation */

    /* matrix stacks */
    sgl_matrix_mode_t cur_matrix_mode;
    int top_of_stack[SGL_NUM_MATRIXMODES];
    _sgl_matrix_t matrix_stack[SGL_NUM_MATRIXMODES][_SGL_MAX_STACK_DEPTH];
} _sgl_t;
static _sgl_t _sgl;

/*== PRIVATE FUNCTIONS =======================================================*/
_SOKOL_PRIVATE void _sgl_rewind(void) {
    _sgl.base_vertex = 0;
    _sgl.cur_vertex = 0;
    _sgl.cur_uniform = 0;
    _sgl.cur_command = 0;
    _sgl.error = SGL_NO_ERROR;
}

static inline _sgl_vertex_t* _sgl_next_vertex(void) {
    if (_sgl.cur_vertex < _sgl.num_vertices) {
        return &_sgl.vertices[_sgl.cur_vertex++];
    }
    else {
        _sgl.error = SGL_ERROR_VERTICES_FULL;
        return 0;
    }
}

static inline _sgl_uniform_t* _sgl_next_uniform(void) {
    if (_sgl.cur_uniform < _sgl.num_uniforms) {
        return &_sgl.uniforms[_sgl.cur_uniform++];
    }
    else {
        _sgl.error = SGL_ERROR_UNIFORMS_FULL;
        return 0;
    }
}

static inline _sgl_command_t* _sgl_next_command(void) {
    if (_sgl.cur_command < _sgl.num_commands) {
        return &_sgl.commands[_sgl.cur_command++];
    }
    else {
        _sgl.error = SGL_ERROR_COMMANDS_FULL;
        return 0;
    }
}

static inline int16_t _sgl_pack_u(float u) {
    return (int16_t) ((u * (1<<15)) / _sgl.u_scale);
}

static inline int16_t _sgl_pack_v(float v) {
    return (int16_t) ((v * (1<<15)) / _sgl.v_scale);
}

static inline uint32_t _sgl_pack_rgbab(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (uint32_t)((a<<24)|(b<<16)|(g<<8)|r);
}

static inline uint32_t _sgl_pack_rgbaf(float r, float g, float b, float a) {
    uint8_t r_u8 = (uint8_t) (r * 255.0f);
    uint8_t g_u8 = (uint8_t) (g * 255.0f);
    uint8_t b_u8 = (uint8_t) (b * 255.0f);
    uint8_t a_u8 = (uint8_t) (a * 255.0f);
    return _sgl_pack_rgbab(r_u8, g_u8, b_u8, a_u8);
}

static inline void _sgl_vtx(float x, float y, float z, int16_t u, int16_t v, uint32_t rgba) {
    SOKOL_ASSERT(_sgl.in_begin);
    _sgl_vertex_t* vtx = _sgl_next_vertex();
    if (vtx) {
        vtx->pos[0] = x; vtx->pos[1] = y; vtx->pos[2] = z;
        vtx->uv[0] = u; vtx->uv[1] = v;
        vtx->rgba = rgba;
    }
}

/* set primitive type in 16-bit merged state */
static inline uint16_t _sgl_set_prim_type(sgl_primitive_type_t type, uint16_t bits) {
    SOKOL_ASSERT(((int)type) < 8);
    return (bits & ~7) | (type & 7);
}

/* extract primitive type from 16-bit merged state */
static inline sgl_primitive_type_t _sgl_prim_type(uint16_t bits) {
    return (sgl_primitive_type_t) (bits & 7);
}

/* set render state bit in 16-bit merged state */
static inline uint16_t _sgl_enable_state(sgl_state_t state, uint16_t bits) {
    /* first 3 bits are used by the primitive type */
    return bits | (8<<state);
}

/* clear render state bit in 16-bit merged state */
static inline uint16_t _sgl_disable_state(sgl_state_t state, uint16_t bits) {
    /* first 3 bits are used by the primitive type */
    return bits & ~(8<<state);
}

/* get render state from merged state bit mask */
static inline bool _sgl_state(sgl_state_t state, uint16_t bits) {
    return 0 != (bits & (8<<state));
}

/* get pipeline index from merged primitive-type / render state bit mask */
static inline int _sgl_pipeline_index(uint16_t state) {
    return (int) (state & (_SGL_MAX_PIPELINES-1));
}

/* lookup or lazy-create pipeline object */
static inline sg_pipeline _sgl_pipeline(uint16_t state_bits) {
    int pip_index = _sgl_pipeline_index(state_bits);
    if (SG_INVALID_ID == _sgl.pip[pip_index].id) {
        _sgl.pip_desc.blend.enabled = _sgl_state(SGL_STATE_BLEND, state_bits);
        _sgl.pip_desc.rasterizer.cull_mode = _sgl_state(SGL_STATE_CULLFACE, state_bits) ? SG_CULLMODE_BACK : SG_CULLMODE_NONE;
        _sgl.pip_desc.depth_stencil.depth_compare_func = _sgl_state(SGL_STATE_DEPTHTEST, state_bits) ? SG_COMPAREFUNC_LESS_EQUAL : SG_COMPAREFUNC_ALWAYS;
        switch (_sgl_prim_type(state_bits)) {
            case SGL_POINTS: _sgl.pip_desc.primitive_type = SG_PRIMITIVETYPE_POINTS; break;
            case SGL_LINES: _sgl.pip_desc.primitive_type = SG_PRIMITIVETYPE_LINES; break;
            case SGL_LINE_STRIP: _sgl.pip_desc.primitive_type = SG_PRIMITIVETYPE_LINE_STRIP; break;
            case SGL_TRIANGLES: _sgl.pip_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES; break;
            case SGL_TRIANGLE_STRIP: _sgl.pip_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLE_STRIP; break;
            default: SOKOL_UNREACHABLE; break;
        }
        _sgl.pip[pip_index] = sg_make_pipeline(&_sgl.pip_desc);
    }
    return _sgl.pip[pip_index];
}

/* initialize identity matrix */
static inline void _sgl_identity(_sgl_matrix_t* m) {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            m->v[c][r] = (c == r) ? 1.0f : 0.0f;
        }
    }
}

/* multiply to (row-major) matrices */
static inline _sgl_matrix_t _sgl_mul(const _sgl_matrix_t* m0, const _sgl_matrix_t* m1) {
    _sgl_matrix_t res;
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            res.v[c][r] = m0->v[0][r] * m1->v[c][0] + 
                          m0->v[1][r] * m1->v[c][1] + 
                          m0->v[2][r] * m1->v[c][2] + 
                          m0->v[3][r] * m1->v[c][3];
        }
    }
    return res;
}

/* return transposed matrix */
static inline _sgl_matrix_t _sgl_transpose(const _sgl_matrix_t* m) {
    _sgl_matrix_t res;
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            res.v[c][r] = m->v[r][c];
        }
    }
    return res;
}

/* current top-of-stack projection matrix */
static inline _sgl_matrix_t* _sgl_matrix_projection(void) {
    return &_sgl.matrix_stack[SGL_MATRIXMODE_PROJECTION][_sgl.top_of_stack[SGL_MATRIXMODE_PROJECTION]];
}

/* get top-of-stack modelview matrix */
static inline _sgl_matrix_t* _sgl_matrix_modelview(void) {
    return &_sgl.matrix_stack[SGL_MATRIXMODE_MODELVIEW][_sgl.top_of_stack[SGL_MATRIXMODE_MODELVIEW]];
}

/* get top-of-stack texture matrix */
static inline _sgl_matrix_t* _sgl_matrix_texture(void) {
    return &_sgl.matrix_stack[SGL_MATRIXMODE_TEXTURE][_sgl.top_of_stack[SGL_MATRIXMODE_TEXTURE]];
}

/* get pointer to current top-of-stack of current matrix mode */
static inline _sgl_matrix_t* _sgl_matrix(void) {
    return &_sgl.matrix_stack[_sgl.cur_matrix_mode][_sgl.top_of_stack[_sgl.cur_matrix_mode]];
}

/*== PUBLIC FUNCTIONS ========================================================*/
SOKOL_API_IMPL void sgl_setup(const sgl_desc_t* desc) {
    SOKOL_ASSERT(desc);
    memset(&_sgl, 0, sizeof(_sgl));
    _sgl.init_cookie = _SGL_INIT_COOKIE;

    /* allocate buffers */
    _sgl.num_vertices = _sgl_def(desc->max_vertices, (1<<16));
    _sgl.num_uniforms = _sgl_def(desc->max_commands, (1<<14));
    _sgl.num_commands = _sgl.num_uniforms;
    _sgl.vertices = (_sgl_vertex_t*) SOKOL_MALLOC(_sgl.num_vertices * sizeof(_sgl_vertex_t));
    SOKOL_ASSERT(_sgl.vertices);
    _sgl.uniforms = (_sgl_uniform_t*) SOKOL_MALLOC(_sgl.num_uniforms * sizeof(_sgl_uniform_t));
    SOKOL_ASSERT(_sgl.uniforms);
    _sgl.commands = (_sgl_command_t*) SOKOL_MALLOC(_sgl.num_commands * sizeof(_sgl_command_t));
    SOKOL_ASSERT(_sgl.commands);

    /* default state */
    _sgl.u_scale = _sgl.v_scale = 1.0f;
    _sgl.rgba = 0xFFFFFFFF;
    for (int i = 0; i < SGL_NUM_MATRIXMODES; i++) {
        _sgl_identity(&_sgl.matrix_stack[i][0]);
    }

    /* create sokol-gfx resource objects */
    sg_push_debug_group("sokol-gl");

    sg_buffer_desc vbuf_desc;
    memset(&vbuf_desc, 0, sizeof(vbuf_desc));
    vbuf_desc.size = _sgl.num_vertices * sizeof(_sgl_vertex_t);
    vbuf_desc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    vbuf_desc.usage = SG_USAGE_STREAM;
    vbuf_desc.label = "sgl-vertex-buffer";
    _sgl.vbuf = sg_make_buffer(&vbuf_desc);
    SOKOL_ASSERT(SG_INVALID_ID != _sgl.vbuf.id);

    uint32_t pixels[64];
    for (int i = 0; i < 64; i++) {
        pixels[i] = 0xFFFFFFFF;
    }
    sg_image_desc img_desc;
    memset(&img_desc, 0, sizeof(img_desc));
    img_desc.type = SG_IMAGETYPE_2D;
    img_desc.width = 8;
    img_desc.height = 8;
    img_desc.num_mipmaps = 1;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.min_filter = SG_FILTER_NEAREST;
    img_desc.mag_filter = SG_FILTER_NEAREST;
    img_desc.content.subimage[0][0].ptr = pixels;
    img_desc.content.subimage[0][0].size = sizeof(pixels);
    img_desc.label = "sgl-default-texture";
    _sgl.img = sg_make_image(&img_desc);
    SOKOL_ASSERT(SG_INVALID_ID != _sgl.img.id);
    _sgl.tex = _sgl.img;

    sg_shader_desc shd_desc;
    memset(&shd_desc, 0, sizeof(shd_desc));
    sg_shader_uniform_block_desc* ub = &shd_desc.vs.uniform_blocks[0];
    ub->size = sizeof(_sgl_uniform_t);
    ub->uniforms[0].name = "mvp";
    ub->uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    ub->uniforms[1].name = "uv_scale";
    ub->uniforms[1].type = SG_UNIFORMTYPE_FLOAT2;
    shd_desc.fs.images[0].name = "tex";
    shd_desc.fs.images[0].type = SG_IMAGETYPE_2D;
    shd_desc.vs.source = _sgl_vs_src;
    shd_desc.fs.source = _sgl_fs_src;
    shd_desc.label = "sgl-shader";
    _sgl.shd = sg_make_shader(&shd_desc);
    sg_pop_debug_group();

    /* template desc for lazy pipeline creation */
    _sgl.pip_desc.layout.buffers[0].stride = sizeof(_sgl_vertex_t);
    {
        sg_vertex_attr_desc* pos = &_sgl.pip_desc.layout.attrs[0];
        pos->name = "position";
        pos->sem_name = "POSITION";
        pos->offset = offsetof(_sgl_vertex_t, pos);
        pos->format = SG_VERTEXFORMAT_FLOAT3;
    }
    {
        sg_vertex_attr_desc* uv = &_sgl.pip_desc.layout.attrs[1];
        uv->name = "texcoord0";
        uv->sem_name = "TEXCOORD";
        uv->offset = offsetof(_sgl_vertex_t, uv);
        uv->format = SG_VERTEXFORMAT_SHORT2N;
    }
    {
        sg_vertex_attr_desc* rgba = &_sgl.pip_desc.layout.attrs[2];
        rgba->name = "color0";
        rgba->sem_name = "TEXCOORD";
        rgba->offset = offsetof(_sgl_vertex_t, rgba);
        rgba->format = SG_VERTEXFORMAT_UBYTE4N;
    }
    _sgl.pip_desc.shader = _sgl.shd;
    _sgl.pip_desc.index_type = SG_INDEXTYPE_NONE;
    _sgl.pip_desc.depth_stencil.depth_write_enabled = true;
    _sgl.pip_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    _sgl.pip_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    _sgl.pip_desc.blend.color_write_mask = SG_COLORMASK_RGB;
    _sgl.pip_desc.blend.color_format = desc->color_format;
    _sgl.pip_desc.blend.depth_format = desc->depth_format;
    _sgl.pip_desc.rasterizer.sample_count = desc->sample_count;
}

SOKOL_API_IMPL void sgl_shutdown(void) {
    SOKOL_ASSERT(_sgl.init_cookie == 0xABCDABCD);
    SOKOL_FREE(_sgl.vertices); _sgl.vertices = 0;
    SOKOL_FREE(_sgl.uniforms); _sgl.uniforms = 0;
    SOKOL_FREE(_sgl.commands = _sgl.commands = 0);
    sg_destroy_buffer(_sgl.vbuf);
    sg_destroy_image(_sgl.img);
    sg_destroy_shader(_sgl.shd);
    /* NOTE: calling sg_destroy_*() with an invalid id is valid */
    for (int i = 0; i < _SGL_MAX_PIPELINES; i++) {
        sg_destroy_pipeline(_sgl.pip[i]);
    }
    _sgl.init_cookie = 0;
}

SOKOL_API_IMPL sgl_error_t sgl_error(void) {
    return _sgl.error;
}

SOKOL_API_IMPL void sgl_enable(sgl_state_t state) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    _sgl.state_bits = _sgl_enable_state(state, _sgl.state_bits);
}

SOKOL_API_IMPL void sgl_disable(sgl_state_t state) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    _sgl.state_bits = _sgl_disable_state(state, _sgl.state_bits);
}

SOKOL_API_IMPL bool sgl_is_enabled(sgl_state_t state) {
    SOKOL_ASSERT((state >= 0) && (state < SGL_NUM_STATES));
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    return _sgl_state(state, _sgl.state_bits);
}

SOKOL_API_IMPL void sgl_viewport(int x, int y, int w, int h, bool origin_top_left) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    _sgl_command_t* cmd = _sgl_next_command();
    if (cmd) {
        cmd->cmd = SGL_COMMAND_VIEWPORT;
        cmd->args.viewport.x = x;
        cmd->args.viewport.y = y;
        cmd->args.viewport.w = w;
        cmd->args.viewport.h = h;
        cmd->args.viewport.origin_top_left = origin_top_left;
    }
}

SOKOL_API_IMPL void sgl_scissor_rect(int x, int y, int w, int h, bool origin_top_left) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    _sgl_command_t* cmd = _sgl_next_command();
    if (cmd) {
        cmd->cmd = SGL_COMMAND_SCISSOR_RECT;
        cmd->args.scissor_rect.x = x;
        cmd->args.scissor_rect.y = y;
        cmd->args.scissor_rect.w = w;
        cmd->args.scissor_rect.h = h;
        cmd->args.scissor_rect.origin_top_left = origin_top_left;
    }
}

SOKOL_API_IMPL void sgl_texture(sgl_texture_t tex) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    if (SG_INVALID_ID != tex.id) {
        _sgl.tex = tex;
    }
    else {
        _sgl.tex = _sgl.img;
    }
}

SOKOL_API_IMPL void sgl_texcoord_int_bits(int u_bits, int v_bits) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(!_sgl.in_begin);
    SOKOL_ASSERT((u_bits >= 0) && (u_bits <= 15));
    SOKOL_ASSERT((v_bits >= 0) && (v_bits <= 15));
    _sgl.u_scale = (float)(1<<u_bits);
    _sgl.v_scale = (float)(1<<v_bits);
}

SOKOL_API_IMPL void sgl_begin(sgl_primitive_type_t mode) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT((mode >= 0) && (mode <= SGL_NUM_PRIMITIVE_TYPES));
    SOKOL_ASSERT(!_sgl.in_begin);
    _sgl.in_begin = true;
    _sgl.base_vertex = _sgl.cur_vertex;
    _sgl.state_bits = _sgl_set_prim_type(mode, _sgl.state_bits);
}

SOKOL_API_IMPL void sgl_end(void) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT(_sgl.in_begin);
    _sgl.in_begin = false;
    if (_sgl.base_vertex == _sgl.cur_vertex) {
        return;
    }
    _sgl_command_t* cmd = _sgl_next_command();
    if (cmd) {
        cmd->cmd = SGL_COMMAND_DRAW;
        cmd->args.draw.tex = _sgl.tex;
        cmd->args.draw.state_bits = _sgl.state_bits;
        cmd->args.draw.base_vertex = _sgl.base_vertex;
        cmd->args.draw.num_vertices = _sgl.cur_vertex - _sgl.base_vertex;
        cmd->args.draw.uniform_index = _sgl.cur_uniform;
    }
    _sgl_uniform_t* uni = _sgl_next_uniform();
    if (uni) {
        /* FIXME: update the model-view-proj matrix lazily */
        uni->mvp = _sgl_mul(_sgl_matrix_projection(), _sgl_matrix_modelview());
        uni->uv_scale[0] = _sgl.u_scale;
        uni->uv_scale[1] = _sgl.v_scale;
    }
}

SOKOL_API_IMPL void sgl_t2f(float u, float v) {
    _sgl.u = _sgl_pack_u(u);
    _sgl.v = _sgl_pack_u(v);
}

SOKOL_API_IMPL void sgl_c4f(float r, float g, float b, float a) {
    _sgl.rgba = _sgl_pack_rgbaf(r, g, b, a);
}

SOKOL_API_IMPL void sgl_c4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _sgl.rgba = _sgl_pack_rgbab(r, g, b, a);
}

SOKOL_API_IMPL void sgl_c1i(uint32_t rgba) {
    _sgl.rgba = rgba;
}

SOKOL_API_IMPL void sgl_v2f(float x, float y) {
    _sgl_vtx(x, y, 0.0f, _sgl.u, _sgl.v, _sgl.rgba);
}

SOKOL_API_IMPL void sgl_v3f(float x, float y, float z) {
    _sgl_vtx(x, y, z, _sgl.u, _sgl.v, _sgl.rgba);
}

SOKOL_API_IMPL void sgl_v2f_t2f(float x, float y, float u, float v) {
    _sgl_vtx(x, y, 0.0f, _sgl_pack_u(u), _sgl_pack_v(v), _sgl.rgba);
}

SOKOL_API_IMPL void sgl_v3f_t2f(float x, float y, float z, float u, float v) {
    _sgl_vtx(x, y, z, _sgl_pack_u(u), _sgl_pack_v(v), _sgl.rgba);
}

SOKOL_API_IMPL void sgl_v2f_c4f(float x, float y, float r, float g, float b, float a) {
    _sgl_vtx(x, y, 0.0f, _sgl.u, _sgl.v, _sgl_pack_rgbaf(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v2f_c4b(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _sgl_vtx(x, y, 0.0f, _sgl.u, _sgl.v, _sgl_pack_rgbab(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v2f_c1i(float x, float y, uint32_t rgba) {
    _sgl_vtx(x, y, 0.0f, _sgl.u, _sgl.v, rgba);
}

SOKOL_API_IMPL void sgl_v3f_c4f(float x, float y, float z, float r, float g, float b, float a) {
    _sgl_vtx(x, y, z, _sgl.u, _sgl.v, _sgl_pack_rgbaf(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v3f_c4b(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _sgl_vtx(x, y, z, _sgl.u, _sgl.v, _sgl_pack_rgbab(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v3f_c1i(float x, float y, float z, uint32_t rgba) {
    _sgl_vtx(x, y, z, _sgl.u, _sgl.v, rgba);
}

SOKOL_API_IMPL void sgl_v2f_t2f_c4f(float x, float y, float u, float v, float r, float g, float b, float a) {
    _sgl_vtx(x, y, 0.0f, _sgl_pack_u(u), _sgl_pack_v(v), _sgl_pack_rgbaf(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v2f_t2f_c4b(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _sgl_vtx(x, y, 0.0f, _sgl_pack_u(u), _sgl_pack_v(v), _sgl_pack_rgbab(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v2f_t2f_c1i(float x, float y, float u, float v, uint32_t rgba) {
    _sgl_vtx(x, y, 0.0f, _sgl_pack_u(u), _sgl_pack_v(v), rgba);
}

SOKOL_API_IMPL void sgl_v3f_t2f_c4f(float x, float y, float z, float u, float v, float r, float g, float b, float a) {
    _sgl_vtx(x, y, z, _sgl_pack_u(u), _sgl_pack_v(v), _sgl_pack_rgbaf(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v3f_t2f_c4b(float x, float y, float z, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _sgl_vtx(x, y, z, _sgl_pack_u(u), _sgl_pack_v(v), _sgl_pack_rgbab(r, g, b, a));
}

SOKOL_API_IMPL void sgl_v3f_t2f_c1i(float x, float y, float z, float u, float v, uint32_t rgba) {
    _sgl_vtx(x, y, z, _sgl_pack_u(u), _sgl_pack_v(v), rgba);
}

SOKOL_API_IMPL void sgl_matrix_mode(sgl_matrix_mode_t mode) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    SOKOL_ASSERT((mode >= 0) && (mode < SGL_NUM_MATRIXMODES));
    _sgl.cur_matrix_mode = mode;
}

SOKOL_API_IMPL void sgl_load_identify(void) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    _sgl_identity(_sgl_matrix());
}

SOKOL_API_IMPL void sgl_load_matrix_cm(const float m[16]) {
    _sgl_matrix_t* dst = _sgl_matrix();
    memcpy(&dst->v[0][0], &m[0], 64);
}

SOKOL_API_IMPL void sgl_load_matrix_rm(const float m[16]) {
    _sgl_matrix_t* dst = _sgl_matrix();
    *dst = _sgl_transpose((const _sgl_matrix_t*) &m[0]);
}

SOKOL_API_IMPL void sgl_mult_matrix_cm(const float m[16]) {
    _sgl_matrix_t* m1 = _sgl_matrix();
    const _sgl_matrix_t* m0  = (const _sgl_matrix_t*) &m[0];
    /* order? */
    *m1 = _sgl_mul(m0, m1);
}

SOKOL_API_IMPL void sgl_mult_matrix_rm(const float m[16]) {
    _sgl_matrix_t* m1 = _sgl_matrix();
    _sgl_matrix_t m0 = _sgl_transpose((const _sgl_matrix_t*) &m[0]);
    /* order? */
    *m1 = _sgl_mul(&m0, m1);
}

/* this draw the accumulated draw commands via sokol-gfx */
SOKOL_API_IMPL void sgl_draw(void) {
    SOKOL_ASSERT(_SGL_INIT_COOKIE == _sgl.init_cookie);
    if ((_sgl.error == SGL_NO_ERROR) && (_sgl.cur_vertex > 0) && (_sgl.cur_command > 0)) {
        sg_push_debug_group("sokol-gl");
        sg_update_buffer(_sgl.vbuf, _sgl.vertices, _sgl.cur_vertex * sizeof(_sgl_vertex_t));
        _sgl.bind.vertex_buffers[0] = _sgl.vbuf;
        for (int i = 0; i < _sgl.cur_command; i++) {
            const _sgl_command_t* cmd = &_sgl.commands[i];
            switch (cmd->cmd) {
                case SGL_COMMAND_VIEWPORT:
                    { 
                        const _sgl_viewport_args_t* args = &cmd->args.viewport;
                        sg_apply_viewport(args->x, args->y, args->w, args->h, args->origin_top_left);
                    }
                    break;
                case SGL_COMMAND_SCISSOR_RECT:
                    {
                        const _sgl_scissor_rect_args_t* args = &cmd->args.scissor_rect;
                        sg_apply_scissor_rect(args->x, args->y, args->w, args->h, args->origin_top_left);
                    }
                    break;
                case SGL_COMMAND_DRAW:
                    {
                        /* FIXME: don't apply redundant state */
                        const _sgl_draw_args_t* args = &cmd->args.draw;
                        sg_pipeline pip = _sgl_pipeline(args->state_bits);
                        sg_apply_pipeline(pip);
                        _sgl.bind.fs_images[0] = args->tex;
                        sg_apply_bindings(&_sgl.bind);
                        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &_sgl.uniforms[args->uniform_index], sizeof(_sgl_uniform_t));
                        /* FIXME: what if number of vertices doesn't match the primitive type? */
                        sg_draw(args->base_vertex, args->num_vertices, 1);
                    }
                    break;
            }
        }
        sg_pop_debug_group();
    }
    _sgl_rewind();
}
#endif /* SOKOL_GL_IMPL */
