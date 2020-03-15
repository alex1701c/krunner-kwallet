//  Licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See License in the project root for license information.
#ifndef KWALLETRUNNER_H
#define KWALLETRUNNER_H

#include <KRunner/AbstractRunner>
#include <KWallet/KWallet>
#include <QStringBuilder>
#include <QRegularExpression>

#include <editdialog/editdialog.h>
#include "entrydialog/entrydialog.h"

using KWallet::Wallet;

class KWalletRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:
    KWalletRunner(QObject *parent, const QVariantList &args);
    ~KWalletRunner() override;

    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
    QList<QAction *> actionsForMatch(const Plasma::QueryMatch &match) override;

    static void setClipboardPassword(const QString &password);

private:
    Wallet *wallet;
    QList<QAction *> actions;
    const QLatin1String searchString = QLatin1String("kwallet ");
    const QLatin1String shortSearchString = QLatin1String("kwl ");
    const QLatin1String defaultMatchId = QLatin1String("kwalletrunner");
    const QRegularExpression addRegex = QRegularExpression(QStringLiteral("^kwallet-add ?"));
    const QRegularExpression shortAddRegex = QRegularExpression(QStringLiteral("^kwl-add ?"));
    bool validWallet;
};

#endif /* KWALLETRUNNER_H */
