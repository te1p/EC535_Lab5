#ifndef HARDWAREIO_H
#define HARDWAREIO_H

#include <QString>
#include <QFile>

// GPIOHelper: Low-level GPIO control through Linux sysfs interface
// Provides functions to export pins, set directions, and read/write values
class GPIOHelper {
public:
    // Exports a GPIO pin to userspace via /sys/class/gpio/export
    static void exportPin(int pin) {
        QFile f("/sys/class/gpio/export");
        if (f.open(QIODevice::WriteOnly)) {
            f.write(QString::number(pin).toLatin1());
            f.close();
        }
    }
    
    // Sets GPIO pin direction ("in" for input, "out" for output)
    static void setDirection(int pin, const QString &dir) {
        QFile f(QString("/sys/class/gpio/gpio%1/direction").arg(pin));
        if (f.open(QIODevice::WriteOnly)) {
            f.write(dir.toLatin1());
            f.close();
        }
    }
    
    // Writes value (0 or 1) to GPIO pin
    static void writePin(int pin, int value) {
        QFile f(QString("/sys/class/gpio/gpio%1/value").arg(pin));
        if (f.open(QIODevice::WriteOnly)) {
            f.write(QString::number(value).toLatin1());
            f.close();
        }
    }
    
    // Reads current value from GPIO pin (returns 1 or 0)
    static int readPin(int pin) {
        QFile f(QString("/sys/class/gpio/gpio%1/value").arg(pin));
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray v = f.readAll();
            f.close();
            return (v.trimmed() == "1");
        }
        return 0;
    }
};

// GPIO Pin Definitions for Hardware Buttons
// Buttons use internal pull-ups (active low: pin goes to GND when pressed)
#define BTN_LEFT_PIN    61
#define BTN_RIGHT_PIN   67
#define BTN_UP_PIN      44   
#define BTN_DOWN_PIN    65   

// GPIO Pin Definitions for LED Health Indicator
#define LED_RED_PIN     115
#define LED_YELLOW_PIN  117
#define LED_GREEN_PIN   49

// HardwareIO: High-level wrapper for button and LED control
// Handles initialization and provides simple read/write interface
class HardwareIO {
public:
    // Initializes all button GPIO pins as inputs
    static void initButtons() {
        GPIOHelper::exportPin(BTN_LEFT_PIN);
        GPIOHelper::exportPin(BTN_RIGHT_PIN);
        GPIOHelper::exportPin(BTN_UP_PIN);
        GPIOHelper::exportPin(BTN_DOWN_PIN);
        
        GPIOHelper::setDirection(BTN_LEFT_PIN, "in");
        GPIOHelper::setDirection(BTN_RIGHT_PIN, "in");
        GPIOHelper::setDirection(BTN_UP_PIN, "in");
        GPIOHelper::setDirection(BTN_DOWN_PIN, "in");
    }
    
    // Initializes all LED GPIO pins as outputs
    static void initLEDs() {
        GPIOHelper::exportPin(LED_RED_PIN);
        GPIOHelper::exportPin(LED_YELLOW_PIN);
        GPIOHelper::exportPin(LED_GREEN_PIN);
        
        GPIOHelper::setDirection(LED_RED_PIN, "out");
        GPIOHelper::setDirection(LED_YELLOW_PIN, "out");
        GPIOHelper::setDirection(LED_GREEN_PIN, "out");
    }
    
    // Sets LED states for health indicator display
    // red: true = LED on, false = LED off (same for yellow and green)
    static void setLEDState(bool red, bool yellow, bool green) {
        GPIOHelper::writePin(LED_RED_PIN, red ? 1 : 0);
        GPIOHelper::writePin(LED_YELLOW_PIN, yellow ? 1 : 0);
        GPIOHelper::writePin(LED_GREEN_PIN, green ? 1 : 0);
    }
    
    // Button read functions (returns 1 when pressed, 0 when released)
    // Note: Buttons use internal pull-ups, reads are inverted in logic
    static int readLeft()  { return GPIOHelper::readPin(BTN_LEFT_PIN); }
    static int readRight() { return GPIOHelper::readPin(BTN_RIGHT_PIN); }
    static int readUp()    { return GPIOHelper::readPin(BTN_UP_PIN); }
    static int readDown()  { return GPIOHelper::readPin(BTN_DOWN_PIN); }
};

#endif // HARDWAREIO_H
