#include "hint_damage.h"
#include "ui_hint_damage.h"

hint_damage::hint_damage(QWidget * parent) : QDialog(parent), ui(new Ui::hint_damage) {
    ui->setupUi(this);

    setFixedSize(width(), height());
}

hint_damage::~hint_damage() {
    delete ui;
}
