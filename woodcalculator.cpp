#include "woodcalculator.h"
#include "ui_woodcalculator.h"

struct WoodDimension {
    double beforeThickness;
    double beforeWidth;
    double afterThickness;
    double afterWidth;
};

QVector<WoodDimension> woodDimensions;

WoodCalculator::WoodCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WoodCalculator)
{
    ui->setupUi(this);

    ui->spinBox_maxPackageWidth->setVisible(false);

    penBlack.setColor(Qt::black);
    penGray.setColor(Qt::gray);

    graphicsScene_planing = new QGraphicsScene(this);
    ui->graphicsView_planing->setScene(graphicsScene_planing);

    graphicsScene_packaging = new QGraphicsScene(this);
    ui->graphicsView_packaging->setScene(graphicsScene_packaging);

    graphicsScene_gluing = new QGraphicsScene(this);
    ui->graphicsView_gluing->setScene(graphicsScene_gluing);

    calculateCuttingAmountBottom();
    calculateCuttingAmountSide();
    calculatePiecesInPacket();
    calculateGluePressure();
    calculatePressPressure();
    calculatePiecesInLine();
    calculatePackageVolume();
    calculateLinesInPackage();
    calculateGluingPressPressure();
    calculateGluedBoardWidth();
    calculateMaxPiecesInPress();
    calculatePiecesInPackage();
    calculateTotalLengthInPackage();

    updatePlaningDrawing();
    updatePackagingDrawing();
    updateGluingDrawing();

    ui->graphicsView_gluing->setVisible(false);

    grabGesture(Qt::SwipeGesture);

    woodDimensions.append( WoodDimension { 28.0, 100.0, 27.5,  97.0 } );
    woodDimensions.append( WoodDimension { 32.0, 100.0, 31.0,  95.0 } );
    woodDimensions.append( WoodDimension { 19.0, 125.0, 18.0, 122.0 } );
    woodDimensions.append( WoodDimension { 38.0, 125.0, 37.5, 122.0 } );
    woodDimensions.append( WoodDimension { 28.0, 125.0, 27.5, 122.0 } );
    woodDimensions.append( WoodDimension { 38.0, 100.0, 37.5,  98.0 } );
    woodDimensions.append( WoodDimension { 39.0, 155.0, 38.0,  74.5 } );
    woodDimensions.append( WoodDimension { 50.0, 100.0, 48.0,  98.0 } );
    woodDimensions.append( WoodDimension { 50.0, 125.0, 48.0, 122.0 } );
    woodDimensions.append( WoodDimension { 50.0, 122.0, 48.0, 119.0 } );
    woodDimensions.append( WoodDimension { 32.0, 150.0, 31.0, 147.0 } );
    woodDimensions.append( WoodDimension { 44.0,  84.0, 37.0,  79.0 } );
    woodDimensions.append( WoodDimension { 64.0, 103.5, 60.0,  49.0 } );
    woodDimensions.append( WoodDimension { 65.0, 130.0, 62.0,  62.0 } );
    woodDimensions.append( WoodDimension { 50.0, 200.0, 48.0,  62.0 } );
    woodDimensions.append( WoodDimension { 40.0,  72.0, 33.6,  27.5 } );
    woodDimensions.append( WoodDimension { 48.0,  48.0, 45.0,  45.0 } );
    woodDimensions.append( WoodDimension { 37.0,  98.0, 33.6,  27.5 } );
    woodDimensions.append( WoodDimension { 31.0, 147.0, 27.5,  27.5 } );
    woodDimensions.append( WoodDimension { 48.0,  60.0, 42.0,  56.0 } );
    woodDimensions.append( WoodDimension { 16.0,  22.0, 16.0,  22.0 } );
    woodDimensions.append( WoodDimension { 50.0, 200.0, 48.0,  65.0 } );

    QMutableVectorIterator<WoodDimension> wdi(woodDimensions);

    while (wdi.hasNext())
    {
        WoodDimension wd = wdi.next();

        ui->comboBox_woodDimensions->addItem(
                    QString::number( wd.beforeThickness ) + "x"     +
                    QString::number( wd.beforeWidth    ) + " -> "  +
                    QString::number( wd.afterThickness ) + "x"     +
                    QString::number( wd.afterWidth     )
                    );
    }
}

WoodCalculator::~WoodCalculator()
{
    delete ui;
    delete graphicsScene_packaging;
    delete graphicsScene_planing;
    delete graphicsScene_gluing;
}

void WoodCalculator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


}

void WoodCalculator::test(QString value)
{
    Q_UNUSED(value);
    qDebug("fdhfghf");
}


void WoodCalculator::on_doubleSpinBox_thicknessBeforePlaning_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateCuttingAmountBottom();
    updatePlaningDrawing();
}

void WoodCalculator::on_doubleSpinBox_widthAfterPlaning_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateCuttingAmountSide();
    calculatePlanedPieces();
    calculateGluePressure();
    calculatePressPressure();
    calculatePiecesInLine();
    calculateGluingPressPressure();
    calculateMaxPiecesInPress();
    calculateGluedBoardWidth();
    calculateTotalGlueAreaInGluing();
    updatePlaningDrawing();
}

void WoodCalculator::on_doubleSpinBox_thicknessAfterPlaning_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateCuttingAmountBottom();
    calculatePiecesInPacket();
    calculatePressPressure();
    calculateLinesInPackage();
    calculateGluingPressPressure();
    calculateMaxPiecesInPress();
    calculateGluedBoardWidth();
    calculateTotalGlueAreaInGluing();
    updatePlaningDrawing();
}

void WoodCalculator::on_doubleSpinBox_cuttingAmountBottom_valueChanged(double value)
{
    Q_UNUSED(value);
}

void WoodCalculator::calculateCuttingAmountBottom()
{
    double before = ui->doubleSpinBox_thicknessBeforePlaning->value();
    double after  = ui->doubleSpinBox_thicknessAfterPlaning->value();

    ui->doubleSpinBox_cuttingAmountBottom->setValue(0.5*(before-after));
}

int WoodCalculator::calculatePlanedPieces()
{
    double before = ui->doubleSpinBox_widthBeforePlaning->value();
    double after  = ui->doubleSpinBox_widthAfterPlaning->value();

    int pieces = 1;

    if ( after > 0.1 )
    {
        pieces = before / after;
        ui->spinBox_planedPieces->setValue(pieces);
    }

    calculateTotalPlaningWidth();

    return pieces;
}

void WoodCalculator::calculateTotalPlaningWidth()
{
    int pieces = ui->spinBox_planedPieces->value();
    double after = ui->doubleSpinBox_widthAfterPlaning->value();

    ui->doubleSpinBox_totalPlaningWidth->setValue(pieces*after+(pieces-1)*2.2);
}

void WoodCalculator::calculateThicknessAfter()
{
    ui->doubleSpinBox_thicknessAfterPlaning->setValue( ui->doubleSpinBox_thicknessBeforePlaning->value() - 2 * ui->doubleSpinBox_cuttingAmountBottom->value() );
}

void WoodCalculator::calculateCuttingAmountSide()
{
    int pieces = calculatePlanedPieces();

    double before = ui->doubleSpinBox_widthBeforePlaning->value();
    double after  = ui->doubleSpinBox_widthAfterPlaning->value();

    ui->doubleSpinBox_cuttingAmountSide->setValue( 0.5 * ( before - pieces * after - ( pieces - 1 ) * 2.2 ) );
}

void WoodCalculator::calculateWidthAfter()
{
    ui->doubleSpinBox_widthAfterPlaning->setValue( ui->doubleSpinBox_widthBeforePlaning->value() - 2 * ui->doubleSpinBox_cuttingAmountSide->value() );
}


void WoodCalculator::calculatePiecesInPacket()
{
    double boardThickness  = ui->doubleSpinBox_thicknessAfterPlaning->value();

    if(boardThickness > 0.1)
        ui->spinBox_piecesInPacket->setValue( ui->spinBox_maxWidthOfPacket->value() / boardThickness );

    calculatePacketWidth();
}

void WoodCalculator::calculatePacketWidth()
{
    ui->spinBox_widthOfPacket->setValue( qRound( ui->spinBox_piecesInPacket->value() * ui->doubleSpinBox_thicknessAfterPlaning->value() ) );
}

void WoodCalculator::calculateGluePressure()
{
    ui->doubleSpinBox_gluePressure->setValue( 0.001 * ui->doubleSpinBox_widthAfterPlaning->value() * ui->spinBox_gluePressureRatio->value() );
}

void WoodCalculator::calculatePressPressure()
{
    ui->spinBox_pressPressure->setValue( qRound( 0.01 * ui->doubleSpinBox_pressPressureRatio->value() * ui->doubleSpinBox_thicknessAfterPlaning->value() * ui->doubleSpinBox_widthAfterPlaning->value() ) );
}

int WoodCalculator::calculatePiecesInLine()
{
    double boardWidth = ui->doubleSpinBox_widthAfterPlaning->value();
    int piecesInLine = ui->spinBox_maxPackageWidth->value() / boardWidth;
    ui->spinBox_piecesInLine->setValue( piecesInLine );
    calculatePackageWidth();
    return piecesInLine;
}

void WoodCalculator::calculatePackageWidth()
{
    ui->spinBox_packageWidth->setValue( qRound( ui->spinBox_piecesInLine->value() * ui->doubleSpinBox_widthAfterPlaning->value() ) );
}

int WoodCalculator::calculateLinesInPackage()
{
    int maxLinesInPackage = calculateMaxLinesInPackage();

    int linesInPackage = 0;

    linesInPackage = maxLinesInPackage;

    ui->spinBox_linesInPackage->setValue(linesInPackage);
    calculatePackageHeight();
    return linesInPackage;
}

int WoodCalculator::calculateMaxLinesInPackage()
{
    int maxLinesInPackage = getMaxPackageHeight() / getThicknessAfterPlaning();
    ui->spinBox_maxLinesInPackage->setValue( maxLinesInPackage );
    return maxLinesInPackage;
}

void WoodCalculator::calculatePackageHeight()
{
    ui->spinBox_packageHeight->setValue( qRound( ui->spinBox_linesInPackage->value() * ui->doubleSpinBox_thicknessAfterPlaning->value() ) );
}

void WoodCalculator::calculatePackageVolume()
{
    ui->doubleSpinBox_packageVolume->setValue( 0.000000001 * ui->spinBox_packageWidth->value() * ui->spinBox_packageHeight->value() * ui->spinBox_packageLength->value() );
}

void WoodCalculator::calculatePackageMass()
{
    ui->spinBox_packageMass->setValue( 500 * ui->doubleSpinBox_packageVolume->value() );
}

int WoodCalculator::calculatePiecesInPackage()
{
    int piecesInPackage = ui->spinBox_linesInPackage->value() * ui->spinBox_piecesInLine->value();
    ui->spinBox_piecesInPackage->setValue( piecesInPackage );
    return piecesInPackage;
}

int WoodCalculator::calculateMaxPiecesInPackage()
{
    int maxPiecesInPackage = ui->spinBox_maxLinesInPackage->value() * ui->spinBox_piecesInLine->value();
    return maxPiecesInPackage;
}

void WoodCalculator::calculateTotalLengthInPackage()
{
    ui->spinBox_totalLengthInPackage->setValue( 0.001 * ui->spinBox_piecesInPackage->value() * ui->spinBox_packageLength->value());
}

void WoodCalculator::calculateGluingPressPressure()
{
    ui->spinBox_gluingPressPressure->setValue( ui->doubleSpinBox_gluingPressPressureRatio->value() * getGluedPieceWidth() );
}

void WoodCalculator::calculateMaxPiecesInPress()
{
    if(getGluedPieceWidth() > 0.1)
        ui->spinBox_maxPiecesInPress->setValue( ui->spinBox_maxWidthInPress->value() / getGluedPieceWidth() );
}

void WoodCalculator::calculateGluedBoardWidth()
{
    ui->doubleSpinBox_gluedBoardWidth->setValue( ui->spinBox_piecesGluedTogether->value() * getGluedPieceWidth() );
}

void WoodCalculator::calculateMaxGluedBoardsInPress()
{
    if ( ui->doubleSpinBox_gluedBoardWidth->value() > 0.1 )
        ui->spinBox_maxGluedBoardsInPress->setValue( ui->spinBox_maxWidthInPress->value() / ui->doubleSpinBox_gluedBoardWidth->value() );

    if ( ui->doubleSpinBox_totalGlueAreaInGluing->value() > ui->doubleSpinBox_maxTotalGlueAreaInGluing->value() )
        ui->spinBox_maxGluedBoardsInPress->setValue(
                    ui->doubleSpinBox_maxTotalGlueAreaInGluing->value()
                    /
                    (
                        ( ui->spinBox_piecesGluedTogether->value() - 1 )
                        * 0.001 * getGluedPieceThickness()
                        * 0.001 * ui->spinBox_packageLength->value()
                    )
                    );

}

void WoodCalculator::calculateTotalGlueAreaInGluing()
{
    ui->doubleSpinBox_totalGlueAreaInGluing->setValue(

                ui->spinBox_maxGluedBoardsInPress->value()
                * ( ui->spinBox_piecesGluedTogether->value() - 1 )
                * 0.001 * getGluedPieceThickness()
                * 0.001 * ui->spinBox_packageLength->value()

                );
}

void WoodCalculator::calculatePackageSizes()
{
    double totalPieces = ui->spinBox_totalPieces->value();
    double packages = calculatePackages();

    int linesInPackage;

    if ( ui->radioButton_fullPackages->isChecked() and ( packages > 1 ) )
        linesInPackage = calculateMaxLinesInPackage();
    else
    {
        double piecesInLine = getPiecesInLine();
        double totalLines = qCeil( totalPieces / piecesInLine );

        linesInPackage = qCeil( totalLines / packages );
    }

    ui->spinBox_linesInPackage->setValue( linesInPackage );
    calculatePiecesInPackage();
}

int WoodCalculator::calculatePackages()
{
    double totalPieces = ui->spinBox_totalPieces->value();
    double maxPiecesInPackage = calculateMaxPiecesInPackage();
    int packages = qCeil( totalPieces / maxPiecesInPackage );
    ui->spinBox_packages->setValue(packages);
    return packages;
}

int WoodCalculator::calculatePiecesInLastPackage()
{
    int piecesInLastPackage = getTotalPieces() - ( ( getPackages() - 1 ) * getPiecesInPackage() );
    return piecesInLastPackage;
}

int WoodCalculator::calculateLinesInLastPackage()
{
    int linesInLastPackage = qCeil( double( calculatePiecesInLastPackage() ) / double( getPiecesInLine() ) );
    return linesInLastPackage;
}

int WoodCalculator::calculatePiecesInLastLine()
{
    return ( calculatePiecesInLastPackage() - ( calculateLinesInLastPackage() - 1 ) * getPiecesInLine() );
}

double WoodCalculator::getThicknessAfterPlaning()
{
    return ui->doubleSpinBox_thicknessAfterPlaning->value();
}

int WoodCalculator::getMaxPackageHeight()
{
    return ui->spinBox_maxPackageHeight->value();
}

int WoodCalculator::getMaxLinesInPackage()
{
    return ui->spinBox_maxLinesInPackage->value();
}

int WoodCalculator::getPiecesInPackage()
{
    return ui->spinBox_piecesInPackage->value();
}

int WoodCalculator::getMaxPiecesInPackage()
{
    return getPiecesInLine() * getMaxLinesInPackage();
}

int WoodCalculator::getPackages()
{
    return ui->spinBox_packages->value();
}

int WoodCalculator::getTotalPieces()
{
    return ui->spinBox_totalPieces->value();
}

int WoodCalculator::getPiecesInLine()
{
    return ui->spinBox_piecesInLine->value();
}

int WoodCalculator::getLinesInPackage()
{
    return ui->spinBox_linesInPackage->value();
}

void WoodCalculator::on_spinBox_maxWidthOfPacket_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePiecesInPacket();
}

void WoodCalculator::on_spinBox_piecesInPacket_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePacketWidth();
}

void WoodCalculator::on_doubleSpinBox_pressPressureRatio_valueChanged(double value)
{
    Q_UNUSED(value);
    calculatePressPressure();
}


void WoodCalculator::on_spinBox_gluePressureRatio_valueChanged(int value)
{
    Q_UNUSED(value);
    calculateGluePressure();
}

void WoodCalculator::on_spinBox_widthOfPacket_valueChanged(int value)
{
    Q_UNUSED(value);
}

void WoodCalculator::on_doubleSpinBox_widthBeforePlaning_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateCuttingAmountSide();
    updatePlaningDrawing();
}

void WoodCalculator::on_doubleSpinBox_cuttingAmountSide_valueChanged(double value)
{
    Q_UNUSED(value);
}

void WoodCalculator::on_spinBox_maxPackageWidth_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePiecesInLine();
    updatePackagingDrawing();
}

void WoodCalculator::on_spinBox_piecesInLine_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageWidth();
    calculatePiecesInPackage();
    calculatePackageSizes();
    updatePackagingDrawing();
}

void WoodCalculator::on_spinBox_packageWidth_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageVolume();
}

void WoodCalculator::on_spinBox_packageHeight_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageVolume();
}

void WoodCalculator::on_spinBox_packageLength_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageVolume();
    calculateTotalGlueAreaInGluing();
    calculateTotalLengthInPackage();
}

void WoodCalculator::on_doubleSpinBox_packageVolume_valueChanged(double value)
{
    Q_UNUSED(value);
    calculatePackageMass();
}

void WoodCalculator::on_spinBox_maxPackageHeight_valueChanged(int value)
{
    Q_UNUSED(value);
    calculateMaxLinesInPackage();
    calculatePackageSizes();
}

void WoodCalculator::on_spinBox_linesInPackage_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageHeight();
    calculatePiecesInPackage();
    updatePackagingDrawing();
}

void WoodCalculator::on_spinBox_piecesInPackage_valueChanged(int value)
{
    Q_UNUSED(value);
    calculateTotalLengthInPackage();
}

void WoodCalculator::on_spinBox_maxWidthInPress_valueChanged(int value)
{
    Q_UNUSED(value);
    calculateMaxPiecesInPress();
    calculateMaxGluedBoardsInPress();
}

void WoodCalculator::on_doubleSpinBox_gluingPressPressureRatio_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateGluingPressPressure();
}

void WoodCalculator::updatePlaningDrawing()
{
    if (not ui->actionShow_drawings) return;

    double s = 3.0;

    double wb = ui->doubleSpinBox_widthBeforePlaning->value();
    double hb = ui->doubleSpinBox_thicknessBeforePlaning->value();

    QPixmap pix(900,300);
    pix.fill();
    QPainter paint(&pix);



    paint.setPen(Qt::darkGray);
    paint.setBrush(Qt::lightGray);

    paint.drawRect( 50, 10, s * wb, s * hb);

    double wa = ui->doubleSpinBox_widthAfterPlaning->value();
    double ha = ui->doubleSpinBox_thicknessAfterPlaning->value();

    paint.setPen(penBlack);
    paint.setBrush(Qt::gray);
    paint.setBrush(Qt::BDiagPattern);
    for (int i = 0; i < ui->spinBox_planedPieces->value(); i++)
        paint.drawRect( 50 + s * ( ui->doubleSpinBox_cuttingAmountSide->value() + i * ( wa + 2.2 )),
                        10 + s * ui->doubleSpinBox_cuttingAmountBottom->value(), s * wa, s * ha);

    graphicsScene_planing->addPixmap(pix);
}

void WoodCalculator::updatePackagingDrawing()
{
    if (not ui->actionShow_drawings) return;

    double s = 0.25;
    double m = 2.0;
    double w = s * ui->doubleSpinBox_widthAfterPlaning->value();
    double h = s * ui->doubleSpinBox_thicknessAfterPlaning->value();
    double nw = ui->spinBox_piecesInLine->value();

    double full_size = s * 1170;

    QPixmap pix( 2 * full_size + 10, full_size + 30 );
    pix.fill();

    QPainter paint(&pix);

    int drawings;
    if ( getPackages() > 1 )
        drawings = 2;
    else
        drawings = 1;

    for ( int d = 0 ; d < drawings ; d++ )
    {
        double nh = ui->spinBox_linesInPackage->value();
        double dx = ( 1 - d ) * ( full_size + 10);
        paint.setPen(Qt::lightGray);
        paint.setBrush(Qt::lightGray);
        if (d == 0 )
        {
            nh = calculateLinesInLastPackage() - 1;
            paint.drawRect( dx + m, full_size - ( nh + 1 ) * h - m , calculatePiecesInLastLine() * w, h);
        }
        paint.drawRect( dx + m, full_size - nh * h - m , nw * w, nh * h);

        int lines;

        double l = ui->spinBox_piecesInLine->value();
        double ll = ui->spinBox_piecesInLine->value();

        if ( d == 1 )
            if ( ll == l )
                lines = getLinesInPackage() + 1;
            else
                lines = getLinesInPackage() - 1;
        else
            if ( ll == l )
                lines = calculateLinesInLastPackage() + 1;
            else
                lines = calculateLinesInLastPackage() + 2;


        paint.setPen(Qt::darkGray);
        for (int i = 0; i <= ui->spinBox_piecesInLine->value(); i++)
            paint.drawLine( dx + m + i * w, full_size - m,
                            dx + m + i * w, full_size - m - ( lines - 1 ) * h );

        for (int i = 0; i <= ( lines - 1 ); i++)
            paint.drawLine( dx + m, full_size - m - i * h,
                            dx + m + ui->spinBox_piecesInLine->value() * w, full_size - m - i * h );

        if ( d == 0 )
        {
            for ( int i = 0 ; i <= calculatePiecesInLastLine() ; i++ )
                paint.drawLine( dx + m + i * w, full_size - m - (lines - 1) * h,
                                dx + m + i * w, full_size - m - lines * h );

            paint.drawLine( dx + m, full_size - m - lines * h,
                            dx + m + calculatePiecesInLastLine() * w, full_size - m - lines * h );

            paint.drawText( dx + 0.1 * full_size, full_size + 20, "1 nippu x " + QString::number(calculatePiecesInLastPackage()) + " kpl");


        }
        else
        {
            paint.drawText( dx + 0.1 * full_size, full_size + 20, QString::number( getPackages() - 1 ) + " nippua x " +
                            QString::number(getPiecesInPackage()) + " kpl");
        }





    }


    graphicsScene_packaging->addPixmap(pix);
}

void WoodCalculator::updateGluingDrawing()
{
    qDebug("updateGluingDrawing()");
    if (not ui->actionShow_drawings) return;

    QPixmap pix(900,300);
    pix.fill();
    QPainter paint(&pix);

    double s = 0.5;
    int n = ui->spinBox_piecesGluedTogether->value();
    int m = ui->spinBox_maxGluedBoardsInPress->value();

    double w = s * ui->doubleSpinBox_widthAfterPlaning->value();
    double h = s * ui->doubleSpinBox_thicknessAfterPlaning->value();

    double nw = n * w;

    paint.setPen(Qt::darkGray);

    paint.drawRect( 0, 0, m * nw, h );

    for ( int i = 0 ; i < m ; i++ )
    {
        paint.setPen(penBlack);
        paint.drawRect( i * nw - 2, - 2, nw + 2, h + 2 );

        for ( int j = 0 ; j < n ; j++ )
        {
            paint.setPen(Qt::lightGray);
            paint.drawRect( i * nw + 2 + j * w, 2, w - 2, h - 2 );

        }
    }

    graphicsScene_gluing->addPixmap(pix);
}


void WoodCalculator::on_comboBox_woodDimensions_currentIndexChanged(int index)
{
    ui->doubleSpinBox_thicknessBeforePlaning->setValue(woodDimensions[index].beforeThickness);
    ui->doubleSpinBox_widthBeforePlaning->setValue(woodDimensions[index].beforeWidth);

    ui->doubleSpinBox_thicknessAfterPlaning->setValue(woodDimensions[index].afterThickness);
    ui->doubleSpinBox_widthAfterPlaning->setValue(woodDimensions[index].afterWidth);

}

void WoodCalculator::on_radioButton_seamIsWidth_toggled(bool checked)
{
    Q_UNUSED(checked);
    calculateGluingPressPressure();
    calculateMaxPiecesInPress();
    calculateGluedBoardWidth();
    calculateTotalGlueAreaInGluing();
}

void WoodCalculator::on_spinBox_piecesGluedTogether_valueChanged(int value)
{
    Q_UNUSED(value);
    calculateGluedBoardWidth();
    calculateTotalGlueAreaInGluing();
    updateGluingDrawing();
}

double WoodCalculator::getGluedPieceWidth()
{
    if(ui->radioButton_seamIsWidth->isChecked())
        return ui->doubleSpinBox_widthAfterPlaning->value();
    else
        return ui->doubleSpinBox_thicknessAfterPlaning->value();
}

double WoodCalculator::getGluedPieceThickness()
{
    if(not ui->radioButton_seamIsWidth->isChecked())
        return ui->doubleSpinBox_widthAfterPlaning->value();
    else
        return ui->doubleSpinBox_thicknessAfterPlaning->value();
}

void WoodCalculator::on_doubleSpinBox_gluedBoardWidth_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateMaxGluedBoardsInPress();
}

void WoodCalculator::on_spinBox_maxGluedBoardsInPress_valueChanged(int value)
{
    Q_UNUSED(value);
    updateGluingDrawing();
    calculateTotalGlueAreaInGluing();
}

void WoodCalculator::on_doubleSpinBox_maxTotalGlueAreaInGluing_valueChanged(double value)
{
    Q_UNUSED(value);
    calculateMaxGluedBoardsInPress();
}

void WoodCalculator::on_comboBox_maxPackageWidth_currentTextChanged(const QString &text)
{
    int width = text.toInt();
    switch ( width )
    {
        case 800: width -= 3; break;
        default: width -= 2;
    }

    ui->spinBox_maxPackageWidth->setValue(width);
    calculatePackages();
}

void WoodCalculator::on_spinBox_totalPieces_valueChanged(int value)
{
    Q_UNUSED(value);
    calculatePackageSizes();
    updatePackagingDrawing();
}

void WoodCalculator::on_radioButton_fullPackages_toggled(bool checked)
{
    Q_UNUSED(checked);
    calculatePackageSizes();
}

void WoodCalculator::on_comboBox_maxPackageWidth_editTextChanged(const QString &value)
{
    Q_UNUSED(value);
}

void WoodCalculator::on_spinBox_packages_valueChanged(int value)
{
    Q_UNUSED(value);
}

void WoodCalculator::on_spinBox_maxLinesInPackage_valueChanged(int value)
{
    Q_UNUSED(value);
}
