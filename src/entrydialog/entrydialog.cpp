#include "entrydialog.h"
#include "ui_entrydialog.h"

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

EntryDialog::EntryDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EntryDialog) {
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &EntryDialog::close);
}

EntryDialog::~EntryDialog() {
    delete ui;
}

bool EntryDialog::init(EntryDialogData *data) {
    // Set titles
    setWindowTitle(QStringLiteral("KWallet Entry"));
    ui->entryBox->setTitle(data->entry);

    // Open an instance of the wallet and set the folder
    Wallet *wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Synchronous);
    if (!wallet->isOpen()) {
        KNotification::event(KNotification::Error,
                             QStringLiteral("KWallet"),
                             QStringLiteral("Could not open KWallet!"),
                             QStringLiteral("kwalletmanager"));
        return false;
    }
    wallet->setFolder(data->folder);
    const auto type = wallet->entryType(data->entry);

    // For Password, just add a QTextEdit
    if (type == Wallet::Password) {
        QString entryPassword;
        wallet->readPassword(data->entry, entryPassword);

        auto *horizontalLayout = new QHBoxLayout(this);
        ui->entryLayout->addLayout(horizontalLayout);
        horizontalLayout->addWidget(createDisplayLine(entryPassword));
        horizontalLayout->addWidget(createCopyToolButton(entryPassword));
    } else if (type == Wallet::Map) {
        QMap<QString, QString> entryMap;
        wallet->readMap(data->entry, entryMap);

        QMap<QString, QString>::const_iterator i = entryMap.constBegin();
        while (i != entryMap.constEnd()) {
            ui->entryLayout->addWidget(createLabel(i.key()));
            auto *horizontalLayout = new QHBoxLayout(this);
            ui->entryLayout->addLayout(horizontalLayout);
            horizontalLayout->addWidget(createDisplayLine(i.value()));
            horizontalLayout->addWidget(createCopyToolButton(i.value()));
            ++i;
        }
    } else if (type == Wallet::Stream) {
        ui->entryLayout->addWidget(createLabel(QStringLiteral("Stream Type")));
        QByteArray entryPassword;
        wallet->readEntry(data->entry, entryPassword);

        auto *horizontalLayout = new QHBoxLayout(this);
        ui->entryLayout->addLayout(horizontalLayout);
        horizontalLayout->addWidget(createLabel(QStringLiteral("Password in text")));
        horizontalLayout->addWidget(createDisplayLine(entryPassword));
        horizontalLayout->addWidget(createCopyToolButton(entryPassword));

        auto *horizontalLayout2 = new QHBoxLayout(this);
        ui->entryLayout->addLayout(horizontalLayout2);
        horizontalLayout2->addWidget(createLabel(QStringLiteral("Password in hex ")));
        horizontalLayout2->addWidget(createDisplayLine(entryPassword.toHex()));
        horizontalLayout2->addWidget(createCopyToolButton(entryPassword.toHex()));
    } else {
        KNotification::event(KNotification::Error,
                             QStringLiteral("KWallet"),
                             QStringLiteral("Could not open entry, please use the KWalletManager tool!"),
                             QStringLiteral("kwalletmanager"));
        return false;
    }

    delete wallet;
    return true;
}

void EntryDialog::copyToClipboard() {
    const QString value = QObject::sender()->property("value").toString();
    QApplication::clipboard()->setText(value);
}

QLineEdit *EntryDialog::createDisplayLine(const QString &text) {
    auto *lineEdit = new QLineEdit(this);
    lineEdit->setText(text);
    lineEdit->setReadOnly(true);
    return lineEdit;
}

QToolButton *EntryDialog::createCopyToolButton(const QVariant &property) {
    auto *copyButton = new QToolButton(this);
    copyButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    copyButton->setProperty("value", property);
    connect(copyButton, &QToolButton::clicked, this, &EntryDialog::copyToClipboard);
    return copyButton;
}

QLabel *EntryDialog::createLabel(const QString &text) {
    auto *label = new QLabel(this);
    label->setText(text);
    label->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByKeyboard |
                                   Qt::TextInteractionFlag::TextSelectableByMouse);
    return label;
}

