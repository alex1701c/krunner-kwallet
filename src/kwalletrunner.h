#ifndef KWALLETRUNNER_H
#define KWALLETRUNNER_H

#include <KRunner/AbstractRunner>
#include <KWallet/KWallet>
#include <QStringBuilder>

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
    const QString defaultMatchId = "kwalletrunner";
    const QRegExp addRegex = QRegExp(QStringLiteral("^kwallet-add ?"));
};

#endif /* KWALLETRUNNER_H */
