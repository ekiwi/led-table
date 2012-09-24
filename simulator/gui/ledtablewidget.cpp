#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include "ledtablewidget.h"
#include "led_graphics/led_graphics.h"

LEDTableWidget::LEDTableWidget(QWidget *parent) :
    QWidget(parent)
{
    fpsTimer = new QElapsedTimer;
    fpsTimer->start();
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(imageUpdated()));
    mode = LED_TABLE_SIM_MODE;
    ledWidth = LED_WIDTH;
    ledHeight = LED_HEIGHT;
    ledSide = 50;
    receiveX = 0;
    receiveY = 0;
    receiveColor = 0;
    for(int x = 0; x < ledWidth; x++){
        QVector<QColor> column(ledHeight, Qt::black);
        pixels.append(column);
    }
    setFixedSize(ledWidth * ledSide + 1, ledHeight * ledSide + 1);
   // QRect rec;
}

void LEDTableWidget::reset(){
    receiveX = 0;
    receiveY = 0;
    receiveColor = 0;
    for(int x = 0; x < LED_WIDTH; x++){
        for(int y = 0; y < LED_HEIGHT; y++){
            pixels[x][y].setRgb(0,0,0);
        }
    }
    repaint();
}

void LEDTableWidget::paintEvent(QPaintEvent *event){
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    for(int y = 0; y < ledHeight; y++){
        for(int x = 0; x < ledWidth; x++){
            painter.setBrush(QBrush(pixels[x][y]));
            painter.drawRect(x*ledSide,y*ledSide,ledSide, ledSide);
        }
    }
    painter.end();
}

void LEDTableWidget::imageUpdated(){
    sendByte(TABLE_DONE_DISPLAYING_IMAGE);
    receive_state = ST_WAITING_FOR_IMAGE_HEADER;
    qDebug() << "Done displaying image!";
}

void LEDTableWidget::receiveByte(quint8 b){
    if(mode == LED_TABLE_SIM_MODE){ // no sync needed for simulation
        receiveImageByte(b);
    }
    else {  // UART Sysnc!
        //qDebug() << "Received: " << b << endl;
        switch(receive_state){
        case ST_NOT_CONNECTED:
            if(b == SOURCE_CONNECT){    // if connection request ist received
                sendByte(TABLE_CONNECT);     // answer and
                receive_state = ST_WAITING_FOR_IMAGE_HEADER;    // start waiting for image
            }
            break;
        case ST_WAITING_FOR_IMAGE_HEADER:
            if(b == SOURCE_IMAGE_HEADER){   // if a new image is announced
                sendByte(TABLE_IMAGE_HEADER);     // answer
                receive_state = ST_RECEIVING_IMAGE;
            }
            break;
        case ST_RECEIVING_IMAGE:
            if(receiveImageByte(b)){
                receive_state = ST_WAITING_FOR_IMAGE_FOOTER;
            }
            break;
        case ST_WAITING_FOR_IMAGE_FOOTER:
            qDebug() << "ST_WAITING_FOR_IMAGE_FOOTER";
            if(b == SOURCE_IMAGE_FOOTER){
                sendByte(TABLE_IMAGE_HEADER);     // answer
                updateTimer->start(MS_IMAGE_UPDATE);
                receive_state = ST_UPDATING_IMAGE; // in reality the image is already updated, but here we will simulate a timeout because of updatign the display
            }
            break;
        case ST_UPDATING_IMAGE: // sould not get here !!! (the µC should not send any data while is waiting for the image to be displayd)
            qDebug() << "ST_UPDATING_IMAGE";
            break;
        }
    }
}



int LEDTableWidget::receiveImageByte(quint8 b){
    int done = 0;
    switch(receiveColor){
    case 0:
        pixels[receiveX][receiveY].setRed(b);
        break;
    case 1:
        pixels[receiveX][receiveY].setGreen(b);
        break;
    case 2:
        pixels[receiveX][receiveY].setBlue(b);
        break;
    }
    receiveColor++;
    if(receiveColor == 3){
        receiveColor = 0;
        receiveX++;
    }
    if(receiveX == ledWidth){
        receiveX = 0;
        receiveY++;
    }
    if(receiveY == ledHeight){
        receiveY = 0;
        float fps = 1000.0 / fpsTimer->elapsed();
        QString fpsString = QString("FPS: %1").arg((fps));
        newFPS(fpsString);
        fpsTimer->restart();
        qDebug() << "Successfully received picture!!" << endl;
        repaint();
        done = 1;
    }
    return done;
}

void LEDTableWidget::setMode(int m){
    if(m == LED_TABLE_UART_MODE){
        mode = LED_TABLE_UART_MODE;
        receive_state = ST_NOT_CONNECTED;
    }
    else {
        mode = LED_TABLE_SIM_MODE;
    }
    reset();
}
