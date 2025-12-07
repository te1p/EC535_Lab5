#ifndef HARDWAREIO_H
#define HARDWAREIO_H

#include <QString>
#include <QFile>

// GPIO Helper
class GPIOHelper {
public:
    static void exportPin(int pin) {
        QFile f("/sys/class/gpio/export");
        if (f.open(QIODevice::WriteOnly)) {
            f.write(QString::number(pin).toLatin1());
            f.close();
        }
    }

    static void setDirection(int pin, const QString &dir) {
        QFile f(QString("/sys/class/gpio/gpio%1/direction").arg(pin));
        if (f.open(QIODevice::WriteOnly)) {
            f.write(dir.toLatin1());
            f.close();
        }
    }

    static void writePin(int pin, int value) {
        QFile f(QString("/sys/class/gpio/gpio%1/value").arg(pin));
        if (f.open(QIODevice::WriteOnly)) {
            f.write(QString::number(value).toLatin1());
            f.close();
        }
    }

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

// Supported Inputs (Left, Right, Up, Down)
#define BTN_LEFT_PIN    61
#define BTN_RIGHT_PIN   67
#define BTN_UP_PIN      44   
#define BTN_DOWN_PIN    65   

// LEDs
#define LED_RED_PIN     115
#define LED_YELLOW_PIN  117
#define LED_GREEN_PIN   49

// Hardware IO Wrapper
class HardwareIO {
public:

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

    static void initLEDs() {
        GPIOHelper::exportPin(LED_RED_PIN);
        GPIOHelper::exportPin(LED_YELLOW_PIN);
        GPIOHelper::exportPin(LED_GREEN_PIN);

        GPIOHelper::setDirection(LED_RED_PIN, "out");
        GPIOHelper::setDirection(LED_YELLOW_PIN, "out");
        GPIOHelper::setDirection(LED_GREEN_PIN, "out");
    }

    static void setLEDState(bool red, bool yellow, bool green) {
        GPIOHelper::writePin(LED_RED_PIN, red ? 1 : 0);
        GPIOHelper::writePin(LED_YELLOW_PIN, yellow ? 1 : 0);
        GPIOHelper::writePin(LED_GREEN_PIN, green ? 1 : 0);
    }

    // Inverted read = button pressed = returns TRUE
    static int readLeft()  { return GPIOHelper::readPin(BTN_LEFT_PIN); }
    static int readRight() { return GPIOHelper::readPin(BTN_RIGHT_PIN); }
    static int readUp()    { return GPIOHelper::readPin(BTN_UP_PIN); }
    static int readDown()  { return GPIOHelper::readPin(BTN_DOWN_PIN); }
};

#endif // HARDWAREIO_H