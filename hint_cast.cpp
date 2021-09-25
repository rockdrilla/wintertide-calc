#include "hint_cast.h"
#include "ui_hint_cast.h"

hint_cast::hint_cast(QWidget * parent) : QDialog(parent), ui(new Ui::hint_cast) {
    ui->setupUi(this);

    setFixedSize(width(), height());
}

hint_cast::~hint_cast() {
    delete ui;
}
