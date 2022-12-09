#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

void MainWindow::initialize() {
    realtime = new Realtime;

    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(realtime, 1);
    this->setLayout(hLayout);

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QLabel *skybox_label = new QLabel();
    skybox_label->setText("Skybox");
    skybox_label->setFont(font);

    QGroupBox *dayNightLayout = new QGroupBox();
    QHBoxLayout *lDayNight = new QHBoxLayout();

    dayNightSlider = new QSlider(Qt::Orientation::Horizontal);
    dayNightSlider->setTickInterval(1);
    dayNightSlider->setMinimum(0);
    dayNightSlider->setMaximum(100);
    dayNightSlider->setValue(0);

    dayNightBox = new QDoubleSpinBox();
    dayNightBox->setMinimum(0.f);
    dayNightBox->setMaximum(1.f);
    dayNightBox->setSingleStep(0.1f);
    dayNightBox->setValue(0.f);

    lDayNight->addWidget(dayNightSlider);
    lDayNight->addWidget(dayNightBox);
    dayNightLayout->setLayout(lDayNight);

    vLayout->addWidget(skybox_label);
    vLayout->addWidget(dayNightLayout);

    connectUIElements();

    // Set default values
    onValChangeDayNightBox(0.f);
}

void MainWindow::finish() {
    realtime->finish();
    delete(realtime);
}

void MainWindow::connectUIElements() {
    connectDayNight();
}

void MainWindow::connectDayNight() {
    connect(dayNightSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeDayNightSlider);
    connect(dayNightBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeDayNightBox);
}

void MainWindow::onValChangeDayNightSlider(double newValue) {
    dayNightBox->setValue(newValue/100.f);
    settings.dayNight = dayNightBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeDayNightBox(double newValue) {
    dayNightSlider->setValue(int(newValue*100.f));
    settings.dayNight = dayNightBox->value();
    realtime->settingsChanged();
}
