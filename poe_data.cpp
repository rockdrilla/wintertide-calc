#include "poe_data.h"

/* local shortcuts */
#define I int
#define F double

/* numbers below are grabbed from wiki and seemed to be actual as of Path of Exile 3.15 */

struct _skill_data skill_data = {
    .wintertide_brand = {
    /*   parameter                                def     min         max */
        .activation_interval        = valclip<F>(  0.25 , game_tick ,   9 , 2 ),
        .more_damage_per_stage      = valclip<F>( 20    , 1         , 900 , 1 ),
        .max_stages                 = valclip<I>( 20    , 1         ,  90 ),
        .brand_duration             = valclip<F>(  2    , game_tick ,  60 , 2 ),
        .brands_per_enemy           = valclip<I>(  2    , 1         ,  90 ),
        .labyrinth_extra_max_stages = valclip<I>(  4    , 1         ,  90 ),
        .labyrinth_damage_inc       = valclip<F>( 40    , 1         , 900 , 1 ),
        /* skill gem damage for levels 1-40 */
        .damage = {
            12.5,
            15.2,
            19.3,
            24  ,
            29.4,
            35.6,
            42.7,
            48.7,
            55.2,
            62.5,
            70.5,
            79.3,
            89  ,
            99.7,
            111.3,
            124.1,
            133.3,
            143.1,
            153.5,
            164.6,
            176.3,
            188.8,
            202  ,
            216  ,
            230.9,
            246.7,
            263.4,
            281.2,
            300  ,
            319.9,
            330.3,
            341.1,
            352.1,
            363.4,
            375.1,
            387.1,
            399.5,
            412.2,
            425.4,
            438.9,
        },
    },
    .brandsurge = {
    /*   parameter                              def     min         max */
        .faster_activate          = valclip<F>(  35   , 1         , 900 , 1 ),
        .faster_activate_step     = valclip<F>(  35   , 1         , 900 , 1 ),
        .faster_activate_interval = valclip<F>(   0.5 , game_tick ,  10 , 2 ),
        .faster_expire            = valclip<F>( 150   , 1         , 900 , 1 ),
        .faster_expire_step       = valclip<F>( 150   , 1         , 900 , 1 ),
        .faster_expire_interval   = valclip<F>(   0.5 , game_tick ,  10 , 2 ),
    },
};

struct _skill_tree skill_tree = {
    .hierophant = {
    /*   parameter                             def   min   max */
        .more_activation_freq    = valclip<F>( 100 , -99 , 900 , 1 ),
        .attach_duration_expired = valclip<F>(  75 ,   1 ,  99 , 1 ),
    },
    .runebinder = {
    /*   parameter                             def   min   max */
        .more_brands_per_enemy   = valclip<I>(   1 ,   1 ,   9 ),
    },
    .runesmith = {
    /*   parameter                             def   min   max */
        .damage_taken_per_brand  = valclip<F>(   5 , 0.1 ,  90 , 1 ),
    },
};

struct _combat combat = {
/*   parameter                      def   min    max */
    .gem_level        = valclip<I>(  1  , 1    , gem_lvl_max ),
    .brands_attached  = valclip<I>(  1  , 1    ,  9 ),
    .init_stage       = valclip<I>(  0  , 0    , 90 ),
    .duration_inc     = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .duration_more    = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .cast_inc         = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .cast_more        = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .damage_inc       = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .damage_more      = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .damage_mult      = valclip<F>(  1  , 0.01 , 90          , 2 ),
    .damage_edps_mult = valclip<F>(  1  , 0.01 , 90          , 3 ),
    .attach_start     = valclip<F>(  0  , 0    , 20          , 2 ),
    .overcharge_start = valclip<F>( 10  , 0    , 20          , 2 ),
};
