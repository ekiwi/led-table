#include <QDateTime>
#include <QByteArray>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "animations.h"
extern "C" {
    #include "animation_manager.h"
}

//#define DEFAULT_BAUD Baud115200
#define DEFAULT_BAUD Baud9600

MainWindow* instance_pointer;

void sendImg_extern(){
    if(instance_pointer == NULL) return;

}

void setDisplay_extern(uint8_t b, uint8_t x, uint8_t y){
    if(instance_pointer == NULL) return;
    instance_pointer->setDisplay_(b);
}

uint8_t rand_extern(){
    return qrand();
}

uint8_t audioGet_extern(uint8_t i){
    return i*20;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);
    sendString = "";
    ledTable = new LEDTableWidget(this->centralWidget());
    //connect(this, SIGNAL(sendByte(quint8)), ledTable, SLOT(receiveByte(quint8)));
    connect(ledTable, SIGNAL(newFPS(QString)), ui->statusBar, SLOT(showMessage(QString)));
    //connect(ledTable, SIGNAL(sendByte(quint8)), this, SLOT(sendCom(quint8)));
    aniTimer = new QTimer(this);
    connect(aniTimer, SIGNAL(timeout()), this, SLOT(runAnimation()));
    startTime = QDateTime::currentMSecsSinceEpoch();
    connect(this, SIGNAL(setDisplay(int)), this->ui->lcdNumber, SLOT(display(int)));


    // connect "C" and C++ parts
    instance_pointer = this;
    set_display = &setDisplay_extern;
    uart_img_send = &sendImg_extern;
    led_rand = &rand_extern;
    audio_get = &audioGet_extern;

    animation_manager_init();

    // load default animation
    animation_manager_load_animation(START_ANIMATION);

    // start animation
    this->on_btnDraw_clicked();
}


void MainWindow::runAnimation(){
    quint32 time_elapsed = QDateTime::currentMSecsSinceEpoch() - startTime;
    animation_manager_run(time_elapsed);
    for(int y = 0; y < img_buffer.width; y++){
        for(int x = 0; x < img_buffer.height; x++){
            for(int c = 0; c < 3; c++){
                ledTable->receiveByte(img_buffer.raw_data[x][y][c]);
            }
        }
    }
}


void MainWindow::on_btnDraw_clicked(){
    if(aniTimer->isActive()){
        aniTimer->stop();
        ui->btnDraw->setText("Start");
    }
    else {
        aniTimer->start(1000/50);   // 50 fps
        ui->btnDraw->setText("Stop");
    }
}


void MainWindow::setDisplay_(quint8 b){
    setDisplay(b);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_btnLeft_clicked(){
    animation_manager_dec_animation();
}

void MainWindow::on_btnRight_clicked(){
    animation_manager_inc_animation();
}

void MainWindow::on_btnRandom_clicked(){
    animation_manager_random_animation();
}


