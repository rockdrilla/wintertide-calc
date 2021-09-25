#ifndef SKILL_DMG_TABLE_H
#define SKILL_DMG_TABLE_H

#pragma once

#include <QDialog>

namespace Ui { class skill_dmg_table; }

class skill_dmg_table : public QDialog {
    Q_OBJECT

public:
    explicit skill_dmg_table(QWidget * parent = nullptr);
    ~skill_dmg_table();

private slots:
    void on_table_data_cellChanged(int row, int column);

private:
    Ui::skill_dmg_table * ui;
};

#endif // SKILL_DMG_TABLE_H
