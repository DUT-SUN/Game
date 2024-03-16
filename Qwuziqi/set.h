#ifndef SET_H
#define SET_H

#include <QWidget>
#include"GameModel.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#include <QTimer>
#include <QSlider>
#include"fupan.h"
namespace Ui {
class set;
}

class set : public QWidget
{
    Q_OBJECT

public:
    explicit set(QWidget *parent = nullptr);
    ~set();
    void setMainUi(QWidget *parent);
    QWidget *par;
    bool setSysVolume(int level);
        int sysVolume();
private:
    Ui::set *ui;
     GameModel*game;
     fupan f;
//     fupan*x;
private slots:
    void on_pushButton_clicked();

//    void on_horizontalSlider_valueChanged( QSlider *x,int value);
    void on_pushButton_2_clicked();
};

#endif // SET_H
