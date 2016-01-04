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

#include "entrydialog.h"
#include "ui_entrydialog.h"

#include <QClipboard>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include <QDebug>

EntryDialog::EntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryDialog)
{
    ui->setupUi(this);
}

EntryDialog::~EntryDialog()
{
    delete ui;
}

bool EntryDialog::init(QString folder, QString entry)
{
    // Set titles
    setWindowTitle("KWallet Entry");
    ui->entryBox->setTitle(entry);

    // Open an instance of the wallet and set the folder
    Wallet *wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Synchronous);
    wallet->setFolder(folder);

    // Depending on the type of entry (Map, Password, etc), add a different set of widgets
    switch (wallet->entryType(entry)) {

        // For Password, just add a QTextEdit
        case Wallet::Password: {
            QString entryPassword;
            wallet->readPassword(entry, entryPassword);

            QTextEdit *textEdit = new QTextEdit(this);
            textEdit->setReadOnly(true);
            textEdit->setText(entryPassword);
            ui->entryLayout->addWidget(textEdit);

            break;
        }

        // For Map, add a QLabel and QLineEdit per map
        case Wallet::Map: {
            QMap<QString, QString> entryMap;
            wallet->readMap(entry, entryMap);

            QMap<QString, QString>::const_iterator i = entryMap.constBegin();
            while (i != entryMap.constEnd()) {

                // Add QLabel
                QLabel *label = new QLabel(this);
                label->setText(i.key());
                ui->entryLayout->addWidget(label);

                // Add QLineEdit and Copy Button
                QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
                ui->entryLayout->addLayout(horizontalLayout);
                QLineEdit *lineEdit = new QLineEdit(this);
                lineEdit->setText(i.value());
                lineEdit->setReadOnly(true);
                horizontalLayout->addWidget(lineEdit);
                QToolButton *copyButton = new QToolButton(this);
                copyButton->setIcon(QIcon::fromTheme("edit-copy"));
                copyButton->setProperty("value", i.value());
                connect(copyButton, SIGNAL(clicked(bool)), this, SLOT(copyToClipboard()));
                horizontalLayout->addWidget(copyButton);

                i++;
            }

            break;
        }
    }

    // Delete the wallet
    delete wallet;

    return true;
}

void EntryDialog::copyToClipboard()
{
    QString value = QObject::sender()->property("value").toString();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(value);
}

void EntryDialog::on_buttonBox_accepted()
{
    close();
}
