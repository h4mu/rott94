#include "hw_draw.h"
#include "../rt_def.h"
#include "../rt_draw.h"
#include "../rt_ted.h"
#include "../rt_main.h"
#include "../rt_game.h"
#include "../rt_actor.h"
#include "../lumpy.h"
#include "../z_zone.h" // For PU_CACHE
#include "../engine.h"
#include "../rt_door.h"
#include "../rt_floor.h"

#define MAX_VERTICES 128000
#define MAX_DRAW_CMDS 8192

static hw_vertex_t wall_vertices[MAX_VERTICES];
static hw_draw_cmd_t draw_cmds[MAX_DRAW_CMDS];

static int vertex_count = 0;
static int cmd_count = 0;

void HW_InitDraw(void)
{
    // Any initialization for 3D drawing
}

static void EmitQuad(float x1, float y1, float z1, float u1, float v1,
                     float x2, float y2, float z2, float u2, float v2,
                     float x3, float y3, float z3, float u3, float v3,
                     float x4, float y4, float z4, float u4, float v4,
                     float r, float g, float b, float a, int texture)
{
    if (vertex_count + 6 > MAX_VERTICES || cmd_count + 1 > MAX_DRAW_CMDS) return;
    
    // Group by texture if possible
    if (cmd_count > 0 && draw_cmds[cmd_count - 1].texture_lump == texture) {
        draw_cmds[cmd_count - 1].vertex_count += 6;
    } else {
        draw_cmds[cmd_count].texture_lump = texture;
        draw_cmds[cmd_count].start_vertex = vertex_count;
        draw_cmds[cmd_count].vertex_count = 6;
        cmd_count++;
    }
    
    // Triangle 1
    wall_vertices[vertex_count++] = (hw_vertex_t){x1, y1, z1, u1, v1, r, g, b, a};
    wall_vertices[vertex_count++] = (hw_vertex_t){x2, y2, z2, u2, v2, r, g, b, a};
    wall_vertices[vertex_count++] = (hw_vertex_t){x3, y3, z3, u3, v3, r, g, b, a};
    
    // Triangle 2
    wall_vertices[vertex_count++] = (hw_vertex_t){x1, y1, z1, u1, v1, r, g, b, a};
    wall_vertices[vertex_count++] = (hw_vertex_t){x3, y3, z3, u3, v3, r, g, b, a};
    wall_vertices[vertex_count++] = (hw_vertex_t){x4, y4, z4, u4, v4, r, g, b, a};
}

void HW_BuildPlaneGeometry(void)
{
    float light = 1.0f;
    
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
            if (!spotvis[x][y]) continue;
            
            // Floor
            int floornum = MAPSPOT(0, x, y);
            if (floornum >= 179) {
                int floorlump = GetFloorCeilingLump(floornum - 179);
                if (floorlump > 0) {
                    float fx = (float)x;
                    float fy = (float)y;
                    EmitQuad(fx,   fy,   0, 0, 0,
                             fx+1, fy,   0, 1, 0,
                             fx+1, fy+1, 0, 1, 1,
                             fx,   fy+1, 0, 0, 1,
                             light, light, light, 1.0f, floorlump);
                }
            }
            
            // Ceiling
            if (sky == 0) {
                int ceilingnum = MAPSPOT(1, x, y);
                if (ceilingnum >= 197) {
                    int ceilinglump = GetFloorCeilingLump(ceilingnum - 197);
                    if (ceilinglump > 0) {
                        float fx = (float)x;
                        float fy = (float)y;
                        EmitQuad(fx,   fy,   1.0f, 0, 0,
                                 fx+1, fy,   1.0f, 1, 0,
                                 fx+1, fy+1, 1.0f, 1, 1,
                                 fx,   fy+1, 1.0f, 0, 1,
                                 light, light, light, 1.0f, ceilinglump);
                    }
                }
            }
        }
    }
}

void HW_BuildSpriteGeometry(int numvisible)
{
    float cam_angle = (viewangle * 2.0f * (float)M_PI) / 2048.0f;
    float rx = -sinf(cam_angle);
    float ry = cosf(cam_angle);
    
    extern byte * colormap;

    for (int i = 0; i < numvisible; i++) {
        visobj_t* v = &vislist[i];
        
        // Plane-based objects: Masked Walls (2), Doors (3 or >= 128)
        if (v->shapesize == 2 || v->shapesize == 3 || v->shapesize >= 128) {
            float x1 = v->x / 65536.0f;
            float y1 = v->y / 65536.0f;
            float x2 = v->world_x2 / 65536.0f;
            float y2 = v->world_y2 / 65536.0f;
            float z = v->z / 64.0f; // Normalize height (64 units = 1 tile)
            
            // For now, assume full tile height (64 world units = 1.0 tile unit)
            float h = 1.0f; 
            
            float light = 1.0f;
            if (v->colormap >= colormap && v->colormap < colormap + (32 << 8)) {
                int shade = (v->colormap - colormap) >> 8;
                light = (31 - shade) / 31.0f;
                if (light < 0) light = 0;
                if (light > 1) light = 1;
            }

            // Emit plane quad
            EmitQuad(x1, y1, z,     0, 1,
                     x2, y2, z,     1, 1,
                     x2, y2, z + h, 1, 0,
                     x1, y1, z + h, 0, 0,
                     light, light, light, 1.0f, v->shapenum);
            continue;
        }

        // Sprite-based objects: Normal (0), Translucent (1), Special (4)
        if (v->shapesize != 0 && v->shapesize != 1 && v->shapesize != 4) continue;
        
        float vx = v->x / 65536.0f;
        float vy = v->y / 65536.0f;
        float vz = v->z / 64.0f; 
        
        patch_t* p = (patch_t*)W_CacheLumpNum(v->shapenum, PU_CACHE, Cvt_patch_t, 1);
        
        // Scale factor: 64 pixels = 1.0 tile unit
        float w = p->width / 64.0f;
        float h = p->height / 64.0f;
        
        // In ROTT, sprites are horizontally centered using leftoffset
        float leftOffset = p->leftoffset / 64.0f;
        float rightOffset = (p->width - p->leftoffset) / 64.0f;
        
        // Vertical offset: topoffset is distance from top of sprite to origin (vz)
        // vz is usually at the bottom of the sprite in ROTT? No, topoffset 
        // usually puts it at the feet.
        float topOffset = p->topoffset / 64.0f;
        float topZ = vz + topOffset;
        float bottomZ = vz + (topOffset - h);
        
        // Add a tiny Z-offset to prevent z-fighting with floors
        topZ += 0.001f;
        bottomZ += 0.001f;

        float light = 1.0f;
        if (v->colormap >= colormap && v->colormap < colormap + (32 << 8)) {
            int shade = (v->colormap - colormap) >> 8;
            light = (31 - shade) / 31.0f;
            if (light < 0) light = 0;
            if (light > 1) light = 1;
        }

        float alpha = 1.0f;
        if (v->shapesize == 1) {
            // Translucent shape
            alpha = v->h2 / 255.0f;
        }
        
        // Billboard quad with horizontal and vertical offset
        EmitQuad(vx - rx * leftOffset,  vy - ry * leftOffset,  bottomZ,  0, 1,
                 vx + rx * rightOffset, vy + ry * rightOffset, bottomZ,  1, 1,
                 vx + rx * rightOffset, vy + ry * rightOffset, topZ,     1, 0,
                 vx - rx * leftOffset,  vy - ry * leftOffset,  topZ,     0, 0,
                 light, light, light, alpha, v->shapenum);
    }
}

void HW_BuildSkyGeometry(void)
{
    if (!sky) return;
    
    float r = 20000.0f; 
    float h = 10000.0f; 
    float vx = viewx / 65536.0f;
    float vy = viewy / 65536.0f;
    float vz = (float)pheight;
    
    float light = 1.0f;
    int segments = 32;
    for (int i = 0; i < segments; i++) {
        float a1 = i * (2.0f * (float)M_PI / (float)segments);
        float a2 = (i + 1) * (2.0f * (float)M_PI / (float)segments);
        
        // ROTT sky texture is 256 columns. If it repeats 4 times:
        float u1 = (float)i / (float)segments * 4.0f;
        float u2 = (float)(i + 1) / (float)segments * 4.0f;
        
        EmitQuad(vx + r*cosf(a1), vy + r*sinf(a1), vz - h, u1, 1,
                 vx + r*cosf(a2), vy + r*sinf(a2), vz - h, u2, 1,
                 vx + r*cosf(a2), vy + r*sinf(a2), vz + h, u2, 0,
                 vx + r*cosf(a1), vy + r*sinf(a1), vz + h, u1, 0,
                 light, light, light, 1.0f, -1); // -1 is magic for sky
    }
}

void HW_BuildWorldGeometry(void)
{
    vertex_count = 0;
    cmd_count = 0;
    
    HW_BuildSkyGeometry();
    HW_BuildPlaneGeometry();
    
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
            if (!spotvis[x][y]) continue;
            
            int tile = tilemap[x][y];
            if (tile == 0) continue;
            
            // Skip doors for now
            if (tile & 0x8000) continue; 
            
            int lump = tile & 0x3ff;
            if (tile & 0x1000) {
                lump = animwalls[lump].texture;
            }
            
            float fx = (float)x;
            float fy = (float)y;
            float fz = 0.0f;
            float s = 1.0f; 
            
            float light = 1.0f; 
            
            // East Face (x + 1)
            if (x < 63 && tilemap[x+1][y] == 0) {
                EmitQuad(fx+s, fy+s, fz,   1, 1,
                         fx+s, fy,   fz,   0, 1,
                         fx+s, fy,   fz+s, 0, 0,
                         fx+s, fy+s, fz+s, 1, 0,
                         light, light, light, 1.0f, lump);
            }
            // West Face (x - 1)
            if (x > 0 && tilemap[x-1][y] == 0) {
                EmitQuad(fx, fy,   fz,   1, 1,
                         fx, fy+s, fz,   0, 1,
                         fx, fy+s, fz+s, 0, 0,
                         fx, fy,   fz+s, 1, 0,
                         light, light, light, 1.0f, lump);
            }
            // North Face (y - 1)
            if (y > 0 && tilemap[x][y-1] == 0) {
                EmitQuad(fx+s, fy, fz,   1, 1,
                         fx,   fy, fz,   0, 1,
                         fx,   fy, fz+s, 0, 0,
                         fx+s, fy, fz+s, 1, 0,
                         light, light, light, 1.0f, lump);
            }
            // South Face (y + 1)
            if (y < 63 && tilemap[x][y+1] == 0) {
                EmitQuad(fx,   fy+s, fz,   1, 1,
                         fx+s, fy+s, fz,   0, 1,
                         fx+s, fy+s, fz+s, 0, 0,
                         fx,   fy+s, fz+s, 1, 0,
                         light, light, light, 1.0f, lump);
            }
        }
    }
}

hw_vertex_t* HW_GetWallVertices(int* out_count)
{
    if (out_count) *out_count = vertex_count;
    return wall_vertices;
}

hw_draw_cmd_t* HW_GetWallDrawCmds(int* out_count)
{
    if (out_count) *out_count = cmd_count;
    return draw_cmds;
}

// Intercepts DrawWalls
void HW_DrawWorld(void)
{
    // Walls and planes are static-ish
    HW_BuildWorldGeometry();
}

// Intercepts DrawScaleds
void HW_DrawSprites(int numvisible)
{
    HW_BuildSpriteGeometry(numvisible);
}
