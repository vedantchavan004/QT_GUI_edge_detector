#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Slider_thresolding->hide();
    ui->label_6_thresold->hide();
    ui->slider_lowThreshold->hide();
    ui->label_6_thresold_low->hide();
    ui->slider_highThreshold->hide();
    ui->label_6_thresold_high->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));
    if (!file_name.isEmpty()){
        //QMessageBox::information(this, "...", file_name);
        QImage img(file_name);
        QPixmap pix = QPixmap::fromImage(img);

        int w = ui->label_2_pic->width();
        int h = ui->label_2_pic->height();

        ui->label_2_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

        unsigned int cols = img.width();
        unsigned int rows = img.height();
        unsigned int numBlackPixels = 0;
        QVector<QVector<int>> imgArray(rows, QVector<int>(cols, 0));

        ui->dimension_values->setText(QString::fromStdString("W: " + std::to_string(cols) + "  H: " + std::to_string(rows)));

        // Convert to grayscale
        grayImg = QImage(cols, rows, QImage::Format_Grayscale8);
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                int gray = qGray(img.pixel(x, y));
                grayImg.setPixel(x, y, qRgb(gray, gray, gray));
            }
        }
        // Edge detection using a simple Sobel filter
        /*edgeImg = QImage(cols, rows, QImage::Format_Grayscale8);

        for (unsigned int y = 1; y < rows - 1; y++) {
            for (unsigned int x = 1; x < cols - 1; x++) {
                int gx = -qGray(grayImg.pixel(x - 1, y - 1)) - 2 * qGray(grayImg.pixel(x - 1, y)) - qGray(grayImg.pixel(x - 1, y + 1))
                + qGray(grayImg.pixel(x + 1, y - 1)) + 2 * qGray(grayImg.pixel(x + 1, y)) + qGray(grayImg.pixel(x + 1, y + 1));

                int gy = -qGray(grayImg.pixel(x - 1, y - 1)) - 2 * qGray(grayImg.pixel(x, y - 1)) - qGray(grayImg.pixel(x + 1, y - 1))
                         + qGray(grayImg.pixel(x - 1, y + 1)) + 2 * qGray(grayImg.pixel(x, y + 1)) + qGray(grayImg.pixel(x + 1, y + 1));

                int magnitude = qBound(0, static_cast<int>(std::sqrt(gx * gx + gy * gy)), 255);
                edgeImg.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
            }
        }

        */
        // Display edge image
        //QPixmap pix_ = QPixmap::fromImage(edgeImg);

        int w_ = ui->label_edge_pic->width();
        int h_ = ui->label_edge_pic->height();

        //ui->label_edge_pic->setPixmap(pix_.scaled(w_,h_,Qt::KeepAspectRatio));

        ui->dimension_values->setText(QString("W: %1  H: %2").arg(cols).arg(rows));



    }
}

void MainWindow::applySobel(int threshold)
{
    edgeImg = QImage(grayImg.width(), grayImg.height(), QImage::Format_Grayscale8);

    for (int y = 1; y < grayImg.height() - 1; ++y) {
        for (int x = 1; x < grayImg.width() - 1; ++x) {
            int gx = -qGray(grayImg.pixel(x - 1, y - 1)) - 2 * qGray(grayImg.pixel(x - 1, y)) - qGray(grayImg.pixel(x - 1, y + 1))
            + qGray(grayImg.pixel(x + 1, y - 1)) + 2 * qGray(grayImg.pixel(x + 1, y)) + qGray(grayImg.pixel(x + 1, y + 1));

            int gy = -qGray(grayImg.pixel(x - 1, y - 1)) - 2 * qGray(grayImg.pixel(x, y - 1)) - qGray(grayImg.pixel(x + 1, y - 1))
                     + qGray(grayImg.pixel(x - 1, y + 1)) + 2 * qGray(grayImg.pixel(x, y + 1)) + qGray(grayImg.pixel(x + 1, y + 1));

            int val = std::sqrt(gx * gx + gy * gy);
            int binary = (val >= threshold) ? 255 : 0;
            edgeImg.setPixel(x, y, qRgb(binary, binary, binary));
        }
    }

    ui->label_edge_pic->setPixmap(QPixmap::fromImage(edgeImg).scaled(ui->label_edge_pic->width(), ui->label_edge_pic->height(), Qt::KeepAspectRatio));
}

void MainWindow::applyCanny(int low, int high)
{
    cv::Mat src(grayImg.height(), grayImg.width(), CV_8UC1, (void*)grayImg.bits(), grayImg.bytesPerLine());
    cv::Mat edges;
    cv::Canny(src, edges, low, high);

    QImage result(edges.data, edges.cols, edges.rows, static_cast<int>(edges.step), QImage::Format_Grayscale8);
    QImage safeCopy = result.copy();
    ui->label_edge_pic->setPixmap(QPixmap::fromImage(safeCopy).scaled(ui->label_edge_pic->width(), ui->label_edge_pic->height(), Qt::KeepAspectRatio));
}


void MainWindow::on_Slider_thresolding_actionTriggered(int action)
{

    Q_UNUSED(action);

    int threshold = ui->Slider_thresolding->value();
    ui->label_6_thresold->setText(QString(" %1").arg(threshold));

    if (currentMethod == "Sobel" && !grayImg.isNull()) {
        applySobel(threshold);
    }

}


void MainWindow::on_comboBox_edgeMethod_currentTextChanged(const QString &method)
{
    currentMethod = method;

    if (grayImg.isNull()) return;

    if (method == "Sobel") {
        applySobel(ui->Slider_thresolding->value());

        // Show Sobel slider & label
        ui->Slider_thresolding->show();
        ui->label_6_thresold->show();

        // Hide Canny sliders & labels
        ui->slider_lowThreshold->hide();
        ui->slider_highThreshold->hide();
        ui->label_6_thresold_low->hide();
        ui->label_6_thresold_high->hide();
    }
    else if (method == "Canny") {
        applyCanny(ui->slider_lowThreshold->value(), ui->slider_highThreshold->value());

        // Show Canny sliders & labels
        ui->slider_lowThreshold->show();
        ui->slider_highThreshold->show();
        ui->label_6_thresold_low->show();
        ui->label_6_thresold_high->show();

        // Hide Sobel slider & label
        ui->Slider_thresolding->hide();
        ui->label_6_thresold->hide();
    }
    else {
        // None selected — hide all
        ui->Slider_thresolding->hide();
        ui->label_6_thresold->hide();
        ui->slider_lowThreshold->hide();
        ui->label_6_thresold_low->hide();
        ui->slider_highThreshold->hide();
        ui->label_6_thresold_high->hide();

        ui->label_edge_pic->clear();
    }
}


void MainWindow::on_slider_lowThreshold_valueChanged(int value)
{
    ui->label_6_thresold_low->setText(QString("low %1").arg(value));
    if (currentMethod == "Canny" && !grayImg.isNull()) {
        applyCanny(value, ui->slider_highThreshold->value());
    }

}


void MainWindow::on_slider_highThreshold_valueChanged(int value)
{
    ui->label_6_thresold_high->setText(QString("high %1").arg(value));
    if (currentMethod == "Canny" && !grayImg.isNull()) {
        applyCanny(ui->slider_lowThreshold->value(), value);
    }

}

