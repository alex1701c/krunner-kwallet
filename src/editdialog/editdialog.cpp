#include "editdialog.h"
#include "ui_editdialog.h"

#include <QClipboard>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <KNotifications/KNotification>
#include <QStringBuilder>

EditDialog::EditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddDialog) {
    ui->setupUi(this);
}

EditDialog::~EditDialog() {
    delete ui;
    delete wallet;
}

bool EditDialog::init(AddDialogData *data) {
    ui->nameLineEdit->setText(data->name);
    initialName = data->name;
    ui->passwordPlainTextEdit->setText(data->value);

    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &EditDialog::validateEntryExists);
    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &EditDialog::validateSave);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDialog::saveEntry);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &EditDialog::close);
    ui->label->setHidden(true);

    return true;
}

void EditDialog::saveEntry() {
    const QString entryName = ui->nameLineEdit->text();
    wallet->setFolder("");
    if (wallet->writePassword(entryName, ui->passwordPlainTextEdit->text()) == 0) {
        displayUpdateNotification(entryName % QStringLiteral(" was saved to KWallet."), KNotification::Notification);
    } else {
        displayUpdateNotification(entryName % QStringLiteral(" could not saved to KWallet."), KNotification::Error);
    }

    close();
}

void EditDialog::displayUpdateNotification(const QString &msg, const KNotification::StandardEvent type) {
    KNotification::event(type, QStringLiteral("KWallet"), msg, QStringLiteral("kwalletmanager"));
}

void EditDialog::validateSave() {
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(ui->nameLineEdit->text().isEmpty());
}

void EditDialog::validateEntryExists() {
    ui->label->setHidden(!wallet->hasEntry(ui->nameLineEdit->text()));
}
