#ifndef HINT_CAST_H
#define HINT_CAST_H

#pragma once

#include <QDialog>

namespace Ui { class hint_cast; }

class hint_cast : public QDialog {
    Q_OBJECT

public:
    explicit hint_cast(QWidget * parent = nullptr);
    ~hint_cast();

private:
    Ui::hint_cast * ui;
};

#endif // HINT_CAST_H
