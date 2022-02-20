#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <kernel.h>

#include <gsKit.h>
#include <dmaKit.h>

#include <gsToolkit.h>
#include <gsInline.h>

#include <math3d.h>

#include <packet2.h>
#include <packet2_utils.h>


//todo rewrite this to class?
typedef struct
{
    u8 PSM;       ///< Pixel Storage Method (Color Format)
    u8 ClutPSM;   ///< CLUT Pixel Storage Method (Color Format)
    u32 *Clut;    ///< EE CLUT Memory Pointer
    u32 VramClut; ///< GS VRAM CLUT Memory Pointer
} GSCLUT;

typedef struct
{
    float x, y;
    float u, v;
} rm_tx_coord_t;

typedef struct
{
    rm_tx_coord_t ul;
    rm_tx_coord_t br;
    u64 color;
    GSTEXTURE *txt;
} rm_quad_t;


typedef struct 
{
	texel_t* 	stqr;
	color_t* 	rgba;
    vertex_f_t* xyzw;
    VECTOR*     test;
} vData;

struct model
{
	uint32_t facesCount;
    uint16_t* idxList;
    VECTOR* positions;
	VECTOR* texcoords;
	VECTOR* normals;
    VECTOR* colours;
    VECTOR* bounding_box;
    GSTEXTURE* texture;
};

typedef u32 Color;
#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

class graphics
{
  public:    
    void initGraphics();

    void clearScreen(Color color);

    void flipScreen();

    void graphicWaitVblankStart();
private:
    void setVSync(bool vsync_flag);
};

class gs
{
 public:
    void gsKit_clear_screens();
    void setVideoMode(s16 mode, int width, int height, int psm, s16 interlace, s16 field, bool zbuffering, int psmz);
    void InvalidateTexture(GSTEXTURE *txt);

  private:
        GSGLOBAL *getGSGLOBAL();

};

class load
{
    public:
        GSTEXTURE* loadpng(const char *Path, bool delayed);
        GSTEXTURE* loadbmp(const char *Path, bool delayed);
        GSTEXTURE* loadjpeg(const char *Path, bool scale_down, bool delayed);
        model* loadOBJ(const char* path, GSTEXTURE* text);
        GSFONT* loadFont(const char* path);
        void loadFontM();
    private: 
        void unloadFontM();
        void unloadFont(GSFONT* font);
        void UnloadTexture(GSTEXTURE *txt);

};

class draw
{
  public:
    void drawPixel(float x, float y, Color color);
    void drawLine(float x, float y, float x2, float y2, Color color);
    void drawRect(float x, float y, int width, int height, Color color);
    void drawCircle(float x, float y, float radius, u64 color, u8 filled);
    void drawTriangle(float x, float y, float x2, float y2, float x3, float y3, Color color);
    void drawTriangle_gouraud(float x, float y, float x2, float y2, float x3, float y3, Color color, Color color2, Color color3);
    void drawQuad(float x, float y, float x2, float y2, float x3, float y3, float x4, float y4, Color color);
    void drawQuad_gouraud(float x, float y, float x2, float y2, float x3, float y3, float x4, float y4, Color color, Color color2, Color color3, Color color4);
    void fntDrawQuad(rm_quad_t *q);
    void drawOBJ(model* m, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z);

    void draw_bbox(model* m, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z, Color color);

};

class set
{
 public:
    void init3D(float aspect);

    void setCameraPosition(float x, float y, float z);

    void setCameraRotation(float x, float y, float z);

    void setLightQuantity(int quantity);

    void createLight(int lightid, float dir_x, float dir_y, float dir_z, int type, float r, float g, float b);
};



class Font : public gs
{
  public:  
    void printFontText(GSFONT* font, const char* text, float x, float y, float scale, Color color);

    void printFontMText(const char* text, float x, float y, float scale, Color color);
};



#endif