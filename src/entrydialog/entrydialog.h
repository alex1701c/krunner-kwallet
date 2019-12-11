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

#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include <KWallet/KWallet>
#include <utility>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

using KWallet::Wallet;

namespace Ui {
    class EntryDialog;
}

class EntryDialogData : public QObject {
Q_OBJECT
public:
    const QString folder;
    const QString entry;

public:
    EntryDialogData(QString folder, QString entry, QObject *parent = nullptr) : QObject(parent), folder(std::move(folder)),
                                                                                entry(std::move(entry)) {}
};


class EntryDialog : public QDialog {
Q_OBJECT


public:
    explicit EntryDialog(QWidget *parent = nullptr);

    ~EntryDialog() override;

    bool init(EntryDialogData *data);

    QToolButton *createCopyToolButton(const QVariant &property);

    QLineEdit *createDisplayLine(const QString &text);

    QLabel *createLabel(const QString &text);

private slots:

    void copyToClipboard();

    void on_buttonBox_accepted();

private:
    Ui::EntryDialog *ui;
};

#endif // ENTRYDIALOG_H
