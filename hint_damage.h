#ifndef HINT_DAMAGE_H
#define HINT_DAMAGE_H

#pragma once

#include <QDialog>

namespace Ui { class hint_damage; }

class hint_damage : public QDialog {
    Q_OBJECT

public:
    explicit hint_damage(QWidget * parent = nullptr);
    ~hint_damage();

private:
    Ui::hint_damage * ui;
};

#endif // HINT_DAMAGE_H
