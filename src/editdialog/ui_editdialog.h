#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'editdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITDIALOG_H
#define UI_EDITDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *entryBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *entryLayout;
    QLabel *label;
    QLineEdit *nameLineEdit;
    QLineEdit *passwordPlainTextEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(313, 194);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("dialog-password");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        AddDialog->setWindowIcon(icon);
        AddDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(AddDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        entryBox = new QGroupBox(AddDialog);
        entryBox->setObjectName(QString::fromUtf8("entryBox"));
        verticalLayout_3 = new QVBoxLayout(entryBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        entryLayout = new QVBoxLayout();
        entryLayout->setObjectName(QString::fromUtf8("entryLayout"));
        label = new QLabel(entryBox);
        label->setObjectName(QString::fromUtf8("label"));

        entryLayout->addWidget(label);

        nameLineEdit = new QLineEdit(entryBox);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        entryLayout->addWidget(nameLineEdit);

        passwordPlainTextEdit = new QLineEdit(entryBox);
        passwordPlainTextEdit->setObjectName(QString::fromUtf8("passwordPlainTextEdit"));

        entryLayout->addWidget(passwordPlainTextEdit);


        verticalLayout_3->addLayout(entryLayout);


        verticalLayout_2->addWidget(entryBox);

        buttonBox = new QDialogButtonBox(AddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(tr2i18n("Dialog", nullptr));
        entryBox->setTitle(QString());
        label->setText(tr2i18n("Info: This entry already exists !", nullptr));
        nameLineEdit->setPlaceholderText(tr2i18n("Name", nullptr));
        passwordPlainTextEdit->setText(QString());
        passwordPlainTextEdit->setPlaceholderText(tr2i18n("Password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EDITDIALOG_H

