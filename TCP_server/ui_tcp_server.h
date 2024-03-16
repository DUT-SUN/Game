/********************************************************************************
** Form generated from reading UI file 'tcp_server.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCP_SERVER_H
#define UI_TCP_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TCP_server
{
public:
    QWidget *widget;
    QLabel *label;
    QSpinBox *spinBox;
    QPushButton *begin_button;
    QLabel *label_2;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_4;
    QLCDNumber *Black_timer;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QLabel *label_6;
    QLCDNumber *White_timer;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QFrame *line;
    QLineEdit *lineEdit;
    QPushButton *push_button;
    QTextBrowser *textBrowser;
    QLabel *label_7;

    void setupUi(QWidget *TCP_server)
    {
        if (TCP_server->objectName().isEmpty())
            TCP_server->setObjectName(QString::fromUtf8("TCP_server"));
        TCP_server->resize(1058, 741);
        TCP_server->setMouseTracking(true);
        widget = new QWidget(TCP_server);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(810, 60, 251, 611));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 60, 61, 21));
        spinBox = new QSpinBox(widget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(130, 60, 81, 22));
        spinBox->setReadOnly(false);
        spinBox->setMaximum(65535);
        spinBox->setValue(9999);
        begin_button = new QPushButton(widget);
        begin_button->setObjectName(QString::fromUtf8("begin_button"));
        begin_button->setGeometry(QRect(40, 100, 161, 31));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 140, 51, 16));
        dockWidget = new QDockWidget(TCP_server);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setEnabled(true);
        dockWidget->setGeometry(QRect(120, 370, 451, 171));
        dockWidget->setMouseTracking(false);
        dockWidget->setAcceptDrops(false);
        dockWidget->setFloating(true);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        widget_3 = new QWidget(dockWidgetContents);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 0, 131, 151));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("image: url(:/res/black.png);"));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/img/black.png")));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(widget_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(18);
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_4);

        Black_timer = new QLCDNumber(widget_3);
        Black_timer->setObjectName(QString::fromUtf8("Black_timer"));
        Black_timer->setDigitCount(5);
        Black_timer->setSegmentStyle(QLCDNumber::Flat);

        verticalLayout->addWidget(Black_timer);

        widget_4 = new QWidget(dockWidgetContents);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(140, 0, 141, 151));
        verticalLayout_2 = new QVBoxLayout(widget_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_5 = new QLabel(widget_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("image: url(:/res/white.png);"));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/img/white.png")));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_5);

        label_6 = new QLabel(widget_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_6);

        White_timer = new QLCDNumber(widget_4);
        White_timer->setObjectName(QString::fromUtf8("White_timer"));
        White_timer->setDigitCount(5);
        White_timer->setSegmentStyle(QLCDNumber::Flat);

        verticalLayout_2->addWidget(White_timer);

        pushButton_2 = new QPushButton(dockWidgetContents);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(290, 0, 151, 28));
        pushButton_3 = new QPushButton(dockWidgetContents);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(290, 30, 151, 28));
        pushButton_4 = new QPushButton(dockWidgetContents);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(290, 60, 151, 28));
        pushButton_5 = new QPushButton(dockWidgetContents);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(290, 90, 151, 28));
        pushButton_6 = new QPushButton(dockWidgetContents);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(290, 120, 151, 28));
        dockWidget->setWidget(dockWidgetContents);
        line = new QFrame(TCP_server);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(800, 0, 20, 741));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        lineEdit = new QLineEdit(TCP_server);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);
        lineEdit->setGeometry(QRect(810, 710, 181, 21));
        push_button = new QPushButton(TCP_server);
        push_button->setObjectName(QString::fromUtf8("push_button"));
        push_button->setEnabled(false);
        push_button->setGeometry(QRect(1010, 710, 41, 21));
        textBrowser = new QTextBrowser(TCP_server);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(810, 220, 251, 481));
        label_7 = new QLabel(TCP_server);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(830, 20, 221, 71));
        label_7->setStyleSheet(QString::fromUtf8("font-size:30px"));

        retranslateUi(TCP_server);

        QMetaObject::connectSlotsByName(TCP_server);
    } // setupUi

    void retranslateUi(QWidget *TCP_server)
    {
        TCP_server->setWindowTitle(QCoreApplication::translate("TCP_server", "TCP_server", nullptr));
        label->setText(QCoreApplication::translate("TCP_server", "\347\253\257\345\217\243\345\217\267:", nullptr));
        begin_button->setText(QCoreApplication::translate("TCP_server", "\345\220\257\345\212\250", nullptr));
        label_2->setText(QCoreApplication::translate("TCP_server", "\350\201\212\345\244\251\346\241\206", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("TCP_server", "\345\267\245\345\205\267\346\240\217", nullptr));
        label_3->setText(QString());
        label_4->setText(QCoreApplication::translate("TCP_server", "\345\200\222\350\256\241\346\227\266", nullptr));
        label_5->setText(QString());
        label_6->setText(QCoreApplication::translate("TCP_server", "\345\200\222\350\256\241\346\227\266", nullptr));
        pushButton_2->setText(QCoreApplication::translate("TCP_server", "\347\224\263\350\257\267\345\274\200/\345\205\263\351\231\220\346\227\266\346\250\241\345\274\217", nullptr));
        pushButton_3->setText(QCoreApplication::translate("TCP_server", "\346\202\224\346\243\213", nullptr));
        pushButton_4->setText(QCoreApplication::translate("TCP_server", "\346\212\225\351\231\215", nullptr));
        pushButton_5->setText(QCoreApplication::translate("TCP_server", "\346\261\202\345\222\214", nullptr));
        pushButton_6->setText(QCoreApplication::translate("TCP_server", "\346\232\202\346\227\266\351\200\200\345\207\272\345\271\266\344\277\235\345\255\230\345\257\271\345\261\200", nullptr));
        push_button->setText(QCoreApplication::translate("TCP_server", "\345\217\221\351\200\201", nullptr));
        label_7->setText(QCoreApplication::translate("TCP_server", "\345\261\200\345\237\237\347\275\221\350\201\224\346\234\272\346\250\241\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TCP_server: public Ui_TCP_server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCP_SERVER_H
