#include "skill_dmg_table.h"
#include "ui_skill_dmg_table.h"

#include "poe_data.h"

skill_dmg_table::skill_dmg_table(QWidget * parent) : QDialog(parent), ui(new Ui::skill_dmg_table) {
    ui->setupUi(this);

    setFixedSize(width(), height());

    ui->table_data->setRowCount(gem_lvl_max);
    for (int i = 0; i < gem_lvl_max; i++) {
        ui->table_data->setItem(i, 0,
            new QTableWidgetItem(tr("%1").arg(
                skill_data.wintertide_brand.damage[i],
                0, 'f', 1
        )));
    }

    ui->table_data->resizeColumnToContents(0);
}

skill_dmg_table::~skill_dmg_table() {
    delete ui;
}

void skill_dmg_table::on_table_data_cellChanged(int row, int column) {
    constexpr auto r = Qt::DisplayRole;

    auto i = ui->table_data->item(row, column);
    auto v = i->data(r);

    bool ok = false;
    double d = v.toDouble(&ok);
    if (ok && (row < gem_lvl_max)) {
        skill_data.wintertide_brand.damage[row] = d;
    } else {
        i->setData(r, QVariant(
            skill_data.wintertide_brand.damage[row]
        ));
    }
}
