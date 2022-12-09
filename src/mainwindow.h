#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "realtime.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
    void connectUIElements();
    void connectDayNight();

    Realtime *realtime;
    QSlider *dayNightSlider;
    QDoubleSpinBox *dayNightBox;

private slots:
    void onValChangeDayNightSlider(double newValue);
    void onValChangeDayNightBox(double newValue);
};
