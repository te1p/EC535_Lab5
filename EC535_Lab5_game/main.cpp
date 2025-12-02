#include "mainwindow.h"
#include "score.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QImage>
#include <QBrush>
#include <QStackedWidget>
#include <QLayout>
#include <QComboBox>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // QWidget *firstPageWidget = new QWidget;
    // QWidget *secondPageWidget = new QWidget;
    // QWidget *thirdPageWidget = new QWidget;

    // QStackedWidget *stackedWidget = new QStackedWidget;
    // stackedWidget->addWidget(firstPageWidget);
    // stackedWidget->addWidget(secondPageWidget);
    // stackedWidget->addWidget(thirdPageWidget);

    // QVBoxLayout *layout = new QVBoxLayout;
    // layout->addWidget(stackedWidget);
    // setLayout(layout);

    // QComboBox *pageComboBox = new QComboBox;
    // pageComboBox->addItem("Page 1");
    // pageComboBox->addItem("Page 2");
    // pageComboBox->addItem("Page 3");
    // connect(pageComboBox, QOverload<int>::of(&QComboBox::activated),
    //         stackedWidget, &QStackedWidget::setCurrentIndex);
    return a.exec();
}
