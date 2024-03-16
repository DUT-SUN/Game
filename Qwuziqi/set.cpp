#include "set.h"
#include "ui_set.h"

#include <QDebug>


set::set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::set)
{
      f.setMainUi(this);
this->setWindowTitle("设置");
    ui->setupUi(this);
//    ui->horizontalSlider->setMinimum(0);
//    ui->horizontalSlider->setMaximum(100);
//    //设置滑动条控件的值
//    ui->horizontalSlider->setValue(50);

    //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本


    QSlider *pSliderValume = new QSlider(Qt::Horizontal,this);
      pSliderValume-> setSingleStep(10);

    pSliderValume->setGeometry(210, 120, 160, 22);
    pSliderValume->setValue(sysVolume());  //CWindows.h
    connect(pSliderValume,&QSlider::valueChanged,[=](int nVal){
        setSysVolume(nVal); //CWindows.h
    });
     connect(pSliderValume, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(pSliderValume,int)));
     connect(ui->spinBox,SIGNAL(valueChanged(int)),pSliderValume,SLOT(setValue(int)));
     connect(pSliderValume,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
         ui->spinBox->setValue(50);

    pSliderValume->setMaximum(100);
    pSliderValume->setMinimum(-1);

    /*监听系统音量变化*/
    QTimer *pTimerListenVolume = new QTimer(this);
    pTimerListenVolume->start(10000);
    connect(pTimerListenVolume,&QTimer::timeout,[=]{
        int nSysValume = sysVolume();
        if(pSliderValume->value() != nSysValume)
            pSliderValume->setValue(nSysValume);
});
}



set::~set()
{
    delete ui;
}
void set::setMainUi(QWidget *parent)
{
    par = parent;
}

void set::on_pushButton_clicked()
{
    this->hide();
    par->show();
}


//void set::on_horizontalSlider_valueChanged( QSlider *x,int value)
//{
//       qDebug() << "值变";
//    int pos = x->value();
//    QString str = QString("%1").arg(pos);
//ui->lineEdit->setText(str);

//}


//参数:
//	-2 恢复静音
//	-1 静音
//	0~100:音量比例
bool set:: setSysVolume(int level)
{
  HRESULT hr;
  IMMDeviceEnumerator* pDeviceEnumerator=0;
  IMMDevice* pDevice=0;
  IAudioEndpointVolume* pAudioEndpointVolume=0;
  IAudioClient* pAudioClient=0;

  try{
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
    if(FAILED(hr)) throw "CoCreateInstance";
    hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
    if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
    if(FAILED(hr)) throw "pDevice->Active";
    hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
    if(FAILED(hr)) throw "pDevice->Active";

    if(level==-2){
      hr = pAudioEndpointVolume->SetMute(FALSE,NULL);
      if(FAILED(hr)) throw "SetMute";
    }else if(level==-1){
      hr = pAudioEndpointVolume->SetMute(TRUE,NULL);
      if(FAILED(hr)) throw "SetMute";
    }else{
      if(level<0 || level>100){
        hr = E_INVALIDARG;
        throw "Invalid Arg";
      }

      float fVolume;
      fVolume = level/100.0f;
      hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume,&GUID_NULL);
      if(FAILED(hr)) throw "SetMasterVolumeLevelScalar";

      pAudioClient->Release();
      pAudioEndpointVolume->Release();
      pDevice->Release();
      pDeviceEnumerator->Release();
      return true;
    }
  }
  catch(...){
    if(pAudioClient) pAudioClient->Release();
    if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
    if(pDevice) pDevice->Release();
    if(pDeviceEnumerator) pDeviceEnumerator->Release();
    throw;
  }
  return false;
}


int set:: sysVolume()
{
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;
    IAudioClient* pAudioClient=0;

    try{
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
        if(FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
        if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
        if(FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
        if(FAILED(hr)) throw "pDevice->Active";

        float fVolume;

        hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);

        if(FAILED(hr)) throw "SetMasterVolumeLevelScalar";

        pAudioClient->Release();
        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        int  intVolume = fVolume*100+1;
        if(fVolume>100)
        {
            fVolume =100;
        }
        return intVolume;

    }

    catch(...){
        if(pAudioClient) pAudioClient->Release();
        if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if(pDevice) pDevice->Release();
        if(pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
}

void set::on_pushButton_2_clicked()
{
//     fupan* x = new fupan;
//    connect(x,&fupan::ReturnToMenu,this,[=]()   // 绑定 GameControl 中的返回信号
//    {
//        x->close();
//        this->show();
//    },Qt::QueuedConnection);
    this->hide();
    f.show();
}
