#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QAbstractSpinBox>
#include <QThread>

#include "valclip.h"
#include "poe_data.h"
#include "poe_math.h"

class calc_thread : public QThread {
    Q_OBJECT

    combat_result result;

    void run(void) override {
        QThread::msleep(50);

        result._owner = this;
        calc_result(&result);

        emit data_ready(&result);
    }

signals:
    void data_ready(combat_result * r);

public:
    void force_cleanup(void) {
        dispose_result(&result);
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

public slots:
    void handle_data(combat_result *);

private slots:
    void on_button_duration_hint_clicked();
    void on_button_spd_hint_clicked();
    void on_button_dmg_hint_clicked();
    void on_button_open_skill_dmg_table_clicked();

private:
    Ui::MainWindow * ui;

    calc_thread * _calc_thread = nullptr;

    void ui_post_change(void);

    template <typename TCheck>
    void bind_check_data(TCheck * check, bool & value) {
        check->setChecked(value);

        QObject::connect(check, &TCheck::toggled, this,
            [this, check, &value] (void) {
                value = check->isChecked();
                this->ui_post_change();
            }
        );
    }

    /*
       TODO: make function more generic (i.e. "xtra" is optional param)
       and get rid of function above
    */
    template <typename TCheck, typename TFunc>
    void bind_check_data(TCheck * check, bool & value, TFunc xtra) {
        check->setChecked(value);

        QObject::connect(check, &TCheck::toggled, this,
            [this, check, &value, xtra] (void) {
                value = check->isChecked();
                xtra();
                this->ui_post_change();
            }
        );
    }

    template <typename TSpin, typename Tscalar>
    void bind_spin_data(TSpin * spin, valclip<Tscalar> & value) {
        spin->setRange(value.min(), value.max());
        spin->setSingleStep(value.step());
        spin->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        spin->setValue(value);

        auto p = "decimals";
        auto v = spin->property(p);
        if (v.isValid()) {
            spin->setProperty(p, QVariant(value.precision()));
        }

        auto s = spin->sizePolicy();
        s.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
        spin->setSizePolicy(s);
        spin->adjustSize();

        QObject::connect(spin, &TSpin::valueChanged, this,
            [this, spin, &value] (void) {
                value = spin->value();
                this->ui_post_change();
            }
        );
    }

    /*
       TODO: make function more generic (i.e. "xtra" is optional param)
       and get rid of function above
    */
    template <typename TSpin, typename Tscalar, typename TFunc>
    void bind_spin_data(TSpin * spin, valclip<Tscalar> & value, TFunc xtra) {
        spin->setRange(value.min(), value.max());
        spin->setSingleStep(value.step());
        spin->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        spin->setValue(value);

        auto p = "decimals";
        auto v = spin->property(p);
        if (v.isValid()) {
            spin->setProperty(p, QVariant(value.precision()));
        }

        auto s = spin->sizePolicy();
        s.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
        spin->setSizePolicy(s);
        spin->adjustSize();

        QObject::connect(spin, &TSpin::valueChanged, this,
            [this, spin, &value, xtra] (void) {
                value = spin->value();
                xtra();
                this->ui_post_change();
            }
        );
    }
};

#endif // MAINWINDOW_H
