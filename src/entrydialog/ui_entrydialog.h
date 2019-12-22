#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'entrydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRYDIALOG_H
#define UI_ENTRYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EntryDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *entryBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *entryLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EntryDialog)
    {
        if (EntryDialog->objectName().isEmpty())
            EntryDialog->setObjectName(QString::fromUtf8("EntryDialog"));
        EntryDialog->resize(313, 153);
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("dialog-password")));
        EntryDialog->setWindowIcon(icon);
        EntryDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(EntryDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        entryBox = new QGroupBox(EntryDialog);
        entryBox->setObjectName(QString::fromUtf8("entryBox"));
        verticalLayout_3 = new QVBoxLayout(entryBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        entryLayout = new QVBoxLayout();
        entryLayout->setObjectName(QString::fromUtf8("entryLayout"));

        verticalLayout_3->addLayout(entryLayout);


        verticalLayout_2->addWidget(entryBox);

        buttonBox = new QDialogButtonBox(EntryDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(EntryDialog);

        QMetaObject::connectSlotsByName(EntryDialog);
    } // setupUi

    void retranslateUi(QDialog *EntryDialog)
    {
        EntryDialog->setWindowTitle(tr2i18n("Dialog", nullptr));
        entryBox->setTitle(tr2i18n("Entry", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntryDialog: public Ui_EntryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENTRYDIALOG_H

