/********************************************************************************
** Form generated from reading UI file 'mxflash.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MXFLASH_H
#define UI_MXFLASH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mxflash
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *pageAction;
    QLabel *labelAction;
    QGroupBox *whatGroupBox;
    QGridLayout *_2;
    QRadioButton *removeRadioButton;
    QRadioButton *installRadioButton;
    QRadioButton *updateRadioButton;
    QRadioButton *oldRadioButton;
    QWidget *pageRemove;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *removeBox;
    QLabel *labelRemove;
    QWidget *pageUpdate;
    QGroupBox *updateBox;
    QGroupBox *whatUpdateBox;
    QGridLayout *_3;
    QRadioButton *manualRadioButton;
    QRadioButton *automaticRadioButton;
    QLabel *whatUpdateLabel;
    QWidget *pageInstall;
    QGroupBox *installBox;
    QLabel *labelInstall;
    QProgressBar *progressBar;
    QGridLayout *buttonBar;
    QPushButton *buttonAbout;
    QSpacerItem *horizontalSpacer1;
    QPushButton *buttonHelp;
    QPushButton *buttonOk;
    QSpacerItem *horizontalSpacer2;
    QPushButton *buttonCancel;
    QLabel *label;

    void setupUi(QDialog *mxflash)
    {
        if (mxflash->objectName().isEmpty())
            mxflash->setObjectName(QString::fromUtf8("mxflash"));
        mxflash->resize(548, 277);
        verticalLayout = new QVBoxLayout(mxflash);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(mxflash);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageAction = new QWidget();
        pageAction->setObjectName(QString::fromUtf8("pageAction"));
        labelAction = new QLabel(pageAction);
        labelAction->setObjectName(QString::fromUtf8("labelAction"));
        labelAction->setGeometry(QRect(20, 20, 481, 71));
        labelAction->setTextFormat(Qt::RichText);
        labelAction->setWordWrap(true);
        whatGroupBox = new QGroupBox(pageAction);
        whatGroupBox->setObjectName(QString::fromUtf8("whatGroupBox"));
        whatGroupBox->setGeometry(QRect(0, 70, 491, 111));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(whatGroupBox->sizePolicy().hasHeightForWidth());
        whatGroupBox->setSizePolicy(sizePolicy);
        _2 = new QGridLayout(whatGroupBox);
        _2->setSpacing(6);
        _2->setContentsMargins(9, 9, 9, 9);
        _2->setObjectName(QString::fromUtf8("_2"));
        removeRadioButton = new QRadioButton(whatGroupBox);
        removeRadioButton->setObjectName(QString::fromUtf8("removeRadioButton"));
        removeRadioButton->setChecked(true);

        _2->addWidget(removeRadioButton, 0, 0, 1, 1);

        installRadioButton = new QRadioButton(whatGroupBox);
        installRadioButton->setObjectName(QString::fromUtf8("installRadioButton"));

        _2->addWidget(installRadioButton, 4, 0, 1, 1);

        updateRadioButton = new QRadioButton(whatGroupBox);
        updateRadioButton->setObjectName(QString::fromUtf8("updateRadioButton"));

        _2->addWidget(updateRadioButton, 2, 0, 1, 1);

        oldRadioButton = new QRadioButton(whatGroupBox);
        oldRadioButton->setObjectName(QString::fromUtf8("oldRadioButton"));

        _2->addWidget(oldRadioButton, 6, 0, 1, 1);

        stackedWidget->addWidget(pageAction);
        pageRemove = new QWidget();
        pageRemove->setObjectName(QString::fromUtf8("pageRemove"));
        verticalLayout_2 = new QVBoxLayout(pageRemove);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        removeBox = new QGroupBox(pageRemove);
        removeBox->setObjectName(QString::fromUtf8("removeBox"));
        labelRemove = new QLabel(removeBox);
        labelRemove->setObjectName(QString::fromUtf8("labelRemove"));
        labelRemove->setGeometry(QRect(10, 30, 481, 91));
        labelRemove->setWordWrap(true);

        verticalLayout_2->addWidget(removeBox);

        stackedWidget->addWidget(pageRemove);
        pageUpdate = new QWidget();
        pageUpdate->setObjectName(QString::fromUtf8("pageUpdate"));
        updateBox = new QGroupBox(pageUpdate);
        updateBox->setObjectName(QString::fromUtf8("updateBox"));
        updateBox->setGeometry(QRect(0, 10, 521, 201));
        whatUpdateBox = new QGroupBox(updateBox);
        whatUpdateBox->setObjectName(QString::fromUtf8("whatUpdateBox"));
        whatUpdateBox->setGeometry(QRect(0, 0, 511, 100));
        sizePolicy.setHeightForWidth(whatUpdateBox->sizePolicy().hasHeightForWidth());
        whatUpdateBox->setSizePolicy(sizePolicy);
        _3 = new QGridLayout(whatUpdateBox);
        _3->setSpacing(6);
        _3->setContentsMargins(9, 9, 9, 9);
        _3->setObjectName(QString::fromUtf8("_3"));
        manualRadioButton = new QRadioButton(whatUpdateBox);
        manualRadioButton->setObjectName(QString::fromUtf8("manualRadioButton"));
        sizePolicy.setHeightForWidth(manualRadioButton->sizePolicy().hasHeightForWidth());
        manualRadioButton->setSizePolicy(sizePolicy);
        manualRadioButton->setChecked(true);

        _3->addWidget(manualRadioButton, 1, 0, 1, 1);

        automaticRadioButton = new QRadioButton(whatUpdateBox);
        automaticRadioButton->setObjectName(QString::fromUtf8("automaticRadioButton"));
        sizePolicy.setHeightForWidth(automaticRadioButton->sizePolicy().hasHeightForWidth());
        automaticRadioButton->setSizePolicy(sizePolicy);

        _3->addWidget(automaticRadioButton, 3, 0, 1, 1);

        whatUpdateLabel = new QLabel(whatUpdateBox);
        whatUpdateLabel->setObjectName(QString::fromUtf8("whatUpdateLabel"));

        _3->addWidget(whatUpdateLabel, 0, 0, 1, 1);

        stackedWidget->addWidget(pageUpdate);
        pageInstall = new QWidget();
        pageInstall->setObjectName(QString::fromUtf8("pageInstall"));
        installBox = new QGroupBox(pageInstall);
        installBox->setObjectName(QString::fromUtf8("installBox"));
        installBox->setGeometry(QRect(10, 20, 512, 201));
        labelInstall = new QLabel(installBox);
        labelInstall->setObjectName(QString::fromUtf8("labelInstall"));
        labelInstall->setGeometry(QRect(10, 30, 481, 91));
        labelInstall->setWordWrap(true);
        stackedWidget->addWidget(pageInstall);

        verticalLayout->addWidget(stackedWidget);

        progressBar = new QProgressBar(mxflash);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        buttonBar = new QGridLayout();
        buttonBar->setSpacing(5);
        buttonBar->setContentsMargins(0, 0, 0, 0);
        buttonBar->setObjectName(QString::fromUtf8("buttonBar"));
        buttonBar->setSizeConstraint(QLayout::SetDefaultConstraint);
        buttonAbout = new QPushButton(mxflash);
        buttonAbout->setObjectName(QString::fromUtf8("buttonAbout"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../../../usr/share/icons/Tango/16x16/apps/accessories-text-editor.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonAbout->setIcon(icon);
        buttonAbout->setAutoDefault(true);

        buttonBar->addWidget(buttonAbout, 0, 0, 1, 1);

        horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonBar->addItem(horizontalSpacer1, 0, 2, 1, 1);

        buttonHelp = new QPushButton(mxflash);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonHelp->sizePolicy().hasHeightForWidth());
        buttonHelp->setSizePolicy(sizePolicy1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../../../../../../../../usr/share/icons/Tango/16x16/apps/gnome-help.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonHelp->setIcon(icon1);

        buttonBar->addWidget(buttonHelp, 0, 1, 1, 1);

        buttonOk = new QPushButton(mxflash);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../../../../../../../../../usr/share/mx-user/icons/dialog-ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOk->setIcon(icon2);
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        buttonBar->addWidget(buttonOk, 0, 6, 1, 1);

        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonBar->addItem(horizontalSpacer2, 0, 5, 1, 1);

        buttonCancel = new QPushButton(mxflash);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../../../../../../../../../usr/share/icons/Tango/16x16/actions/gtk-cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonCancel->setIcon(icon3);
        buttonCancel->setAutoDefault(true);

        buttonBar->addWidget(buttonCancel, 0, 8, 1, 1);

        label = new QLabel(mxflash);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(32, 32));
        label->setMidLineWidth(0);
        label->setPixmap(QPixmap(QString::fromUtf8("../../../../../../../../../usr/local/share/icons/mxfcelogo.png")));
        label->setScaledContents(true);

        buttonBar->addWidget(label, 0, 3, 1, 1);


        verticalLayout->addLayout(buttonBar);


        retranslateUi(mxflash);
        QObject::connect(buttonCancel, SIGNAL(pressed()), mxflash, SLOT(reject()));

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mxflash);
    } // setupUi

    void retranslateUi(QDialog *mxflash)
    {
        mxflash->setWindowTitle(QApplication::translate("mxflash", "MX Flash Manager", 0, QApplication::UnicodeUTF8));
        labelAction->setText(QApplication::translate("mxflash", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Adobe Flash comes preinstalled on MX-14. However, it's a non-free program that has ongoing security problems that concern some users. Consult the Help document for details.</p>\n"
"<p align=\"justify\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">What would you like MX Flash Manager to do?</p></body></html>", 0, QApplication::UnicodeUTF8));
        removeRadioButton->setText(QApplication::translate("mxflash", "Remove Flash", 0, QApplication::UnicodeUTF8));
        installRadioButton->setText(QApplication::translate("mxflash", "Reinstall Flash", 0, QApplication::UnicodeUTF8));
        updateRadioButton->setText(QApplication::translate("mxflash", "Update Flash", 0, QApplication::UnicodeUTF8));
        oldRadioButton->setText(QApplication::translate("mxflash", "Use Old Flash", 0, QApplication::UnicodeUTF8));
        labelRemove->setText(QApplication::translate("mxflash", "Removing Flash...", 0, QApplication::UnicodeUTF8));
        manualRadioButton->setText(QApplication::translate("mxflash", "Update just one time", 0, QApplication::UnicodeUTF8));
        automaticRadioButton->setText(QApplication::translate("mxflash", "Update on an automatic schedule", 0, QApplication::UnicodeUTF8));
        whatUpdateLabel->setText(QApplication::translate("mxflash", "What kind of update do you prefer?", 0, QApplication::UnicodeUTF8));
        labelInstall->setText(QApplication::translate("mxflash", "Installing Flash...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonAbout->setToolTip(QApplication::translate("mxflash", "About this application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonAbout->setText(QApplication::translate("mxflash", "About...", 0, QApplication::UnicodeUTF8));
        buttonAbout->setShortcut(QApplication::translate("mxflash", "Alt+B", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonHelp->setToolTip(QApplication::translate("mxflash", "Display help ", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonHelp->setText(QApplication::translate("mxflash", "Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("mxflash", "Alt+H", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonOk->setToolTip(QApplication::translate("mxflash", "Apply any changes then quit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonOk->setText(QApplication::translate("mxflash", "OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
#ifndef QT_NO_TOOLTIP
        buttonCancel->setToolTip(QApplication::translate("mxflash", "Cancel any changes then quit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("mxflash", "Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QApplication::translate("mxflash", "Alt+N", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class mxflash: public Ui_mxflash {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MXFLASH_H
