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

#include "entrydialog.h"

KWalletRunner::KWalletRunner(QObject *parent, const QVariantList &args) :
        Plasma::AbstractRunner(parent, args) {
    Q_UNUSED(args);

    setObjectName(QLatin1String("KWallet"));

    // Add the syntax
    addSyntax(Plasma::RunnerSyntax(QLatin1String("kwallet :q:"), i18n("Finds all KWallet entries matching :q:")));
    addSyntax(Plasma::RunnerSyntax(QLatin1String("kwallet-add :q:"),
                                   i18n(R"(Add an entry using syntax: entryName username="your_username" password="pass")")));

    // Open the wallet
    m_wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);
}

KWalletRunner::~KWalletRunner() {
    delete m_wallet;
}

void KWalletRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;

    // Make sure command starts with "kwallet"
    if (context.query().startsWith(QLatin1String("kwallet "), Qt::CaseInsensitive)) {
        // Get the search term
        const QString searchTerm = context.query().split(" ", QString::SkipEmptyParts).at(1);
        // Cycle through each folder in our wallet
        for (const QString &folderName: m_wallet->folderList()) {

            // Set the folder
            m_wallet->setFolder(folderName);

            // Cycle through each entry
            for (const QString &entryName: m_wallet->entryList()) {
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
    if (context.isValid() && context.query().startsWith(QLatin1String("kwallet-add "), Qt::CaseInsensitive)) {
        // Get a list of the arguments  
        KShell::Errors splitArgsError;
        QStringList arguments = KShell::splitArgs(context.query(), KShell::AbortOnMeta, &splitArgsError);

        // If the arguments could not be split, abort
        if (splitArgsError != KShell::Errors::NoError) {
            return;
        }

        // Pop the first "kwallet-add" from the list of arguments
        arguments.pop_front();

        // If there's at least two arguments
        if (arguments.length() >= 2) {
            // Show option to open OneTimePass
            Plasma::QueryMatch newAction(this);
            newAction.setId("add");
            newAction.setType(Plasma::QueryMatch::HelperMatch);
            newAction.setIconName("kwalletmanager");
            newAction.setText("Add entry for " + arguments[0]);
            newAction.setData(context.query());
            context.addMatch(newAction);
        }
    }
}


void KWalletRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    // If we are adding an entry
    if (match.type() == Plasma::QueryMatch::HelperMatch) {
        // Get a list of the arguments  
        KShell::Errors splitArgsError;
        QStringList arguments = KShell::splitArgs(match.data().toString(), KShell::AbortOnMeta, &splitArgsError);

        // If the arguments could not be split, abort
        if (splitArgsError != KShell::Errors::NoError) {
            return;
        }

        // Pop the first "kwallet-add" from the list of arguments
        arguments.pop_front();

        // Variables to store final map in
        QString entryName = arguments[0];
        QMap<QString, QString> keyValueMap;

        // Pop the entry name from the list of arguments as we've already saved it
        arguments.pop_front();

        // Loop through and process each argument
        for (const QString &argument: arguments) {
            const QStringList keyValue = argument.split("=", QString::SkipEmptyParts);

            // If we could split in two, it's a valid key/value pair
            if (keyValue.length() == 2) {
                keyValueMap.insert(keyValue[0], keyValue[1]);
            }
        }

        m_wallet->setFolder("Passwords");

        // If we could successfully save this entry 
        if (m_wallet->writeMap(entryName, keyValueMap) == 0) {
            KNotification::event(KNotification::Notification, "KWallet", entryName + " added to KWallet.", "kwallet");
        } else { // Otherwise notify user of error saving
            KNotification::event(KNotification::Error, "KWallet", entryName + " could not be added.", "kwallet");
        }
    }

    // If we want to view an entry
    else {
        QString folder = match.subtext();
        QString entry = match.text();
        EntryDialog entryDialog;
        entryDialog.init(folder, entry);
        entryDialog.exec();
    }
}

K_EXPORT_PLASMA_RUNNER(kwallet, KWalletRunner)

#include "kwalletrunner.moc"
