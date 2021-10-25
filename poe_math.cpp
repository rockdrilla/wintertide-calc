#include "poe_math.h"

#include <cstring>

void dispose_result(combat_result * r) {
    for (auto i = 0; i < r->log.count(); i++) {
        r->log[i].notes.clear();
    }
    r->log.clear();
}

/* local shortcuts */
#define I int
#define F double

static F tick_floor(F value) {
    return game_tick * floor(value / game_tick);
}

static F tick_ceil(F value) {
    return game_tick * ceil(value / game_tick);
}

static F brand_damage_base(void) {
    F damage_inc = combat.damage_inc;
    if (combat.labyrinth_any && combat.labyrinth_damage) {
        damage_inc += (skill_data.wintertide_brand.labyrinth_damage_inc / 100.0);
    }

    F damage_edps_runesmith = 1.0;
    if (combat.tree_runesmith) {
        damage_edps_runesmith += ((skill_tree.runesmith.damage_taken_per_brand * (double) combat.brands_attached) / 100.0);
        damage_inc *= damage_edps_runesmith;
    }

    return skill_data.wintertide_brand.damage[combat.gem_level - 1]
           * damage_inc
           * combat.damage_more
           * combat.damage_mult
           * combat.damage_edps_mult
    ;
}

/*
  TODO:
  take in account detach duration to avoid (overcharged) brand expiration
  before attaching to enemy
*/
void calc_result(combat_result * r) {
    struct {
        struct {
            F init, current, part, overall;
        } damage;
        struct {
            I init, current, max;
            F duration_init, duration_current;
            struct {
                F time;
                I tick;
            } next;
        } stage;
        struct {
            F init, current;
        } lifetime;
        struct {
            F attach, overcharge, min, max, now, step, next, remain;
        } time;
        struct {
            I attach, overcharge, min, max, now, step, next, remain;
        } tick;
        struct {
            struct {
                I stage;
                struct {
                    F time;
                    I tick;
                } next;
            } expire, speedup;
        } overcharge;

        F lifecycle; // percent

        bool hierophant, brandsurge; // shortcuts
    } x;
    // just to be sure
    std::memset(&x, 0, sizeof(x));

    x.stage.current =
    x.stage.init = combat.init_stage;

    x.stage.max = skill_data.wintertide_brand.max_stages;
    if (combat.labyrinth_any && combat.labyrinth_stages) {
        x.stage.max += skill_data.wintertide_brand.labyrinth_extra_max_stages;
    }

    x.damage.init = brand_damage_base();

    x.lifetime.init = combat.duration_more * combat.duration_inc
                    * skill_data.wintertide_brand.brand_duration;

    x.stage.duration_init = skill_data.wintertide_brand.activation_interval
                          / (combat.cast_inc * combat.cast_more);

    x.time.attach     = combat.attach_start;
    x.time.overcharge = combat.overcharge_start;
    x.time.min        = fmin(x.time.attach, x.time.overcharge);
    x.time.max        = combat.attach_start + x.lifetime.init;
    x.time.now        = x.time.min;

    x.tick.attach     = (I) ceil(x.time.attach / game_tick);
    x.tick.overcharge = (I) ceil(x.time.overcharge / game_tick);
    x.tick.min        = (I) ceil(x.time.min / game_tick);
    x.tick.max        = (I) ceil(x.time.max / game_tick);
    x.tick.now        = x.tick.min;

    x.hierophant = combat.hierophant_sign_of_purpose;
    x.brandsurge = combat.item_witchhunter && (x.time.overcharge < x.time.max);

    auto F_damage = [&x] (void) -> F {
        return x.damage.init * (
            1.0 + ((skill_data.wintertide_brand.more_damage_per_stage * (F) x.stage.current) / 100.0)
        );
    };

    auto F_lifetime = [&x] (void) -> F {
        if (!x.brandsurge) {
            return x.lifetime.init;
        }

        if (x.overcharge.expire.stage == 0) {
            return x.lifetime.init;
        }

        F a = 1.0
            + (skill_data.brandsurge.faster_expire / 100.0)
            + ((skill_data.brandsurge.faster_expire_step * (F) (x.overcharge.expire.stage - 1)) / 100.0)
        ;
        return x.lifetime.init / a;
    };
    x.lifetime.current = F_lifetime();

    auto F_stage_duration = [&x] (void) -> F {
        bool need_adjust = false;
        F a = 1.0;

        if (x.overcharge.speedup.stage > 0) {
            need_adjust = true;
            a += (skill_data.brandsurge.faster_activate / 100.0)
               + ((skill_data.brandsurge.faster_activate_step * (F) (x.overcharge.speedup.stage - 1)) / 100.0)
            ;
        }
        if (x.hierophant) {
            F d = x.lifecycle - skill_tree.hierophant.attach_duration_expired;
            if (d > -1.0) {
                need_adjust = true;
                a *= (1.0
                    + (skill_tree.hierophant.more_activation_freq / 100.0)
                );
            }
        }

        return need_adjust ? (x.stage.duration_init / a) : x.stage.duration_init;
    };
    x.stage.duration_current = F_stage_duration();

    x.stage.next.time = x.time.attach;
    x.stage.next.tick = x.tick.attach;

    x.overcharge.expire.next.time =
    x.overcharge.speedup.next.time = x.brandsurge ? x.time.overcharge : -1;

    x.overcharge.expire.next.tick =
    x.overcharge.speedup.next.tick = x.brandsurge ? x.tick.overcharge : -1;

    log_entry L;

    if (x.tick.min > 0) {
        std::memset(&L, 0, sizeof(L));
        L.notes = QStringList();

        L.notes.append("idle");

        L.time.duration = x.time.min;
        L.server_time.duration = tick_ceil(L.time.duration);
        L.skew.duration = L.server_time.duration - L.time.duration;

        r->log.append(L);
    }

    r->dps.init = F_damage();

    while (x.tick.now < x.tick.max) {
        // early prepare log entry
        std::memset(&L, 0, sizeof(L));
        L.notes = QStringList();

        x.time.step = x.time.max - x.time.now;
        x.tick.step = x.tick.max - x.tick.now;

        if (x.tick.now == x.tick.attach) {
            L.notes.append("attach to enemy");
        }

        if (x.tick.now == x.overcharge.expire.next.tick) {
            L.notes.append("reduce brand duration");

            x.overcharge.expire.stage++;
            x.lifetime.current = F_lifetime();

            x.overcharge.expire.next.time += skill_data.brandsurge.faster_expire_interval;
            x.overcharge.expire.next.tick = (I) ceil(x.overcharge.expire.next.time / game_tick);
        }

        if (x.tick.now == x.overcharge.speedup.next.tick) {
            L.notes.append("increase brand frequency");

            struct {
                struct {
                    F before, after;
                } duration, lifecycle;
            } stage;

            stage.duration.before = F_stage_duration();
            x.overcharge.speedup.stage++;
            stage.duration.after = F_stage_duration();

            if ((x.tick.now >= x.tick.attach) && (x.tick.now != x.stage.next.tick)) {
                stage.lifecycle.before = (x.time.now + stage.duration.before - x.stage.next.time)
                                    / stage.duration.before;
                stage.lifecycle.after = 1.0 - stage.lifecycle.before;

                x.stage.next.time = x.time.now + stage.duration.after * stage.lifecycle.after;
                x.stage.next.tick = (I) ceil(x.stage.next.time / game_tick);

                x.time.step = fmin(x.time.step, x.stage.next.time - x.time.now);
                x.tick.step = MIN(x.tick.step, x.stage.next.tick - x.tick.now);
            }

            x.stage.duration_current = stage.duration.after;

            x.overcharge.speedup.next.time += skill_data.brandsurge.faster_activate_interval;
            x.overcharge.speedup.next.tick = (I) ceil(x.overcharge.speedup.next.time / game_tick);
        }

        if (x.brandsurge) {
            x.time.step = fmin(x.time.step, x.overcharge.expire.next.time - x.time.now);
            x.tick.step = MIN(x.tick.step, x.overcharge.expire.next.tick - x.tick.now);

            x.time.step = fmin(x.time.step, x.overcharge.speedup.next.time - x.time.now);
            x.tick.step = MIN(x.tick.step, x.overcharge.speedup.next.tick - x.tick.now);
        }

        if (x.tick.now == x.stage.next.tick) {
            if (x.tick.now != x.tick.attach) {
                x.stage.current++;
                x.stage.current = MIN(x.stage.current, x.stage.max);
            }

            x.damage.current = F_damage();
            x.stage.duration_current = F_stage_duration();

            x.stage.next.time += x.stage.duration_current;
            x.stage.next.tick = (I) ceil(x.stage.next.time / game_tick);

            x.time.step = fmin(x.time.step, x.stage.next.time - x.time.now);
            x.tick.step = MIN(x.tick.step, x.stage.next.tick - x.tick.now);
        }

        {
            x.time.remain = (x.lifetime.current * (100.0 - x.lifecycle)) / 100.0;
            x.tick.remain = (I) ceil(x.time.step / game_tick);

            x.time.step = fmin(x.time.step, x.time.remain);
            x.tick.step = MIN(x.tick.step, x.tick.remain);
        }

        x.damage.part     = x.damage.current * x.time.step;
        x.damage.overall += x.damage.part;

        L.time.start        = x.time.now;
        L.server_time.start = tick_ceil(L.time.start);
        L.skew.start        = L.server_time.start - L.time.start;

        L.time.duration        = x.time.step;
        L.server_time.duration = tick_ceil(x.time.step);
        L.skew.duration        = L.server_time.duration - L.time.duration;

        L.stage     = x.stage.current;
        L.frequency = 1.0 / x.stage.duration_current;
        L.duration  = x.lifetime.current;
        L.lifecycle = x.lifecycle;

        L.damage.current = x.damage.current;
        L.damage.part    = x.damage.part;
        L.damage.overall = x.damage.overall;

        r->log.append(L);

        x.time.now += x.time.step;
        x.tick.now += x.tick.step;

        if (x.tick.now >= x.tick.attach) {
            x.lifecycle += ((100.0 * x.time.step) / x.lifetime.current);
            if (x.lifecycle >= 100.0) { break; }
        }
    }

    r->duration.attach = x.time.now - x.time.attach;
    r->duration.total = x.time.now;

    r->damage = x.damage.overall;
    r->dps.overall = x.damage.overall / x.time.now;
}
