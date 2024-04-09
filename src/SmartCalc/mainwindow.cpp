#include "mainwindow.h"

#include <iostream>

#include "../modules/s21_rpn.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->dateEdit->setDate(QDate::currentDate());
  ui->tableWidget_3->resizeColumnsToContents();
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_13, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_14, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_16, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_17, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_18, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_19, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_22, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_23, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_24, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_25, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_26, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_27, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_28, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_29, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_30, SIGNAL(clicked()), this, SLOT(Addtext()));
  connect(ui->pushButton_31, SIGNAL(clicked()), this, SLOT(Addtext()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Addtext() {
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  QString str = button->text();
  if (str.length() > 1 && str != "mod") str = str + "(";
  ui->lineEdit->setText(ui->lineEdit->text() + str);
}

void MainWindow::on_pushButton_15_clicked() {
  QString str = ui->lineEdit->text();
  if (str.endsWith("mod", Qt::CaseInsensitive)) {
    str.chop(3);
  } else if (str.endsWith("(", Qt::CaseInsensitive)) {
    str.chop(1);
    if (str.endsWith("ln", Qt::CaseInsensitive)) {
      str.chop(2);
    } else if (str.endsWith("asin", Qt::CaseInsensitive) ||
               str.endsWith("acos", Qt::CaseInsensitive) ||
               str.endsWith("atan", Qt::CaseInsensitive) ||
               str.endsWith("sqrt", Qt::CaseInsensitive)) {
      str.chop(4);
    } else if (str.endsWith("sin", Qt::CaseInsensitive) ||
               str.endsWith("cos", Qt::CaseInsensitive) ||
               str.endsWith("tan", Qt::CaseInsensitive) ||
               str.endsWith("log", Qt::CaseInsensitive)) {
      str.chop(3);
    }
  } else if (str.length() > 0) {
    str.chop(1);
  }
  ui->lineEdit->setText(str);
}

void MainWindow::on_pushButton_11_clicked() {
  QString str = ui->lineEdit->text();
  str.replace(" ", "");
  int diff = str.count("(") - str.count(")");
  if (diff > 0)
    for (int i = 0; i < diff; i++) str += ")";
  ui->lineEdit->setText(str);
  std::string str2 = str.toStdString();
  const char* strc = str2.c_str();
  struct node_list* rpn_list = rpn(strc);
  if (rpn_list == NULL) {
    ui->label->setText("Error");
  } else {
    double x = ui->doubleSpinBox->value();
    int error = 0;
    ui->label->setText(
        QString::number(rpn_calculate(rpn_list, x, &error), 'f', 7));
    if (error != 0) {
      ui->label->setText("Error");
    }
  }
  list_destroy(rpn_list);
}

void MainWindow::on_pushButton_clicked() {
  ui->lineEdit->setText("");
  ui->label->setText("");
  ui->widget->removeGraph(0);
  ui->widget->replot();
}

void MainWindow::print_rpn(struct node_list* root) {
  struct node_list* iter = root->next;
  qDebug("->");
  while (iter != NULL) {
    if (iter->value >= 0) {
      qDebug("%.8f", iter->value);
    } else {
      char c = (int)round(-iter->value);
      qDebug("%c", c);
    }
    iter = iter->next;
  }
}

void MainWindow::on_pushButton_32_clicked() {
  QString str = ui->lineEdit->text();
  str.replace(" ", "");
  int diff = str.count("(") - str.count(")");
  if (diff > 0)
    for (int i = 0; i < diff; i++) str += ")";
  ui->lineEdit->setText(str);
  std::string str2 = str.toStdString();
  const char* strc = str2.c_str();
  struct node_list* rpn_list = rpn(strc);
  if (rpn_list == NULL) {
    ui->label->setText("Error");
  } else {
    ui->label->setText("Chart");
    QVector<double> x, y;
    int x1 = ui->spinBox->value();
    int x2 = ui->spinBox_2->value();
    int y1 = ui->spinBox_3->value();
    int y2 = ui->spinBox_4->value();
    int error = 0;
    double res = 0;
    double step = ((double)x2 - x1) / 1000;
    for (int i = 0; i < 1000; i++) {
      error = 0;
      res = rpn_calculate(rpn_list, x1 + i * step, &error);
      double last = 0;
      if (y.count() > 0) last = y.last();
      if (error == 0) {
        if (fabs(res - last) > 10 * (y2 - y1)) {
          x.append(x1 + i * step);
          y.append(0.0 / 0.0);
        } else {
          x.append(x1 + i * step);
          y.append(res);
        }
      }
    }
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->xAxis->setRange(ui->spinBox->value(), ui->spinBox_2->value());
    ui->widget->yAxis->setRange(ui->spinBox_3->value(), ui->spinBox_4->value());
    ui->widget->replot();
  }
}

void MainWindow::on_pushButton_33_clicked() {
  QDate date = QDate::currentDate();
  QString cur = " " + ui->comboBox_4->currentText();
  double proc = ui->doubleSpinBox_2->value() / 1200;
  int mounth = ui->spinBox_5->value();
  if (ui->comboBox_3->currentIndex() == 1) mounth *= 12;
  double amount = ui->lineEdit_2->text().toDouble();
  ui->tableWidget->setRowCount(0);
  if (amount > 0 && !std::isnan(amount) && !std::isinf(amount)) {
    double payment = amount * (proc + proc / (pow(1 + proc, mounth) - 1));
    double rest = amount, mounth_proc;
    double total_proc = amount * proc * mounth;
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setRowCount(mounth + 1);
    for (int i = 1; i <= mounth; i++) {
      date = date.addMonths(1);
      QString str = QLocale(QLocale::Russian).toString(date, "MMM yyyy");
      ui->tableWidget->setItem(i - 1, 0, new QTableWidgetItem(str));
      mounth_proc = rest * proc;
      ui->tableWidget->setItem(
          i - 1, 2,
          new QTableWidgetItem(QString::number(mounth_proc, 'f', 2) + cur));
      if (ui->comboBox->currentIndex() == 0) {
        rest -= payment - mounth_proc;
        ui->tableWidget->setItem(
            i - 1, 1,
            new QTableWidgetItem(QString::number(payment, 'f', 2) + cur));
        ui->tableWidget->setItem(
            i - 1, 3,
            new QTableWidgetItem(
                QString::number(payment - mounth_proc, 'f', 2) + cur));
      } else {
        payment = amount / mounth;
        rest -= payment;
        ui->tableWidget->setItem(
            i - 1, 1,
            new QTableWidgetItem(
                QString::number(payment + mounth_proc, 'f', 2) + cur));
        ui->tableWidget->setItem(
            i - 1, 3,
            new QTableWidgetItem(QString::number(payment, 'f', 2) + cur));
      }
      ui->tableWidget->setItem(
          i - 1, 4,
          new QTableWidgetItem(QString::number(fabs(rest), 'f', 2) + cur));
    }
    ui->tableWidget->setItem(mounth, 0,
                             new QTableWidgetItem("Общая выплата :"));
    ui->tableWidget->setItem(
        mounth, 1,
        new QTableWidgetItem(QString::number(total_proc + amount, 'f', 2) +
                             cur));
    ui->tableWidget->setItem(mounth, 3, new QTableWidgetItem("Переплата :"));
    ui->tableWidget->setItem(
        mounth, 4,
        new QTableWidgetItem(QString::number(total_proc, 'f', 2) + cur));
    QFont font;
    font.setBold(true);
    ui->tableWidget->item(mounth, 0)->setFont(font);
    ui->tableWidget->item(mounth, 1)->setFont(font);
    ui->tableWidget->item(mounth, 3)->setFont(font);
    ui->tableWidget->item(mounth, 4)->setFont(font);
    ui->tableWidget->resizeColumnsToContents();
  }
}

void MainWindow::on_pushButton_35_clicked() {
  QString cur = " " + ui->comboBox_6->currentText();
  ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount() + 1);
  ui->tableWidget_2->setItem(
      ui->tableWidget_2->rowCount() - 1, 0,
      new QTableWidgetItem(ui->comboBox_9->currentText()));
  ui->tableWidget_2->setItem(
      ui->tableWidget_2->rowCount() - 1, 1,
      new QTableWidgetItem(ui->comboBox_8->currentText()));
  ui->tableWidget_2->setItem(
      ui->tableWidget_2->rowCount() - 1, 2,
      new QTableWidgetItem(ui->dateEdit->date().toString("dd.MM.yyyy")));
  ui->tableWidget_2->setItem(
      ui->tableWidget_2->rowCount() - 1, 3,
      new QTableWidgetItem(ui->lineEdit_4->text() + cur));
  ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount() - 1, 4,
                             new QTableWidgetItem("Удалить"));
  ui->tableWidget_2->resizeColumnsToContents();
}

void MainWindow::on_tableWidget_2_cellDoubleClicked(int row, int column) {
  if (column == 4) {
    ui->tableWidget_2->removeRow(row);
  }
}

void MainWindow::on_pushButton_34_clicked() {
  QString cur = " " + ui->comboBox_6->currentText();
  QDate date_start = QDate::currentDate();
  QDate date_finish, date_st = date_start, date_start_period = date_start,
                     date_end_period;
  double dep_amount = ui->lineEdit_3->text().toDouble();
  double percent_all = 0, percent_period = 0;
  double proc = ui->doubleSpinBox_3->value() / 100;
  double delta = 0;
  ui->tableWidget_3->setRowCount(0);
  int mounth;
  if (ui->comboBox_5->currentIndex() == 0) {
    date_finish = date_start.addMonths(ui->spinBox_6->value());
    mounth = ui->spinBox_6->value();
  } else {
    date_finish = date_start.addYears(ui->spinBox_6->value());
    mounth = 12 * ui->spinBox_6->value();
  }
  while (date_st < date_finish) {
    date_st = date_st.addDays(1);
    percent_period += dep_amount * proc / date_st.daysInYear();
    delta = checkTable(date_st, date_finish);
    switch (ui->comboBox_2->currentIndex()) {
      case 0:
        date_end_period = date_start_period.addDays(1);
        break;
      case 1:
        date_end_period = date_start_period.addDays(7);
        break;
      case 2:
        date_end_period = date_start_period.addMonths(1);
        break;
      case 3:
        date_end_period = date_start_period.addMonths(3);
        break;
      case 4:
        date_end_period = date_start_period.addMonths(6);
        break;
      case 5:
        date_end_period = date_start_period.addYears(1);
        break;
      case 6:
        date_end_period = date_finish;
        break;
    }
    if (date_st == date_end_period ||
        (date_st == date_finish && percent_period > 0) || delta != 0) {
      dep_amount += delta;
      ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount() + 1);
      ui->tableWidget_3->setItem(
          ui->tableWidget_3->rowCount() - 1, 0,
          new QTableWidgetItem(date_st.toString("dd.MM.yyyy")));
      if (date_st == date_end_period ||
          (date_st == date_finish && percent_period > 0)) {
        if (ui->comboBox_7->currentIndex() == 0) {
          dep_amount += percent_period;
        }
        ui->tableWidget_3->setItem(
            ui->tableWidget_3->rowCount() - 1, 1,
            new QTableWidgetItem(QString::number(percent_period, 'f', 2) +
                                 cur));
        percent_all += percent_period;
        percent_period = 0;
        date_start_period = date_st;
      }
      if (delta != 0)
        ui->tableWidget_3->setItem(
            ui->tableWidget_3->rowCount() - 1, 2,
            new QTableWidgetItem(QString::number(delta, 'f', 2) + cur));
      ui->tableWidget_3->setItem(
          ui->tableWidget_3->rowCount() - 1, 3,
          new QTableWidgetItem(QString::number(dep_amount, 'f', 2) + cur));
      ui->tableWidget_3->resizeColumnsToContents();
    }
  }
  ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount() + 2);
  ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount() - 2, 0,
                             new QTableWidgetItem("Начисленные проценты :"));
  ui->tableWidget_3->setItem(
      ui->tableWidget_3->rowCount() - 2, 1,
      new QTableWidgetItem(QString::number(percent_all, 'f', 2) + cur));
  ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount() - 2, 2,
                             new QTableWidgetItem("Сумма на вкладе :"));
  ui->tableWidget_3->setItem(
      ui->tableWidget_3->rowCount() - 2, 3,
      new QTableWidgetItem(QString::number(dep_amount, 'f', 2) + cur));
  ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount() - 1, 0,
                             new QTableWidgetItem("Сумма налога :"));
  double nalog = percent_all - 42500;
  if (nalog < 0) nalog = 0;
  nalog = nalog * ui->doubleSpinBox_4->value() * mounth / 1200;
  ui->tableWidget_3->setItem(
      ui->tableWidget_3->rowCount() - 1, 1,
      new QTableWidgetItem(QString::number(nalog, 'f', 2) + cur));
  QFont font;
  font.setBold(true);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 2, 0)->setFont(font);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 2, 1)->setFont(font);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 2, 2)->setFont(font);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 2, 3)->setFont(font);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 1, 0)->setFont(font);
  ui->tableWidget_3->item(ui->tableWidget_3->rowCount() - 1, 1)->setFont(font);
  ui->tableWidget_3->resizeColumnsToContents();
}

double MainWindow::checkTable(QDate date_st, QDate date_finish) {
  double delta = 0;
  int change_dep = 0;
  for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
    QDate d1 =
        QDate::fromString(ui->tableWidget_2->item(i, 2)->text(), "dd.MM.yyyy");
    if (ui->tableWidget_2->item(i, 1)->text() == ui->comboBox_8->itemText(0)) {
      if (d1 == date_st) change_dep = 1;
    } else if (ui->tableWidget_2->item(i, 1)->text() ==
               ui->comboBox_8->itemText(1)) {
      while (d1 <= date_finish) {
        if (d1 == date_st) change_dep = 1;
        d1 = d1.addMonths(1);
      }
    } else if (ui->tableWidget_2->item(i, 1)->text() ==
               ui->comboBox_8->itemText(2)) {
      while (d1 <= date_finish) {
        if (d1 == date_st) change_dep = 1;
        d1 = d1.addMonths(2);
      }
    } else if (ui->tableWidget_2->item(i, 1)->text() ==
               ui->comboBox_8->itemText(3)) {
      while (d1 <= date_finish) {
        if (d1 == date_st) change_dep = 1;
        d1 = d1.addMonths(3);
      }
    } else if (ui->tableWidget_2->item(i, 1)->text() ==
               ui->comboBox_8->itemText(4)) {
      while (d1 <= date_finish) {
        if (d1 == date_st) change_dep = 1;
        d1 = d1.addMonths(4);
      }
    } else if (ui->tableWidget_2->item(i, 1)->text() ==
               ui->comboBox_8->itemText(5)) {
      while (d1 <= date_finish) {
        if (d1 == date_st) change_dep = 1;
        d1 = d1.addYears(1);
      }
    }
    if (change_dep == 1) {
      QString delta_str = ui->tableWidget_2->item(i, 3)->text();
      delta_str.chop(2);
      if (ui->tableWidget_2->item(i, 0)->text() ==
          ui->comboBox_9->itemText(1)) {
        delta -= delta_str.toDouble();
      } else {
        delta += delta_str.toDouble();
      }
      change_dep = 0;
    }
  }
  return delta;
}
