#include "hint_duration.h"
#include "ui_hint_duration.h"

hint_duration::hint_duration(QWidget * parent) : QDialog(parent), ui(new Ui::hint_duration) {
    ui->setupUi(this);

    setFixedSize(width(), height());
}

hint_duration::~hint_duration() {
    delete ui;
}
