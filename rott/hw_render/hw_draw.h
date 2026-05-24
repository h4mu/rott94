#ifndef HW_DRAW_H
#define HW_DRAW_H

#include <stdint.h>

typedef struct {
    float x, y, z;
    float u, v;
    float r, g, b, a;
} hw_vertex_t;

typedef struct {
    int texture_lump;
    int start_vertex;
    int vertex_count;
} hw_draw_cmd_t;

void HW_InitDraw(void);
void HW_DrawWorld(void);
void HW_DrawSprites(int numvisible);

// For the main render loop to access generated geometry
hw_vertex_t* HW_GetWallVertices(int* out_count);
hw_draw_cmd_t* HW_GetWallDrawCmds(int* out_count);
void HW_BuildWorldGeometry(void);

#endif
