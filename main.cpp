#include "woodcalculator.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString locale = "fi_FI";

    qputenv("QT_SCALE_FACTOR", "2");

    /*a.setStyleSheet("QSpinBox { border: 3px solid red; border-radius: 5px; background-color: yellow; min-width: 200px; min-height: 200px; }"
                    "QDoubleSpinBox { border: 3px solid red; border-radius: 5px; background-color: yellow; min-width: 200px; min-height: 200px; }");*/

    /*a.setStyleSheet("QSpinBox       { min-height: 100px; }"
                    "QDoubleSpinBox { min-height: ´100px; }");*/
    // "QSpinBox { padding-right: 15px; border-image: url(:/images/frame.png) 4; border-width: 3; }"

    /*
    a.setStyleSheet("QSpinBox, QDoubleSpinBox { height: 70px; }"
                    "QSpinBox::up-button, QDoubleSpinBox::up-button { background: green; subcontrol-origin: border; subcontrol-position: right; "
                    "width: 32px; height: 21px; border: 1px solid green   ; }"
                    "QSpinBox::down-button, QDoubleSpinBox::down-button          { background: red; subcontrol-origin: border; subcontrol-position: left; "
                    "width: 32px; height: 21px; border: 1px solid red     ; }");*/

    //locale = "en_EN";

    QString translationFileFullPath = ":/woodcalculator_" + locale;

    if(translator.load(translationFileFullPath))
        a.installTranslator(&translator);
    else if(locale != "en_EN")
        qDebug("Not found: %s", qUtf8Printable(translationFileFullPath));

    WoodCalculator w;
    w.show();
    return a.exec();
}
