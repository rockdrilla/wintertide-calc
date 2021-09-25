#ifndef POE_MATH_H
#define POE_MATH_H

#pragma once

#include <QThread>
#include <QList>
#include <QStringList>

#include "poe_data.h"

/* local shortcuts */
#define I int
#define F double

typedef struct log_entry {
    struct {
        F start, duration;
    } time;
    struct {
        F start, duration;
    } server_time;
    struct {
        F start, duration;
    } skew;
    struct {
        F current, part, overall;
    } damage;

    F lifecycle;
    I stage;
    F frequency, duration;

    QStringList notes;
} log_entry;

typedef struct combat_result {
    QThread * _owner;

    struct {
        F init, overall;
    } dps;
    struct {
        F attach, total;
    } duration;

    F damage;

    QList<log_entry> log;
} combat_result;

extern void calc_result(combat_result * result);
extern void dispose_result(combat_result * result);

/* shortcuts cleanup */
#undef I
#undef F

#endif
