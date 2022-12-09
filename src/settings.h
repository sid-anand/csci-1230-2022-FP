#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    float dayNight = 0;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
