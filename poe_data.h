#ifndef POE_DATA_H
#define POE_DATA_H

#pragma once

#include "valclip.h"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) < (b)) ? (b) : (a))

/* local shortcuts */
#define I int
#define F double

static constexpr I gem_lvl_max  = 40;

static constexpr F game_tick    = 0.033;

typedef struct _wintertide_brand {
    /* seconds */
    valclip<F>  activation_interval;
    /* percent, "more/less" stat part */
    valclip<F>  more_damage_per_stage;
    valclip<I>  max_stages;
    /* seconds */
    valclip<F>  brand_duration;
    valclip<I>  brands_per_enemy;
    valclip<I>  labyrinth_extra_max_stages;
    /* percent for base skill damage */
    valclip<F>  labyrinth_damage_inc;
    F           damage[gem_lvl_max];
} _wintertide_brand;

typedef struct _brandsurge {
    /* percent */
    valclip<F>  faster_activate;
    /* percent */
    valclip<F>  faster_activate_step;
    /* seconds */
    valclip<F>  faster_activate_interval;
    /* percent */
    valclip<F>  faster_expire;
    /* percent */
    valclip<F>  faster_expire_step;
    /* seconds */
    valclip<F>  faster_expire_interval;
} _brandsurge;

typedef struct _skill_data {
    struct _wintertide_brand  wintertide_brand;
    struct _brandsurge        brandsurge;
} _skill_data;
extern struct _skill_data skill_data;

typedef struct _hierophant {
    /* percent */
    valclip<F>  more_activation_freq;
    /* percent */
    valclip<F>  attach_duration_expired;
} _hierophant;

typedef struct _runebinder {
    valclip<I>  more_brands_per_enemy;
} _runebinder;

typedef struct _runesmith {
    /* percent */
    valclip<F>  damage_taken_per_brand;
} _runesmith;

typedef struct _skill_tree {
    struct _hierophant  hierophant;
    struct _runebinder  runebinder;
    struct _runesmith   runesmith;
} _skill_tree;
extern struct _skill_tree skill_tree;

typedef struct _combat {
    valclip<I>  gem_level;
    valclip<I>  brands_attached;
    valclip<I>  init_stage;
    valclip<F>  duration_inc;
    valclip<F>  duration_more;
    valclip<F>  cast_inc;
    valclip<F>  cast_more;
    valclip<F>  damage_inc;
    valclip<F>  damage_more;
    valclip<F>  damage_mult;
    valclip<F>  damage_edps_mult;
    valclip<F>  attach_start;
    valclip<F>  overcharge_start;
    bool        hierophant_sign_of_purpose;
    bool        tree_runebinder;
    bool        tree_runesmith;
    bool        labyrinth_any;
    bool        labyrinth_stages;
    bool        labyrinth_damage;
    bool        item_witchhunter;
} _combat;
extern struct _combat combat;

/* shortcuts cleanup */
#undef I
#undef F

#endif // POE_DATA_H
