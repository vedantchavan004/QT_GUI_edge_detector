#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_Slider_thresolding_actionTriggered(int action);
    void applySobel(int threshold);
    void applyCanny(int low, int high);
    void on_comboBox_edgeMethod_currentTextChanged(const QString &method);
    void on_slider_lowThreshold_valueChanged(int value);
    void on_slider_highThreshold_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage grayImg;

    QImage edgeImg;
    QString currentMethod;

};
#endif // MAINWINDOW_H
