//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
#include "editdialog.h"
#include "ui_editdialog.h"

#include <KNotification>
#include <QClipboard>
#include <QDebug>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStringBuilder>

EditDialog::EditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

EditDialog::~EditDialog()
{
    delete ui;
    delete wallet;
}

bool EditDialog::init(EditDialogData *data)
{
    ui->nameLineEdit->setText(data->name);
    initialName = data->name;
    ui->passwordPlainTextEdit->setText(data->value);
    ui->label->setHidden(true);
    wallet->setFolder(data->folder);

    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &EditDialog::validateEntryExists);
    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &EditDialog::validateSave);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDialog::saveEntry);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &EditDialog::close);

    return true;
}

void EditDialog::saveEntry()
{
    const QString entryName = ui->nameLineEdit->text();
    if (wallet->writePassword(entryName, ui->passwordPlainTextEdit->text()) == 0) {
        if (!initialName.isEmpty() && entryName != initialName) {
            wallet->removeEntry(initialName);
        }
        displayUpdateNotification(entryName % QStringLiteral(" was saved to KWallet."), KNotification::Notification);
    } else {
        displayUpdateNotification(entryName % QStringLiteral(" could not saved to KWallet."), KNotification::Error);
    }

    close();
}

void EditDialog::displayUpdateNotification(const QString &msg, const KNotification::StandardEvent type)
{
    KNotification::event(type, QStringLiteral("KWallet"), msg, QStringLiteral("kwalletmanager"));
}

void EditDialog::validateSave()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(ui->nameLineEdit->text().isEmpty());
}

void EditDialog::validateEntryExists()
{
    ui->label->setHidden(!wallet->hasEntry(ui->nameLineEdit->text()));
}

#include "moc_editdialog.cpp"
