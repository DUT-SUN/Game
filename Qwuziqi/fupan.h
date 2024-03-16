#ifndef FUPAN_H
#define FUPAN_H

#include <QWidget>
#include <QVector>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkAccessManager>

namespace Ui {
class fupan;
}

struct ItemPic
{
    int id;
    QString url;
};

class fupan : public QWidget
{
    Q_OBJECT
//signals:
//    void ReturnToMenu();                   // 返回主菜单的信号
public:
    explicit fupan(QWidget *parent = nullptr);
    ~fupan();
    void setMainUi(QWidget *parent);
    QWidget *par;

    void  showPic(int index);

private slots:
    void on_pushButton_3_clicked();
void  replyFinished(QNetworkReply *reply);
    void  httpResponse(QNetworkReply * reply);
    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();


    void on_pushButton_clicked();

private:
    Ui::fupan *ui;
    QNetworkAccessManager  manager;
    QNetworkAccessManager  manager2;
        QNetworkAccessManager  manager4;
    QVector<ItemPic> vecPic;
    int curIndex=0;
};

#endif // FUPAN_H
