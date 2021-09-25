#include <QObject>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hint_duration.h"
#include "hint_cast.h"
#include "hint_damage.h"
#include "skill_dmg_table.h"

#include "poe_data.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto f_brands_per_enemy = [s = this->ui->spin_brands_attached] (void) {
        int x = skill_data.wintertide_brand.brands_per_enemy;
        x += (combat.tree_runebinder)
           ? skill_tree.runebinder.more_brands_per_enemy
           : 0;
        int y = s->value();
        s->setValue(MIN(x, y));
        s->setMaximum(x);
    };

    auto f_max_stages = [s = this->ui->spin_init_stage] (void) {
        int x = skill_data.wintertide_brand.max_stages;
        x += (combat.labyrinth_any && combat.labyrinth_stages)
           ? skill_data.wintertide_brand.labyrinth_extra_max_stages
           : 0;
        int y = s->value();
        s->setValue(MIN(x, y));
        s->setMaximum(x);
    };

    // tab 1

    bind_spin_data(ui->spin_gem_level,       combat.gem_level);
    bind_spin_data(ui->spin_brands_attached, combat.brands_attached);
    bind_spin_data(ui->spin_init_stage,      combat.init_stage);
    bind_spin_data(ui->spin_attach_start,    combat.attach_start);
    bind_spin_data(ui->spin_duration_inc,    combat.duration_inc);
    bind_spin_data(ui->spin_duration_more,   combat.duration_more);
    bind_spin_data(ui->spin_cast_inc,        combat.cast_inc);
    bind_spin_data(ui->spin_cast_more,       combat.cast_more);
    bind_spin_data(ui->spin_dmg_total_inc,   combat.damage_inc);
    bind_spin_data(ui->spin_dmg_total_more,  combat.damage_more);
    bind_spin_data(ui->spin_dmg_total_mult,  combat.damage_mult);
    bind_spin_data(ui->spin_dmg_edps_mult,   combat.damage_edps_mult);

    // tab 2

    bind_check_data(ui->box_hierophant, combat.hierophant_sign_of_purpose);
    bind_check_data(ui->box_runebinder, combat.tree_runebinder, f_brands_per_enemy);
    bind_check_data(ui->box_runesmith,  combat.tree_runesmith);

    bind_spin_data(ui->spin_hierophant_more_activation_freq,    skill_tree.hierophant.more_activation_freq);
    bind_spin_data(ui->spin_hierophant_attach_duration_expired, skill_tree.hierophant.attach_duration_expired);
    bind_spin_data(ui->spin_more_brands_per_enemy,              skill_tree.runebinder.more_brands_per_enemy, f_brands_per_enemy);
    bind_spin_data(ui->spin_dmg_taken_per_brand,                skill_tree.runesmith.damage_taken_per_brand);

    // tab 3

    bind_check_data(ui->box_witchhunter, combat.item_witchhunter);

    bind_spin_data(ui->spin_overcharge_start,                    combat.overcharge_start);
    bind_spin_data(ui->spin_brandsurge_faster_activate,          skill_data.brandsurge.faster_activate);
    bind_spin_data(ui->spin_brandsurge_faster_activate_step,     skill_data.brandsurge.faster_activate_step);
    bind_spin_data(ui->spin_brandsurge_faster_activate_interval, skill_data.brandsurge.faster_activate_interval);
    bind_spin_data(ui->spin_brandsurge_faster_expire,            skill_data.brandsurge.faster_expire);
    bind_spin_data(ui->spin_brandsurge_faster_expire_step,       skill_data.brandsurge.faster_expire_step);
    bind_spin_data(ui->spin_brandsurge_faster_expire_interval,   skill_data.brandsurge.faster_expire_interval);

    // tab 4

    bind_check_data(ui->box_labyrinth,            combat.labyrinth_any, f_max_stages);
    bind_check_data(ui->radio_enchant_max_stages, combat.labyrinth_stages, f_max_stages);
    bind_check_data(ui->radio_enchant_more_dmg,   combat.labyrinth_damage);

    bind_spin_data(ui->spin_activation_interval,      skill_data.wintertide_brand.activation_interval);
    bind_spin_data(ui->spin_more_dmg_per_stage,       skill_data.wintertide_brand.more_damage_per_stage);
    bind_spin_data(ui->spin_max_stages,               skill_data.wintertide_brand.max_stages, f_max_stages);
    bind_spin_data(ui->spin_brand_duration,           skill_data.wintertide_brand.brand_duration);
    bind_spin_data(ui->spin_brands_per_enemy,         skill_data.wintertide_brand.brands_per_enemy, f_brands_per_enemy);
    bind_spin_data(ui->spin_enchant_extra_max_stages, skill_data.wintertide_brand.labyrinth_extra_max_stages, f_max_stages);
    bind_spin_data(ui->spin_enchant_dmg_inc,          skill_data.wintertide_brand.labyrinth_damage_inc);

    // post-init ui hints

    f_brands_per_enemy();
    f_max_stages();
    ui_post_change();
}

MainWindow::~MainWindow() {
    if (_calc_thread != nullptr) {
        auto x = _calc_thread;
        _calc_thread = nullptr;

        x->terminate();
        x->wait();
        x->force_cleanup();
        x->deleteLater();
    }
    delete ui;
}

void MainWindow::on_button_duration_hint_clicked() {
    auto d = new hint_duration(this);
    d->exec(); d->deleteLater();
}

void MainWindow::on_button_spd_hint_clicked() {
    auto d = new hint_cast(this);
    d->exec(); d->deleteLater();
}

void MainWindow::on_button_dmg_hint_clicked() {
    auto d = new hint_damage(this);
    d->exec(); d->deleteLater();
}

void MainWindow::on_button_open_skill_dmg_table_clicked() {
    auto d = new skill_dmg_table(this);
    d->exec(); d->deleteLater();
}

void MainWindow::ui_post_change() {
    if (_calc_thread != nullptr) {
        auto x = _calc_thread;
        _calc_thread = nullptr;

        if (x->isRunning()) {
            x->terminate();
            x->wait();
        }

        x->force_cleanup();
        x->deleteLater();
    }

    _calc_thread = new calc_thread();
    connect(_calc_thread, &calc_thread::data_ready, this, &MainWindow::handle_data);
    _calc_thread->start();
}

static QTableWidgetItem * table_item(const int & value) {
    auto r = new QTableWidgetItem(QString::number(value));
    r->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    return r;
}

static QTableWidgetItem * table_item(const double & value, int precision = 2) {
    auto r = new QTableWidgetItem(QString::number(value, 'f', precision));
    r->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    return r;
}

static QTableWidgetItem * table_item(const QString & value) {
    auto r = new QTableWidgetItem(value);
    r->setTextAlignment(Qt::AlignVCenter);
    return r;
}

template <typename T1, typename ... Tx>
static QTableWidgetItem * table_item(const QString & format, T1 arg1, Tx ... args) {
    auto r = new QTableWidgetItem(QString(format).arg(arg1, args...));
    r->setTextAlignment(Qt::AlignVCenter);
    return r;
}

void MainWindow::handle_data(combat_result * r) {
    if (r == nullptr) { return; }

    // populate common values
    {
        ui->spin_result_total_damage->setValue(r->damage);
        ui->spin_result_init_dps->setValue(r->dps.init);
        ui->spin_result_overall_dps->setValue(r->dps.overall);
        ui->spin_result_duration->setValue(r->duration.attach);
    }

    // populate log
    {
        ui->table_damage_log->setRowCount(0);

        auto c = r->log.count();
        ui->table_damage_log->setRowCount(c);
        for (int i = 0; i < c; i++) {
            ui->table_damage_log->setItem(i,  0, table_item(r->log[i].time.start));
            ui->table_damage_log->setItem(i,  1, table_item(r->log[i].server_time.start, 3));
            ui->table_damage_log->setItem(i,  2, table_item(r->log[i].skew.start, 3));

            ui->table_damage_log->setItem(i,  3, table_item(r->log[i].time.duration));
            ui->table_damage_log->setItem(i,  4, table_item(r->log[i].server_time.duration, 3));
            ui->table_damage_log->setItem(i,  5, table_item(r->log[i].skew.duration, 3));

            ui->table_damage_log->setItem(i,  6, table_item(r->log[i].damage.current));
            ui->table_damage_log->setItem(i,  7, table_item(r->log[i].damage.part));
            ui->table_damage_log->setItem(i,  8, table_item(r->log[i].damage.overall, 1));

            ui->table_damage_log->setItem(i,  9, table_item(r->log[i].lifecycle));
            ui->table_damage_log->setItem(i, 10, table_item(r->log[i].duration));
            ui->table_damage_log->setItem(i, 11, table_item(r->log[i].stage));
            ui->table_damage_log->setItem(i, 12, table_item(r->log[i].frequency));

            ui->table_damage_log->setItem(i, 13, table_item(r->log[i].notes.join("\n")));
        }

        ui->table_damage_log->resizeColumnsToContents();
        ui->table_damage_log->resizeRowsToContents();
    }

    // cleanup
    {
        _calc_thread = nullptr;
        auto t = r->_owner;
        dispose_result(r);
        t->deleteLater();
    }
}
