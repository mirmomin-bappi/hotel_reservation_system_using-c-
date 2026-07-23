#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QTimer>

#include "ReservationManager.h"

void populateTable(QTableWidget* table, ReservationManager& mgr) {
    const auto& list = mgr.getAllReservations();

    // Remove existing cell widgets in the delete column to avoid stale pointers
    int oldRows = table->rowCount();
    for (int r = 0; r < oldRows; ++r) {
        QWidget* wdg = table->cellWidget(r, 7);
        if (wdg) {
            table->removeCellWidget(r, 7);
            wdg->deleteLater();
        }
    }

    table->clearContents();
    table->setRowCount((int)list.size());
    for (int r = 0; r < (int)list.size(); ++r) {
        const auto& it = list[r];
        table->setItem(r, 0, new QTableWidgetItem(QString::number(it.id)));
        table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(it.name)));
        table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(it.email)));
        table->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(it.phone)));
        table->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(it.date)));
        table->setItem(r, 5, new QTableWidgetItem(QString::fromStdString(it.roomType)));
        table->setItem(r, 6, new QTableWidgetItem(QString::number(it.price, 'f', 2)));

        // delete button
        QPushButton* del = new QPushButton("Delete");
        table->setCellWidget(r, 7, del);

        // take pointers to avoid capturing references to function parameters
        ReservationManager* mgrPtr = &mgr;
        QTableWidget* tablePtr = table;

        // wire button directly to remove the reservation and refresh the table
        QObject::connect(del, &QPushButton::clicked, [tablePtr, mgrPtr, id = it.id]() {
            QWidget* parent = tablePtr->window();
            QMessageBox::StandardButton res = QMessageBox::question(static_cast<QWidget*>(parent), "Confirm",
                QString("Delete reservation %1?").arg(id));
            if (res == QMessageBox::Yes) {
                mgrPtr->removeReservation(id);
                // Refresh after returning to the event loop to avoid deleting
                // the clicked widget while its slot is still executing.
                QTimer::singleShot(0, [tablePtr, mgrPtr]() { populateTable(tablePtr, *mgrPtr); });
            }
        });
    }
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    ReservationManager mgr;

    QWidget w;
    w.setWindowTitle("Grand Vista Hotel — Reservations");
    QVBoxLayout* mainLayout = new QVBoxLayout(&w);

    QTableWidget* table = new QTableWidget();
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Email" << "Phone" << "Date" << "Type" << "Price" << "");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    populateTable(table, mgr);

    mainLayout->addWidget(table);

    // Form
    QFormLayout* form = new QFormLayout();
    QLineEdit* name = new QLineEdit();
    QLineEdit* email = new QLineEdit();
    QLineEdit* phone = new QLineEdit();
    QLineEdit* date = new QLineEdit();
    QComboBox* type = new QComboBox();
    type->addItem("Economy Class", 1);
    type->addItem("Business Class", 2);

    form->addRow("Name:", name);
    form->addRow("Email:", email);
    form->addRow("Phone:", phone);
    form->addRow("Date:", date);
    form->addRow("Room Type:", type);

    QPushButton* addBtn = new QPushButton("Add Reservation");
    QHBoxLayout* hb = new QHBoxLayout();
    hb->addLayout(form);
    hb->addWidget(addBtn);
    mainLayout->addLayout(hb);

    // Connect add
    QObject::connect(addBtn, &QPushButton::clicked, [&]() {
        std::string sname = name->text().toStdString();
        if (sname.empty()) { QMessageBox::warning(&w, "Validation", "Name is required"); return; }
        std::string semail = email->text().toStdString();
        std::string sphone = phone->text().toStdString();
        std::string sdate = date->text().toStdString();
        int choice = type->currentData().toInt();
        mgr.addReservation(sname, semail, sphone, sdate, choice);
        populateTable(table, mgr);
        name->clear(); email->clear(); phone->clear(); date->clear();
    });

    // Delete handled by individual button handlers wired in populateTable

    w.resize(900, 600);
    w.show();

    return app.exec();
}
