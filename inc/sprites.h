#ifndef SPRITES_H
#define SPRITES_H

#include <raylib.h>

#define TANK_BASE_PIXELWIDTH 16
#define TANK_BASE_PIXELHEIGHT 16
#define TANK_TRACK_PIXELWIDTH 4
#define TANK_TRACK_PIXELHEIGHT 16
#define TANK_HULL_PIXELWIDTH 10
#define TANK_HULL_PIXELHEIGHT 13
#define TANK_TURRET_PIXELWIDTH 8
#define TANK_TURRET_PIXELHEIGHT 8
#define TANK_BARREL_PIXELWIDTH 2
#define TANK_BARREL_PIXELHEIGHT 5
#define BULLET_PIXELWIDTH 2
#define BULLET_PIXELHEIGHT 2
#define EXPORB_PIXELHEIGHT 6
#define EXPORB_PIXELWIDTH 6
#define SPIDER_PIXELWIDTH 15
#define SPIDER_PIXELHEIGHT 12

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
