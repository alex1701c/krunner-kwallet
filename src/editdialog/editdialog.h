//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <KNotification>
#include <KWallet>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <utility>

using KWallet::Wallet;

namespace Ui
{
class AddDialog;
}

class EditDialogData : public QObject
{
    Q_OBJECT
public:
    const QString name;
    const QString folder;
    QString value;

public:
    explicit EditDialogData(QString name = "", QString folder = "", QObject *parent = nullptr)
        : QObject(parent)
        , name(std::move(name))
        , folder(std::move(folder))
    {
    }
};

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog() override;

    bool init(EditDialogData *data);

private slots:
    void validateSave();
    void validateEntryExists();
    void saveEntry();

private:
    Ui::AddDialog *ui;
    Wallet *wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Synchronous);
    QString initialName;

    static void displayUpdateNotification(const QString &msg, KNotification::StandardEvent type);
};

#endif // ADDDIALOG_H
