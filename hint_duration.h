#ifndef HINT_DURATION_H
#define HINT_DURATION_H

#pragma once

#include <QDialog>

namespace Ui { class hint_duration; }

class hint_duration : public QDialog {
    Q_OBJECT

public:
    explicit hint_duration(QWidget * parent = nullptr);
    ~hint_duration();

private:
    Ui::hint_duration * ui;
};

#endif // HINT_DURATION_H
