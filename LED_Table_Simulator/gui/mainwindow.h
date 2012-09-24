#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ledtablewidget.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void setDisplay_(quint8 b);


signals:
    void setDisplay(int b);

private slots:

    // GUI
    void on_btnDraw_clicked();
    void runAnimation(void);
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnRandom_clicked();

private:
    Ui::MainWindow *ui;
    QString sendString;
    QTimer* aniTimer;
    quint64 startTime;
    LEDTableWidget* ledTable;
};

#endif // MAINWINDOW_H
