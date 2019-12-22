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

#include "editdialog.h"
#include "ui_editdialog.h"

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
#include <QStringBuilder>

// Test dq
EditDialog::EditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddDialog) {
    ui->setupUi(this);
}

EditDialog::~EditDialog() {
    delete ui;
}

bool EditDialog::init(AddDialogData *data) {
    ui->nameLineEdit->setText(data->name);
    return true;
}

void EditDialog::on_buttonBox_accepted() {
    const QString entryName = ui->nameLineEdit->text();
    wallet->setFolder("");
    if (wallet->writePassword(entryName, ui->passwordPlainTextEdit->text()) == 0) {
        KNotification::event(KNotification::Notification,
                             QStringLiteral("KWallet"),
                             entryName % " saved to KWallet.",
                             QStringLiteral("kwalletmanager"));
    } else {
        KNotification::event(KNotification::Error,
                             QStringLiteral("KWallet"),
                             entryName % " could not be saved.",
                             QStringLiteral("kwalletmanager"));
    }
    close();
}
