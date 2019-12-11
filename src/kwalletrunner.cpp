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

#include "kwalletrunner.h"

#include <QDebug>
#include <QTimer>
#include <KLocalizedString>
#include <KNotifications/KNotification>
#include <KShell>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <adddialog/adddialog.h>

#include "entrydialog/entrydialog.h"

KWalletRunner::KWalletRunner(QObject *parent, const QVariantList &args) :
        Plasma::AbstractRunner(parent, args) {
    Q_UNUSED(args)

    setObjectName(QLatin1String("KWallet"));

    // Add the syntax
    addSyntax(Plasma::RunnerSyntax(QLatin1String("kwallet :q:"), i18n("Finds all KWallet entries matching :q:")));
    addSyntax(Plasma::RunnerSyntax(QLatin1String("kwallet-add :q:"),
                                   i18n(R"(Add an entry using syntax: entryName username="your_username" password="pass")")));

    // Open the wallet
    wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);

    auto *overview = addAction("overview", QIcon::fromTheme("documentinfo"), "Show Overview");
    overview->setData("overview");
    actions.clear();
    actions.append(overview);
}

KWalletRunner::~KWalletRunner() {
    delete wallet;
}

void KWalletRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;

    if (!Wallet::isEnabled() || !wallet->isOpen()) {
        KNotification::event(KNotification::Error, "KWallet", "Could not open KWallet!", "kwallet");
    }

    // Make sure command starts with "kwallet"
    if (context.query().startsWith(searchString, Qt::CaseInsensitive)) {
        // Get the search term
        const QString searchTerm = context.query().split(" ", QString::SkipEmptyParts).at(1);
        // Cycle through each folder in our wallet
        for (const QString &folderName: wallet->folderList()) {

            // Set the folder
            wallet->setFolder(folderName);

            // Cycle through each entry
            for (const QString &entryName: wallet->entryList()) {
                if (searchTerm.isEmpty() || entryName.contains(searchTerm, Qt::CaseInsensitive)) {
                    Plasma::QueryMatch match(this);
                    match.setType(Plasma::QueryMatch::ExactMatch);
                    match.setIconName("kwalletmanager");
                    match.setText(entryName);
                    match.setSubtext(folderName);
                    context.addMatch(match);
                }
            }
        }
    }

    // KWallet Add
    if (context.query().contains(addRegex)) {
        // Show option to open OneTimePass
        Plasma::QueryMatch newAction(this);
        newAction.setId("add");
        newAction.setType(Plasma::QueryMatch::HelperMatch);
        newAction.setIconName("kwalletmanager");
        newAction.setText("Add entry for " + context.query().remove(addRegex));
        newAction.setData(context.query());
        context.addMatch(newAction);
    }
}


void KWalletRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    // If we are adding an entry
    if (match.type() == Plasma::QueryMatch::HelperMatch) {
        auto *data = new AddDialogData(match.data().toString().remove(addRegex));
        QTimer::singleShot(0, data, [data]() {
            AddDialog addDialog;
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
            if (!resMap.isEmpty() && resMap.size() == 1) {
                setClipboardPassword(resMap.values().at(0));
                return;
            }
        }
    }

    // Fallback case
    QString folder = match.subtext();
    QString entry = match.text();
    auto *data = new EntryDialogData(folder, entry);
    QTimer::singleShot(0, data, [data]() {
        EntryDialog entryDialog;
        entryDialog.init(data);
        entryDialog.exec();
        delete data;
    });
}

QList<QAction *> KWalletRunner::actionsForMatch(const Plasma::QueryMatch &match) {
    Q_UNUSED(match)

    return actions;
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
