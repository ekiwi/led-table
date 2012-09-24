#ifndef LEDTABLEWIDGET_H
#define LEDTABLEWIDGET_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>

#define LED_TABLE_SIM_MODE 0
#define LED_TABLE_UART_MODE 1

#define MS_IMAGE_UPDATE 2   // the time span the simulated display update takes

// Protocol Constans

#define SOURCE_CONNECT 0x01
#define SOURCE_IMAGE_HEADER 0x05
#define SOURCE_IMAGE_FOOTER 0x07

#define TABLE_CONNECT 0x81
#define TABLE_IMAGE_HEADER 0x85
#define TABLE_IMAGE_FOOTER 0x87
#define TABLE_DONE_DISPLAYING_IMAGE 0x89


class LEDTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LEDTableWidget(QWidget *parent = 0);

signals:
    void newFPS(QString fps);
    void sendByte(quint8 b);

public slots:
    void receiveByte(quint8 b);
    void reset();
    void setMode(int m);
    void imageUpdated();

protected:
    void paintEvent(QPaintEvent *event);



private:

    int receiveImageByte(quint8 b);
    enum {
        ST_NOT_CONNECTED,
        ST_WAITING_FOR_IMAGE_HEADER,
        ST_RECEIVING_IMAGE,
        ST_WAITING_FOR_IMAGE_FOOTER,
        ST_UPDATING_IMAGE

    } receive_state;
    QTimer* updateTimer;
    int mode;
    QVector<QVector<QColor> > pixels;
    int ledWidth;
    int ledHeight;
    int ledSide;
    int receiveX;
    int receiveY;
    int receiveColor; // 0=R, 1=G, 2=B
    QElapsedTimer* fpsTimer;




};

#endif // LEDTABLEWIDGET_H
