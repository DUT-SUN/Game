#include "fupan.h"
#include "ui_fupan.h"


fupan::fupan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fupan)
{
    ui->setupUi(this);
    this->setWindowTitle("复盘");


    //获取图片与id
connect(&manager,&QNetworkAccessManager::finished,this,&fupan::httpResponse);

connect(&manager2, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));


QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:8080/uploadchess"));
    manager.get(request); //发出get请求
}

#include<QFile>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>



void fupan::httpResponse(QNetworkReply *reply)
{
     QByteArray  strJson=reply->readAll() ;


     QJsonDocument  document;

     QJsonParseError error;
     document = QJsonDocument::fromJson(strJson, &error);

     vecPic.clear();


     if ( document.isArray()) {
        QJsonArray  array =  document.array();
         for (int i = 0; i < array.size(); i++) {
              QJsonObject object = array.at(i).toObject();
              QJsonObject obj2= object.value("dataValues").toObject();
               int id = obj2.value("id").toInt();
               QString avater = obj2.value("avater").toString();
               qDebug()<<id << "    "<<avater;

               ItemPic  pic;
               pic.id = id;
               pic.url = avater;
               vecPic.push_back(pic);
         }
     }


     if(vecPic.size()>0)
     {
         curIndex=0;
         showPic(curIndex);
     }


}

fupan::~fupan()
{
    delete ui;
}
void fupan::setMainUi(QWidget *parent)
{
    par = parent;
}

void fupan::showPic(int index)
{

       if(index < vecPic.size())
       {

            auto  pic= vecPic.at(index);
            ui->lineEdit->setText(QString("%1").arg(pic.id));

            QNetworkRequest request;
            request.setUrl(QUrl( pic.url ));
            manager2.get(request);
       }

}
void fupan::replyFinished(QNetworkReply *reply)
{

    // 获取响应状态码，200表示正常
    QVariant nCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        int s = bytes.size();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);

        QPixmap pix2 = pixmap.scaled(ui->labelPic->width(), ui->labelPic->height(), Qt::KeepAspectRatio);
        ui->labelPic->setPixmap(pix2);

    }
    else
    {
        // 错误处理-显示错误信息，或显示上一次缓存的图片或叉图。
    }
}

//void QWidget::closeEvent ( QCloseEvent * event )
//{
//    emit(ReturnToMenu());
//}

void fupan::on_pushButton_3_clicked()
{
    this->hide();
    par->show();
}

void fupan::on_pushButtonUp_clicked()
{

    if(curIndex>0)
    {
           curIndex--;
           showPic(curIndex);
    }
}

void fupan::on_pushButtonDown_clicked()
{
    if(curIndex<vecPic.size()-1)
    {
           curIndex++;
           showPic(curIndex);
    }
}



void fupan::on_pushButton_clicked()
{
    QHttpMultiPart *mMultiPart= new QHttpMultiPart (QHttpMultiPart::FormDataType);
    QHttpPart dataPart;
    dataPart.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
    dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"id\""));
    int id=ui->lineEdit->text().toInt();
   QString str = tr("%1").arg(id);
    QByteArray bytes = str.toUtf8();
    dataPart.setBody( bytes );
    mMultiPart->append(dataPart);
   QNetworkRequest request;
   request.setUrl(QUrl("http://localhost:8080/deletechess" ));
   manager4.post(request,mMultiPart);

}
