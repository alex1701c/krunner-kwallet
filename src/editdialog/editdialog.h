/*
    KWallet Runner
    Copyright (C) 2016  James Augustus Zuccon <zuccon@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <KWallet/KWallet>
#include <utility>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <KNotifications/KNotification>

using KWallet::Wallet;

namespace Ui {
    class AddDialog;
}

class AddDialogData : public QObject {
Q_OBJECT
public:
    const QString name;
    QString value;

public:
    AddDialogData(QString name = "", QObject *parent = nullptr) : QObject(parent), name(std::move(name)) {}
};


class EditDialog : public QDialog {
Q_OBJECT


public:
    explicit EditDialog(QWidget *parent = nullptr);

    ~EditDialog() override;

    bool init(AddDialogData *data);

private slots:

    void save_entry();

private:
    Ui::AddDialog *ui;
    Wallet *wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Synchronous);
    QString initialName;

    static void displayUpdateNotification(const QString &msg, const KNotification::StandardEvent type);
};

#endif // ADDDIALOG_H
