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

    void validateSave();

    void validateEntryExists();

    void saveEntry();

private:
    Ui::AddDialog *ui;
    Wallet *wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Synchronous);
    QString initialName;

    static void displayUpdateNotification(const QString &msg, const KNotification::StandardEvent type);
};

#endif // ADDDIALOG_H
