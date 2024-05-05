#ifndef SPRITES_H
#define SPRITES_H

#include <raylib.h>

#define TANK_BASE_PIXELWIDTH 19
#define TANK_BASE_PIXELHEIGHT 20
#define TANK_TRACK_PIXELWIDTH 6
#define TANK_TRACK_PIXELHEIGHT 18
#define TANK_HULL_PIXELWIDTH 9
#define TANK_HULL_PIXELHEIGHT 20
#define TANK_TURRET_PIXELWIDTH 15
#define TANK_TURRET_PIXELHEIGHT 15
#define TANK_BARREL_PIXELWIDTH 5
#define TANK_BARREL_PIXELHEIGHT 12
#define BULLET_PIXELWIDTH 4
#define BULLET_PIXELHEIGHT 4
#define EXPORB_PIXELHEIGHT 8
#define EXPORB_PIXELWIDTH 8
#define SPIDER_PIXELWIDTH 27
#define SPIDER_PIXELHEIGHT 20

typedef struct {
    Texture2D tank_track;
    Texture2D tank_hull;
    Texture2D tank_turret;
    Texture2D tank_barrel;
    Texture2D bullet;
    Texture2D exporb;
    Texture2D spider;
} Sprites;

Sprites *sprites_create();
void sprites_destroy(Sprites *s);

#endif
