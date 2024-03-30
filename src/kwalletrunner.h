//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.

#pragma once

#include <KRunner/AbstractRunner>
#include <KWallet/KWallet>
#include <QStringBuilder>
#include <QRegularExpression>

#include <editdialog/editdialog.h>
#include "entrydialog/entrydialog.h"

using KWallet::Wallet;

class KWalletRunner : public KRunner::AbstractRunner {
Q_OBJECT

public:
    KWalletRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
    QList<QAction *> actionsForMatch(const KRunner::QueryMatch &match) override;

    void setClipboardPassword(const QString &password);

private:
    Wallet *wallet;
    QList<QAction *> actions;
    const QLatin1String searchString{"kwallet "};
    const QLatin1String shortSearchString{"kwl "};
    const QLatin1String defaultMatchId{"kwalletrunner"};
    const QRegularExpression addRegex{QStringLiteral("^kwallet-add ?")};
    const QRegularExpression shortAddRegex{QStringLiteral("^kwl-add ?")};
};
