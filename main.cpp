#include "woodcalculator.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString locale = "fi_FI";

    qputenv("QT_SCALE_FACTOR", "2");

    QString translationFileFullPath = ":/woodcalculator_" + locale;

    if(translator.load(translationFileFullPath))
        a.installTranslator(&translator);
    else if(locale != "en_EN")
        qDebug("Not found: %s", qUtf8Printable(translationFileFullPath));

    WoodCalculator w;
    w.show();
    return a.exec();
}
