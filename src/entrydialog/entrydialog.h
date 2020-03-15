//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
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

    bool init(const EntryDialogData *data);
    QToolButton *createCopyToolButton(const QVariant &property);
    QLineEdit *createDisplayLine(const QString &text);
    QLabel *createLabel(const QString &text);

private slots:
    void copyToClipboard();

private:
    Ui::EntryDialog *ui;
};

#endif // ENTRYDIALOG_H
