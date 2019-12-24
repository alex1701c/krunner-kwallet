//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
#include "kwalletrunner.h"

#include <QDebug>
#include <QTimer>
#include <KLocalizedString>
#include <KNotifications/KNotification>
#include <KShell>
#include <QAction>
#include <QApplication>
#include <QClipboard>


KWalletRunner::KWalletRunner(QObject *parent, const QVariantList &args) :
        Plasma::AbstractRunner(parent, args) {
    Q_UNUSED(args)

    setObjectName(QLatin1String("KWallet"));

    // Add the syntax
    addSyntax(Plasma::RunnerSyntax(QStringLiteral("kwallet :q:"),
                                   i18n("Finds all KWallet entries matching :q:")));
    addSyntax(Plasma::RunnerSyntax(QStringLiteral("kwallet-add :q:"), i18n(R"(Add an entry)")));

    // Open the wallet
    wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);

    auto *overview = addAction(QStringLiteral("overview"),
                               QIcon::fromTheme(QStringLiteral("documentinfo")),
                               QStringLiteral("Show Overview"));
    overview->setData(QStringLiteral("overview"));
    auto *edit = addAction(QStringLiteral("overview"),
                           QIcon::fromTheme(QStringLiteral("document-edit")),
                           QStringLiteral("Edit"));
    edit->setData(QStringLiteral("edit"));
    actions.clear();
    actions.append(overview);
    actions.append(edit);
}

KWalletRunner::~KWalletRunner() {
    delete wallet;
}

void KWalletRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;

    if (!Wallet::isEnabled() || !wallet->isOpen()) {
        KNotification::event(KNotification::Error,
                             QStringLiteral("KWallet"),
                             QStringLiteral("Could not open KWallet!"),
                             QStringLiteral("kwallet"));
        return;
    }
    const QString query = context.query();
    // Make sure command starts with "kwallet"
    if (query.startsWith(searchString) || query.startsWith(shortSearchString)) {
        // Get the search term
        const QString searchTerm = query.split(" ", QString::SkipEmptyParts).at(1);
        // Cycle through each folder in our wallet
        for (const QString &folderName: wallet->folderList()) {

            // Set the folder
            wallet->setFolder(folderName);

            // Cycle through each entry
            for (const QString &entryName: wallet->entryList()) {
                if (entryName.contains(searchTerm, Qt::CaseInsensitive)) {
                    Plasma::QueryMatch match(this);
                    match.setType(Plasma::QueryMatch::ExactMatch);
                    match.setIconName(QStringLiteral("kwalletmanager"));
                    match.setText(entryName);
                    match.setSubtext(folderName);
                    match.setData(QStringList({folderName, entryName}));
                    match.setId("");
                    context.addMatch(match);
                }
            }
        }
    }

    // KWallet Add
    if (query.contains(addRegex) || query.contains(shortAddRegex)) {
        const QString entryName = QString(query).remove(addRegex).remove(shortAddRegex);
        // Set to default folder
        wallet->setFolder("");
        const bool entryExists = !entryName.isEmpty() && wallet->hasEntry(entryName);

        Plasma::QueryMatch match(this);
        match.setId(entryExists ? QStringLiteral("edit") : QStringLiteral("add"));
        match.setType(Plasma::QueryMatch::HelperMatch);
        match.setIconName(QStringLiteral("kwalletmanager"));
        if (entryExists) {
            match.setText(QStringLiteral("Edit entry for ") % entryName);
        } else if (!entryName.isEmpty()) {
            match.setText(QStringLiteral("Add entry for ") % entryName);
        } else {
            match.setText(QStringLiteral("Add entry"));
        }
        match.setData(QStringList({"", entryName}));
        context.addMatch(match);

    }
}


void KWalletRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    // If we are adding or editing an entry
    if (match.type() == Plasma::QueryMatch::HelperMatch ||
        (match.selectedAction() != nullptr && match.selectedAction()->data().toString() == QLatin1String("edit"))) {
        const QStringList matchData = match.data().toStringList();
        auto *data = new EditDialogData(QString(matchData.at(1)).remove(addRegex), matchData.at(0));
        wallet->setFolder(data->folder);
        if (wallet->hasEntry(data->name)) {
            if (wallet->entryType(data->name) == Wallet::Password) {
                QString password;
                wallet->readPassword(data->name, password);
                data->value = password;
            } else {
                KNotification::event(KNotification::Error,
                                     QStringLiteral("KWallet"),
                                     QStringLiteral("Can not edit entry, please use the KwalletManager tool for this !"),
                                     QStringLiteral("kwallet"));
                return;
            }

        }
        QTimer::singleShot(0, data, [data]() {
            EditDialog addDialog;
            addDialog.init(data);
            addDialog.exec();
            delete data;
        });
        return;
    }

        // Default case
    else if (match.selectedAction() == nullptr) {
        wallet->setFolder(match.subtext());
        const Wallet::EntryType entryType = wallet->entryType(match.text());
        if (entryType == Wallet::Password) {
            QString password;
            wallet->readPassword(match.text(), password);
            setClipboardPassword(password);
            return;
        } else if (entryType == Wallet::Map) {
            QMap<QString, QString> resMap;
            wallet->readMap(match.text(), resMap);
            if (resMap.size() == 1) {
                setClipboardPassword(resMap.values().at(0));
                return;
            }
        }
    }

    // Fallback case
    const auto *data = new EntryDialogData(match.subtext(), match.text());
    QTimer::singleShot(0, data, [data]() {
        EntryDialog entryDialog;
        if (entryDialog.init(data)) {
            entryDialog.exec();
        }
        delete data;
    });
}

QList<QAction *> KWalletRunner::actionsForMatch(const Plasma::QueryMatch &match) {
    if (match.id() == defaultMatchId) {
        return actions;
    }
    return QList<QAction *>();
}

void KWalletRunner::setClipboardPassword(const QString &password) {
    QClipboard *cb = QApplication::clipboard();
    cb->setText(password);
    QTimer::singleShot(5000, cb, [cb]() {
        // Clipboard managers might cause the clear function to not work properly
        cb->setText("");
    });
}


K_EXPORT_PLASMA_RUNNER(kwallet, KWalletRunner)

#include "kwalletrunner.moc"
