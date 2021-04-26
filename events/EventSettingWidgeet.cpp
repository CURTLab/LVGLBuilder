#include "EventSettingWidgeet.h"

#include "ui_EventSettingWidgeet.h"

EventSettingWidgeet::EventSettingWidgeet(QWidget *parent)
    : QDialog(parent), ui(new Ui::EventSettingWidgeet) {
  ui->setupUi(this);
  ui->value2comb->hide();
  ui->value2lab->hide();
}

EventSettingWidgeet::~EventSettingWidgeet() { delete ui; }
