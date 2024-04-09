#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

extern "C" {
#include "../modules/s21_rpn.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void Addtext();
  void on_pushButton_15_clicked();
  void on_pushButton_11_clicked();
  void on_pushButton_clicked();
  void print_rpn(struct node_list *root);
  void on_pushButton_32_clicked();
  void on_pushButton_33_clicked();
  void on_pushButton_35_clicked();
  void on_tableWidget_2_cellDoubleClicked(int row, int column);
  void on_pushButton_34_clicked();

 private:
  Ui::MainWindow *ui;
  double checkTable(QDate date_st, QDate date_finish);
};
#endif  // MAINWINDOW_H
