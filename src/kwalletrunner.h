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

#ifndef KWALLETRUNNER_H
#define KWALLETRUNNER_H

#include <KRunner/AbstractRunner>
#include <KWallet/KWallet>

using KWallet::Wallet;

class QAction;

class KWalletRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:

    KWalletRunner(QObject *parent, const QVariantList &args);

    ~KWalletRunner() override;

    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

    QList<QAction *> actionsForMatch(const Plasma::QueryMatch &match) override;

private:
    Wallet *m_wallet;
};

#endif /* KWALLETRUNNER_H */
