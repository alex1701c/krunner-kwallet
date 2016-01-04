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
#include <KLocalizedString>

#include "entrydialog.h"

KWalletRunner::KWalletRunner(QObject *parent, const QVariantList &args):
    Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);

    setObjectName(QLatin1String("KWallet Runner"));

    // Add the syntax
    addSyntax(Plasma::RunnerSyntax(QLatin1String("kwallet :q:"), i18n("Finds all KWallet entries matching :q:")));
    
    // Open the wallet
    m_wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);
}

KWalletRunner::~KWalletRunner()
{
    delete m_wallet;
}

void KWalletRunner::match(Plasma::RunnerContext &context)
{
    // Make sure command starts with "kwallet"
    if (context.isValid() && context.query().startsWith(QLatin1String("kwallet "), Qt::CaseInsensitive)) {
        
        // Get the search term
        QString searchTerm = context.query().split(" ", QString::SkipEmptyParts).at(1);
        
        // Cycle through each folder in our wallet
        Q_FOREACH (QString folderName, m_wallet->folderList()) {
            
            // Set the folder
            m_wallet->setFolder(folderName);
            
            // Cycle through each entry
            Q_FOREACH (QString entryName, m_wallet->entryList()) {
                
                // If there's no search term or the entry contains the search term...
                if (!searchTerm.length() || entryName.contains(searchTerm)) {
                
                    // ... add it to the list of results
                    Plasma::QueryMatch match(this);
                    match.setType(Plasma::QueryMatch::ExactMatch);
                    match.setText(entryName);
                    match.setSubtext(folderName);
                    context.addMatch(match);
                }
            }
        }
    }
}


void KWalletRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context)
    
    QString folder = match.subtext();
    QString entry = match.text();

    EntryDialog entryDialog;
    entryDialog.init(folder, entry);
    entryDialog.exec();
}

K_EXPORT_PLASMA_RUNNER(kwalletrunner, KWalletRunner)

#include "kwalletrunner.moc"
