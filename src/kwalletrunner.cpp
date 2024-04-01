//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
#include "kwalletrunner.h"

#include <krunner_version.h>
#include <QDebug>
#include <QTimer>
#include <KNotification>
#include <KShell>
#include <QAction>
#include <QApplication>
#include <QClipboard>


KWalletRunner::KWalletRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args) :
#if QT_VERSION_MAJOR == 5
        KRunner::AbstractRunner(parent, data, args) {
#else
        KRunner::AbstractRunner(parent, data) {
#endif
    addSyntax(KRunner::RunnerSyntax(QStringLiteral("kwallet :q:"),
                                   QStringLiteral("Finds all KWallet entries matching :q:")));
    addSyntax(KRunner::RunnerSyntax(QStringLiteral("kwallet-add :q:"), QStringLiteral("Add an entry")));

    // Open the wallet
    wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);
    wallet->setParent(this);

#if QT_VERSION_MAJOR==6
    actions = {
        KRunner::Action(QStringLiteral("edit"), QStringLiteral("document-edit"), QStringLiteral("Edit")),
        KRunner::Action(QStringLiteral("overview"), QStringLiteral("documentinfo"), QStringLiteral("Show Overview"))
    };
#else
    auto *overview = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")),
                               QStringLiteral("Show Overview"));
    overview->setData(QStringLiteral("overview"));
    auto *edit = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")),
                           QStringLiteral("Edit"));
    edit->setData(QStringLiteral("edit"));
    actions = {overview, edit};
#endif

    const bool validWallet = Wallet::isEnabled() && wallet;
    if (!validWallet || !wallet->isOpen()) {
        KNotification::event(KNotification::Error,
                             QStringLiteral("KWallet"),
                             QStringLiteral("Could not open KWallet!"),
                             QStringLiteral("kwallet"));
    suspendMatching(true);
    }
}

void KWalletRunner::match(KRunner::RunnerContext &context) {
    const QString query = context.query();
    // Make sure command starts with "kwallet"
    if (query.startsWith(searchString) || query.startsWith(shortSearchString)) {
        const QString searchTerm = query.split(QChar(' '), Qt::SkipEmptyParts).at(1);
        for (const QString &folderName: wallet->folderList()) {
            wallet->setFolder(folderName);
            for (const QString &entryName: wallet->entryList()) {
                qWarning()<<entryName;
                if (entryName.contains(searchTerm, Qt::CaseInsensitive)) {
                    KRunner::QueryMatch match(this);
#if KRUNNER_VERSION < QT_VERSION_CHECK(5, 113, 0)
                    match.setType(KRunner::QueryMatch::ExactMatch);
#else
                    match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);
#endif
                    match.setIconName(QStringLiteral("kwalletmanager"));
                    match.setText(entryName);
                    match.setSubtext(folderName);
                    match.setData(QStringList({folderName, entryName}));
                    match.setId(QString());
                    match.setActions(actions);
                    context.addMatch(match);
                }
            }
        }
    } else if (query.contains(addRegex) || query.contains(shortAddRegex)) {
        const QString entryName = QString(query).remove(addRegex).remove(shortAddRegex);
        // Set to default folder
        wallet->setFolder(QString());
        const bool entryExists = !entryName.isEmpty() && wallet->hasEntry(entryName);

        KRunner::QueryMatch match(this);
        match.setId(entryExists ? QStringLiteral("edit") : QStringLiteral("add"));
#if KRUNNER_VERSION < QT_VERSION_CHECK(5, 113, 0)
                    match.setType(KRunner::QueryMatch::ExactMatch);
#else
                    match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Moderate);
#endif
        match.setIconName(QStringLiteral("kwalletmanager"));
        if (entryExists) {
            match.setText(QStringLiteral("Edit entry for ") % entryName);
        } else if (!entryName.isEmpty()) {
            match.setText(QStringLiteral("Add entry for ") % entryName);
        } else {
            match.setText(QStringLiteral("Add entry"));
        }
        match.setData(QStringList({QString(), entryName}));
        context.addMatch(match);
    }
}

QString getSelectedMatchId(const KRunner::QueryMatch &match) {
    if (!match.selectedAction()) {
        return QString();
    }
#if QT_VERSION_MAJOR == 5
    return match.selectedAction()->data().toString();
#else
    return match.selectedAction().id();
#endif
}

void KWalletRunner::run(const KRunner::RunnerContext &/*context*/, const KRunner::QueryMatch &match) {
    // If we are adding or editing an entry
    if (//match.type() == KRunner::QueryMatch::HelperMatch ||
        (getSelectedMatchId(match) == QLatin1String("edit"))) {
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
                                     QStringLiteral(
                                             "Can not edit entry, please use the KwalletManager tool for this !"),
                                     QStringLiteral("kwallet"));
                return;
            }
        }
        QTimer::singleShot(0, this, [data]() {
            EditDialog addDialog;
            addDialog.init(data);
            addDialog.exec();
            delete data;
        });
        return;
    }

    else if (!match.selectedAction()) {
        // Default case
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
    QTimer::singleShot(0, this,  [data]() {
        EntryDialog entryDialog;
        if (entryDialog.init(data)) {
            entryDialog.exec();
        }
        delete data;
    });
}

void KWalletRunner::setClipboardPassword(const QString &password) {
    QClipboard *cb = QApplication::clipboard();
    cb->setText(password);
    QTimer::singleShot(5000, this, [cb]() {
        // Clipboard managers might cause the clear function to not work properly
        cb->setText(QString());
    });
}


K_PLUGIN_CLASS_WITH_JSON(KWalletRunner, "kwalletrunner.json")

#include "kwalletrunner.moc"
