#ifndef WOODCALCULATOR_H
#define WOODCALCULATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QVector>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class WoodCalculator; }
QT_END_NAMESPACE

class WoodCalculator : public QMainWindow
{
    Q_OBJECT

public:
    WoodCalculator(QWidget *parent = nullptr);
    ~WoodCalculator();

    virtual void paintEvent(QPaintEvent *event);

    Q_INVOKABLE void test(QString value);

private slots:
    void on_doubleSpinBox_thicknessBeforePlaning_valueChanged(double value);
    void on_doubleSpinBox_widthBeforePlaning_valueChanged(double value);

    void on_doubleSpinBox_thicknessAfterPlaning_valueChanged(double value);
    void on_doubleSpinBox_widthAfterPlaning_valueChanged(double value);

    void on_doubleSpinBox_cuttingAmountBottom_valueChanged(double value);
    void on_doubleSpinBox_cuttingAmountSide_valueChanged(double value);

    void on_spinBox_maxWidthOfPacket_valueChanged(int value);
    void on_spinBox_piecesInPacket_valueChanged(int value);
    void on_spinBox_widthOfPacket_valueChanged(int value);

    void on_spinBox_gluePressureRatio_valueChanged(int value);

    void on_doubleSpinBox_pressPressureRatio_valueChanged(double value);

    void on_spinBox_maxPackageWidth_valueChanged(int value);

    void on_spinBox_piecesInLine_valueChanged(int value);

    void on_spinBox_packageWidth_valueChanged(int value);

    void on_spinBox_packageHeight_valueChanged(int value);

    void on_spinBox_packageLength_valueChanged(int value);

    void on_doubleSpinBox_packageVolume_valueChanged(double value);

    void on_spinBox_maxPackageHeight_valueChanged(int value);

    void on_spinBox_linesInPackage_valueChanged(int value);

    void on_spinBox_piecesInPackage_valueChanged(int value);

    void on_spinBox_maxWidthInPress_valueChanged(int value);

    void on_doubleSpinBox_gluingPressPressureRatio_valueChanged(double value);

    //void on_comboBox_beforePlaning_currentIndexChanged(int index);

    void on_comboBox_woodDimensions_currentIndexChanged(int index);

    void on_radioButton_seamIsWidth_toggled(bool checked);

    void on_spinBox_piecesGluedTogether_valueChanged(int arg1);

    void on_doubleSpinBox_gluedBoardWidth_valueChanged(double arg1);

    void on_spinBox_maxGluedBoardsInPress_valueChanged(int arg1);

    void updatePlaningDrawing();
    void updatePackagingDrawing();
    void updateGluingDrawing();

    void on_doubleSpinBox_maxTotalGlueAreaInGluing_valueChanged(double arg1);

    void on_comboBox_maxPackageWidth_currentTextChanged(const QString &arg1);

    void on_spinBox_totalPieces_valueChanged(int arg1);

    void on_radioButton_fullPackages_toggled(bool checked);

    void on_comboBox_maxPackageWidth_editTextChanged(const QString &arg1);

    void on_spinBox_packages_valueChanged(int arg1);

    void on_spinBox_maxLinesInPackage_valueChanged(int arg1);

private:
    Ui::WoodCalculator *ui;
    QGraphicsScene *graphicsScene_planing;
    QGraphicsScene *graphicsScene_packaging;
    QGraphicsScene *graphicsScene_gluing;
    QPen penBlack, penGray;
    double getGluedPieceWidth();
    double getGluedPieceThickness();
    void calculateCuttingAmountBottom();
    int calculatePlanedPieces();
    void calculateTotalPlaningWidth();
    void calculateThicknessAfter();
    void calculateCuttingAmountSide();
    void calculateWidthAfter();
    void calculatePiecesInPacket();
    void calculatePacketWidth();
    void calculateGluePressure();
    void calculatePressPressure();
    int calculatePiecesInLine();
    void calculatePackageWidth();
    int calculateLinesInPackage();
    int calculateMaxLinesInPackage();
    void calculatePackageHeight();
    void calculatePackageVolume();
    void calculatePackageMass();
    int calculatePiecesInPackage();
    int calculateMaxPiecesInPackage();
    void calculateTotalLengthInPackage();
    void calculateGluingPressPressure();
    void calculateMaxPiecesInPress();
    void calculateGluedBoardWidth();
    void calculateMaxGluedBoardsInPress();
    void calculateTotalGlueAreaInGluing();
    void calculatePackageSizes();
    int calculatePackages();
    int calculatePiecesInLastPackage();
    int calculateLinesInLastPackage();
    int calculatePiecesInLastLine();
    double getThicknessAfterPlaning();
    int getMaxPackageHeight();
    int getMaxLinesInPackage();
    int getPiecesInPackage();
    int getMaxPiecesInPackage();
    int getPackages();
    int getTotalPieces();
    int getPiecesInLine();
    int getLinesInPackage();
};
#endif // WOODCALCULATOR_H
