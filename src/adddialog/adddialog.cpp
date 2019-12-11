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

#include "adddialog.h"
#include "ui_adddialog.h"

#include <QClipboard>
#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include <QDebug>
#include <KNotifications/KNotification>

AddDialog::AddDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddDialog) {
    ui->setupUi(this);
}

AddDialog::~AddDialog() {
    delete ui;
}

bool AddDialog::init(AddDialogData *data) {
    ui->nameLineEdit->setText(data->name);
    return true;
}

void AddDialog::on_buttonBox_accepted() {
    const QString entryName = ui->nameLineEdit->text();
    wallet->setFolder("Passwords");
    if (wallet->writePassword(entryName,ui->passwordPlainTextEdit->text()) == 0) {
        KNotification::event(KNotification::Notification, "KWallet", entryName + " added to KWallet.", "kwalletmanager");
    } else { // Otherwise notify user of error saving
        KNotification::event(KNotification::Error, "KWallet", entryName + " could not be added.", "kwalletmanager");
    }
    close();
}
