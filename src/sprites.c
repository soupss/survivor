#include "sprites.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

Sprites *sprites_create() {
    Sprites *s = malloc(sizeof(Sprites));
    util_check_alloc(s);
    Image spritesheet = LoadImage("resources/spritesheet.png");
    Rectangle tank_track_r = {5, 30, TANK_TRACK_PIXELWIDTH, TANK_TRACK_PIXELHEIGHT};
    Rectangle tank_hull_r = {14, 29, TANK_HULL_PIXELWIDTH, TANK_HULL_PIXELHEIGHT};
    Rectangle tank_turret_r = {3, 51, TANK_TURRET_PIXELWIDTH, TANK_TURRET_PIXELHEIGHT};
    Rectangle weapon_machinegun_r = {20, 53, WEAPON_MACHINEGUN_PIXELWIDTH, WEAPON_MACHINEGUN_PIXELHEIGHT};
    Rectangle weapon_gatling_r = {27, 53, WEAPON_GATLING_PIXELWIDTH, WEAPON_GATLING_PIXELHEIGHT};
    Rectangle bullet_r = {23, 68, BULLET_PIXELWIDTH, BULLET_PIXELHEIGHT};
    Rectangle exporb_r = {8, 68, EXPORB_PIXELWIDTH, EXPORB_PIXELHEIGHT};
    Rectangle spider_r = {120, 22, SPIDER_PIXELWIDTH, SPIDER_PIXELHEIGHT};
    Image tank_track_i = ImageFromImage(spritesheet, tank_track_r);
    Image tank_hull_i = ImageFromImage(spritesheet, tank_hull_r);
    Image tank_turret_i = ImageFromImage(spritesheet, tank_turret_r);
    Image weapon_machinegun_i = ImageFromImage(spritesheet, weapon_machinegun_r);
    Image weapon_gatling_i = ImageFromImage(spritesheet, weapon_gatling_r);
    Image bullet_i = ImageFromImage(spritesheet, bullet_r);
    Image exporb_i = ImageFromImage(spritesheet, exporb_r);
    Image spider_i = ImageFromImage(spritesheet, spider_r);
    s->tank_track = LoadTextureFromImage(tank_track_i);
    s->tank_hull = LoadTextureFromImage(tank_hull_i);
    s->tank_turret = LoadTextureFromImage(tank_turret_i);
    s->weapon_machinegun = LoadTextureFromImage(weapon_machinegun_i);
    s->weapon_gatling = LoadTextureFromImage(weapon_gatling_i);
    s->bullet = LoadTextureFromImage(bullet_i);
    s->exporb = LoadTextureFromImage(exporb_i);
    s->spider = LoadTextureFromImage(spider_i);
    UnloadImage(tank_track_i);
    UnloadImage(tank_hull_i);
    UnloadImage(tank_turret_i);
    UnloadImage(weapon_machinegun_i);
    UnloadImage(bullet_i);
    UnloadImage(exporb_i);
    UnloadImage(spider_i);
    return s;
}

void sprites_destroy(Sprites *s) {
    UnloadTexture(s->tank_track);
    UnloadTexture(s->tank_hull);
    UnloadTexture(s->tank_turret);
    UnloadTexture(s->weapon_machinegun);
    UnloadTexture(s->weapon_gatling);
    UnloadTexture(s->bullet);
    UnloadTexture(s->exporb);
    UnloadTexture(s->spider);
    free(s);
}
