#include "MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSortFilterProxyModel>

#include "LVGLDialog.h"
#include "LVGLFontData.h"
#include "LVGLFontDialog.h"
#include "LVGLHelper.h"
#include "LVGLItem.h"
#include "LVGLNewDialog.h"
#include "LVGLObjectModel.h"
#include "LVGLProject.h"
#include "LVGLPropertyModel.h"
#include "LVGLSimulator.h"
#include "LVGLStyleModel.h"
#include "LVGLWidgetListView.h"
#include "LVGLWidgetModel.h"
#include "LVGLWidgetModelDisplay.h"
#include "LVGLWidgetModelInput.h"
#include "ListDelegate.h"
#include "ListViewItem.h"
#include "TabWidget.h"
#include "lvgl/lvgl.h"
#include "ui_MainWindow.h"
#include "widgets/LVGLWidgets.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_zoom_slider(new QSlider(Qt::Horizontal)),
      m_project(nullptr),
      m_maxFileNr(5),
      m_curSimulation(nullptr),
      m_proxyModel(nullptr),
      m_objectModel(nullptr),
      m_proxyModelDPW(nullptr),
      m_proxyModelIPW(nullptr),
      m_widgetModel(nullptr),
      m_widgetModelDPW(nullptr),
      m_widgetModelIPW(nullptr),
      m_filter(nullptr),
      m_curTabWIndex(-1),
      m_frun(true),
      m_isrun(false) {
  m_ui->setupUi(this);
  m_ui->style_tree->setStyleSheet(
      "QTreeView::item{border:1px solid "
      "#f2f2f2;}QTreeView::item::hover{color:black;}QTreeView::item:selected{"
      "border:1px solid #567dbc;color:black;}");
  m_ui->property_tree->setStyleSheet(
      "QTreeView::item{border:1px solid "
      "#f2f2f2;}QTreeView::item::hover{color:black;}QTreeView::item:selected{"
      "border:1px solid #567dbc;color:black;}");
  m_propertyModel = new LVGLPropertyModel(this);
  m_ld1 = new ListDelegate(m_ui->list_widgets->getlistview());
  m_ld2 = new ListDelegate(m_ui->list_widgets_2->getlistview());
  m_ld3 = new ListDelegate(m_ui->list_widgets_3->getlistview());

  m_ui->property_tree->setModel(m_propertyModel);
  m_ui->property_tree->setItemDelegate(new LVGLPropertyDelegate(this));
  m_ui->button_remove_image->setEnabled(false);
  m_ui->button_remove_font->setEnabled(false);

  m_ui->object_tree->setEditTriggers(QAbstractItemView::DoubleClicked |
                                     QAbstractItemView::CurrentChanged);
  m_ui->style_tree->setEditTriggers(QAbstractItemView::DoubleClicked |
                                    QAbstractItemView::CurrentChanged);

  m_zoom_slider->setRange(-2, 2);

  m_ui->statusbar->addPermanentWidget(m_zoom_slider);

  m_styleModel = new LVGLStyleModel(this);
  connect(m_styleModel, &LVGLStyleModel::styleChanged, this,
          &MainWindow::styleChanged);
  m_ui->style_tree->setModel(m_styleModel);
  m_ui->style_tree->setItemDelegate(
      new LVGLStyleDelegate(m_styleModel->styleBase()));
  m_ui->style_tree->expandAll();

  connect(m_ui->action_new, &QAction::triggered, this,
          &MainWindow::openNewProject);

  // recent configurations
  QAction *recentFileAction = nullptr;
  for (int i = 0; i < m_maxFileNr; i++) {
    recentFileAction = new QAction(this);
    recentFileAction->setVisible(false);
    connect(recentFileAction, &QAction::triggered, this,
            &MainWindow::loadRecent);
    m_recentFileActionList.append(recentFileAction);
    m_ui->menu_resent_filess->addAction(recentFileAction);
  }
  updateRecentActionList();

  // add style editor dock to property dock and show the property dock
  tabifyDockWidget(m_ui->PropertyEditor, m_ui->ObjecInspector);
  m_ui->PropertyEditor->raise();
  m_ui->StyleEditor->raise();

  // add font editor dock to image dock and show the image dock
  tabifyDockWidget(m_ui->ImageEditor, m_ui->FontEditor);
  m_ui->ImageEditor->raise();
  m_liststate << LV_STATE_DEFAULT << LV_STATE_CHECKED << LV_STATE_FOCUSED
              << LV_STATE_EDITED << LV_STATE_HOVERED << LV_STATE_PRESSED
              << LV_STATE_DISABLED;
  connect(m_ui->tabWidget, &QTabWidget::currentChanged, this,
          &MainWindow::tabChanged);

  LVGLHelper::getInstance().setMainW(this);
}

MainWindow::~MainWindow() {
  int n = m_ui->list_images->count();
  for (int i = 0; i < n; ++i) {
    auto item = m_ui->list_images->item(0);
    delete item;
  }
  delete m_ui;
  qDeleteAll(m_widgets);
  qDeleteAll(m_widgetsDisplayW);
  qDeleteAll(m_widgetsInputW);
  for (int i = 0; i < 35; ++i)
    if (m_codemap.contains(i)) lv_obj_del(m_codemap[i]);
}

LVGLSimulator *MainWindow::simulator() const { return m_curSimulation; }

void MainWindow::updateProperty() {
  LVGLObject *o = m_curSimulation->selectedObject();
  if (o == nullptr) return;
  LVGLProperty *p = o->widgetClass()->property("Geometry");
  if (p == nullptr) return;

  for (int i = 0; i < p->count(); ++i) {
    auto index = m_propertyModel->propIndex(p->child(i), o->widgetClass(), 1);
    emit m_propertyModel->dataChanged(index, index);
  }
}

void MainWindow::setCurrentObject(LVGLObject *obj) {
  m_ui->combo_style->clear();
  m_ui->combo_state->setCurrentIndex(0);
  m_propertyModel->setObject(obj);
  if (obj) {
    auto parts = obj->widgetClass()->parts();
    m_styleModel->setPart(parts[0]);
    m_styleModel->setState(LV_STATE_DEFAULT);
    m_styleModel->setObj(obj->obj());
    m_ui->combo_style->addItems(obj->widgetClass()->styles());
    m_styleModel->setStyle(obj->style(0, 0),
                           obj->widgetClass()->editableStyles(0));
    updateItemDelegate();
  } else {
    m_styleModel->setStyle(nullptr);
  }
}

void MainWindow::styleChanged() {
  LVGLObject *obj = m_curSimulation->selectedObject();
  if (obj) {
    int index = m_ui->combo_style->currentIndex();
    obj->widgetClass()->setStyle(obj->obj(), index, obj->style(index));
    // refresh_children_style(obj->obj());
    // lv_obj_refresh_style(obj->obj());
  }
}

void MainWindow::loadRecent() {
  QAction *action = qobject_cast<QAction *>(QObject::sender());
  if (action == nullptr) return;
  loadProject(action->data().toString());
}

void MainWindow::openNewProject() {
  LVGLNewDialog dialog(this);
  m_isrun = true;
  if (dialog.exec() == QDialog::Accepted) {
    if (m_curSimulation != nullptr) revlvglConnect();
    TabWidget *tabw = new TabWidget(dialog.selectName(), this);
    lvgl = tabw->getCore();
    const auto res = dialog.selectedResolution();
    lvgl->init(res.width(), res.height());
    if (m_frun) {
      m_frun = false;
      initNewWidgets();
      initcodemap();
    }

    lvgl->initw(m_widgets);
    lvgl->initwDP(m_widgetsDisplayW);
    lvgl->initwIP(m_widgetsInputW);

    m_coreRes[lvgl] = res;
    m_listTabW.push_back(tabw);
    m_ui->tabWidget->addTab(tabw, tabw->getName());
    m_ui->tabWidget->setCurrentIndex(m_listTabW.size() - 1);

    setEnableBuilder(true);
    m_curSimulation->clear();
    m_project = tabw->getProject();
    m_project->setres(res);
    lvgl->changeResolution(res);
  } else {
    if (m_ui->tabWidget->count() == 0) setEnableBuilder(false);
    setWindowTitle("LVGL Builder");
    LVGLHelper::getInstance().reduceFileindex();
  }
  m_isrun = false;
}

void MainWindow::addImage(LVGLImageData *img, QString name) {
  LVGLImageDataCast cast;
  cast.ptr = img;
  for (int i = 0; i < m_ui->list_images->count(); ++i)
    if (m_ui->list_images->item(i)->text() == name) return;

  QListWidgetItem *item = new QListWidgetItem(img->icon(), name);
  item->setData(Qt::UserRole + 3, cast.i);
  m_ui->list_images->addItem(item);
}

void MainWindow::updateImages() {
  m_ui->list_images->clear();
  for (int index = 0; index < m_ui->tabWidget->count(); ++index) {
    auto tabW = static_cast<TabWidget *>(m_ui->tabWidget->widget(index));
    auto tmplvgl = tabW->getCore();
    for (LVGLImageData *i : tmplvgl->images()) {
      if (i->fileName().isEmpty()) continue;
      QString name = QFileInfo(i->fileName()).baseName() +
                     QString(" [%1x%2]").arg(i->width()).arg(i->height());
      addImage(i, name);
    }
  }
}

void MainWindow::addFont(LVGLFontData *font, QString name) {
  LVGLFontDataCast cast;
  cast.ptr = font;
  for (int i = 0; i < m_ui->list_fonts->count(); ++i)
    if (m_ui->list_fonts->item(i)->text() == name) return;
  QListWidgetItem *item = new QListWidgetItem(name);
  item->setData(Qt::UserRole + 3, cast.i);
  m_ui->list_fonts->addItem(item);
}

void MainWindow::updateFonts() {
  m_ui->list_fonts->clear();
  for (int index = 0; index < m_ui->tabWidget->count(); ++index) {
    auto tabW = static_cast<TabWidget *>(m_ui->tabWidget->widget(index));
    auto tmplvgl = tabW->getCore();
    for (const LVGLFontData *f : tmplvgl->customFonts())
      addFont(const_cast<LVGLFontData *>(f), f->name());
  }
}

void MainWindow::updateRecentActionList() {
  QSettings settings("at.fhooe.lvgl", "LVGL Builder");
  QStringList recentFilePaths;
  for (const QString &f : settings.value("recentFiles").toStringList()) {
    if (QFile(f).exists()) recentFilePaths.push_back(f);
  }

  int itEnd = m_maxFileNr;
  if (recentFilePaths.size() <= m_maxFileNr) itEnd = recentFilePaths.size();

  for (int i = 0; i < itEnd; i++) {
    QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
    m_recentFileActionList.at(i)->setText(strippedName);
    m_recentFileActionList.at(i)->setData(recentFilePaths.at(i));
    m_recentFileActionList.at(i)->setVisible(true);
  }

  for (int i = itEnd; i < m_maxFileNr; i++)
    m_recentFileActionList.at(i)->setVisible(false);
}

void MainWindow::adjustForCurrentFile(const QString &fileName) {
  QSettings settings("at.fhooe.lvgl", "LVGL Builder");
  QStringList recentFilePaths = settings.value("recentFiles").toStringList();
  recentFilePaths.removeAll(fileName);
  recentFilePaths.prepend(fileName);
  while (recentFilePaths.size() > m_maxFileNr) recentFilePaths.removeLast();
  settings.setValue("recentFiles", recentFilePaths);

  updateRecentActionList();
}

void MainWindow::loadProject(const QString &fileName) {
  delete m_project;
  m_project = nullptr;
  m_curSimulation->clear();
  revlvglConnect();
  initlvglConnect();
  int index = m_ui->tabWidget->currentIndex();
  auto tab = static_cast<TabWidget *>(m_ui->tabWidget->widget(index));
  tab->setProject(LVGLProject::load(fileName));
  m_project = tab->getProject();
  setWindowTitle("LVGL Builder");
  if (m_project == nullptr) {
    QMessageBox::critical(this, "Error", "Could not load lvgl file!");
    if (m_ui->tabWidget->count() == 0) setEnableBuilder(false);
  } else {
    m_ui->tabWidget->setTabText(index, m_project->name());
    adjustForCurrentFile(fileName);
    lvgl->changeResolution(m_project->resolution());
    m_curSimulation->changeResolution(m_project->resolution());
    setEnableBuilder(true);
  }
  updateImages();
  updateFonts();
}

void MainWindow::setEnableBuilder(bool enable) {
  m_ui->action_save->setEnabled(enable);
  m_ui->action_export_c->setEnabled(enable);
  m_ui->action_run->setEnabled(enable);

  m_ui->WidgeBox->setEnabled(enable);
  m_ui->ImageEditor->setEnabled(enable);
  m_ui->FontEditor->setEnabled(enable);
}

void MainWindow::updateItemDelegate() {
  auto it = m_ui->style_tree->itemDelegate();
  if (nullptr != it) delete it;
  m_ui->style_tree->setItemDelegate(
      new LVGLStyleDelegate(m_styleModel->styleBase()));
}

void MainWindow::on_action_load_triggered() {
  QString path;
  if (m_project != nullptr) path = m_project->fileName();
  QString fileName =
      QFileDialog::getOpenFileName(this, "Load lvgl", path, "LVGL (*.lvgl)");
  if (fileName.isEmpty()) return;
  loadProject(fileName);
}

void MainWindow::on_action_save_triggered() {
  QString path;
  if (m_project != nullptr) path = m_project->fileName();
  QString fileName =
      QFileDialog::getSaveFileName(this, "Save lvgl", path, "LVGL (*.lvgl)");
  if (fileName.isEmpty()) return;
  if (!m_project->save(fileName)) {
    QMessageBox::critical(this, "Error", "Could not save lvgl file!");
  } else {
    adjustForCurrentFile(fileName);
  }
}

void MainWindow::on_combo_style_currentIndexChanged(int index) {
  LVGLObject *obj = m_curSimulation->selectedObject();
  if (obj && (index >= 0)) {
    auto parts = obj->widgetClass()->parts();
    m_styleModel->setState(m_liststate[m_ui->combo_state->currentIndex()]);
    m_styleModel->setPart(parts[index]);
    m_styleModel->setStyle(
        obj->style(index, m_ui->combo_state->currentIndex()),
        obj->widgetClass()->editableStyles(m_ui->combo_style->currentIndex()));
    m_ui->combo_state->setCurrentIndex(0);
    on_combo_state_currentIndexChanged(0);
  }
}

void MainWindow::on_action_export_c_triggered() {
  QString dir;
  if (m_project != nullptr) {
    QFileInfo fi(m_project->fileName());
    dir = fi.absoluteFilePath();
  }
  QString path = QFileDialog::getExistingDirectory(this, "Export C files", dir);
  if (path.isEmpty()) return;
  if (m_project->exportCode(path))
    QMessageBox::information(this, "Export", "C project exported!");
}

void MainWindow::on_button_add_image_clicked() {
  QString dir;
  if (m_project != nullptr) {
    QFileInfo fi(m_project->fileName());
    dir = fi.absoluteFilePath();
  }
  QStringList fileNames = QFileDialog::getOpenFileNames(
      this, "Import image", dir, "Image (*.png *.jpg *.bmp *.jpeg)");
  for (const QString &fileName : fileNames) {
    QImage image(fileName);
    if (image.isNull()) continue;
    if (image.width() >= 2048 || image.height() >= 2048) {
      QMessageBox::critical(
          this, "Error Image Size",
          tr("Image size must be under 2048! (Src: '%1')").arg(fileName));
      continue;
    }

    QString name = QFileInfo(fileName).baseName();
    LVGLImageData *i = lvgl->addImage(fileName, name);
    name += QString(" [%1x%2]").arg(i->width()).arg(i->height());
    addImage(i, name);
  }
}

void MainWindow::on_button_remove_image_clicked() {
  QListWidgetItem *item = m_ui->list_images->currentItem();
  if (item == nullptr) return;
  const int row = m_ui->list_images->currentRow();

  LVGLImageDataCast cast;
  cast.i = item->data(Qt::UserRole + 3).toLongLong();

  if (lvgl->removeImage(cast.ptr)) m_ui->list_images->takeItem(row);
}

void MainWindow::on_list_images_customContextMenuRequested(const QPoint &pos) {
  QPoint item = m_ui->list_images->mapToGlobal(pos);
  QListWidgetItem *listItem = m_ui->list_images->itemAt(pos);
  if (listItem == nullptr) return;

  QMenu menu;
  QAction *save = menu.addAction("Save as ...");
  QAction *color = menu.addAction("Set output color ...");
  QAction *sel = menu.exec(item);
  if (sel == save) {
    LVGLImageDataCast cast;
    cast.i = listItem->data(Qt::UserRole + 3).toLongLong();

    QStringList options({"C Code (*.c)", "Binary (*.bin)"});
    QString selected;
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save image as c file", cast.ptr->codeName(), options.join(";;"),
        &selected);
    if (fileName.isEmpty()) return;
    bool ok = false;
    if (selected == options.at(0))
      ok = cast.ptr->saveAsCode(fileName);
    else if (selected == options.at(1))
      ok = cast.ptr->saveAsBin(fileName);
    if (!ok) {
      QMessageBox::critical(this, "Error",
                            tr("Could not save image '%1'").arg(fileName));
    }
  } else if (sel == color) {
    LVGLImageDataCast cast;
    cast.i = listItem->data(Qt::UserRole + 3).toLongLong();
    int index = static_cast<int>(cast.ptr->colorFormat());
    QString ret =
        QInputDialog::getItem(this, "Output color", "Select output color",
                              LVGLImageData::colorFormats(), index, false);
    index = LVGLImageData::colorFormats().indexOf(ret);
    if (index >= 0)
      cast.ptr->setColorFormat(static_cast<LVGLImageData::ColorFormat>(index));
  }
}

void MainWindow::on_list_images_currentItemChanged(QListWidgetItem *current,
                                                   QListWidgetItem *previous) {
  Q_UNUSED(previous)
  m_ui->button_remove_image->setEnabled(current != nullptr);
}

void MainWindow::on_button_add_font_clicked() {
  LVGLFontDialog dialog(this);
  if (dialog.exec() != QDialog::Accepted) return;
  LVGLFontData *f =
      lvgl->addFont(dialog.selectedFontPath(), dialog.selectedFontSize());
  if (f)
    addFont(f, f->name());
  else
    QMessageBox::critical(this, "Error", "Could not load font!");
}

void MainWindow::on_button_remove_font_clicked() {
  QListWidgetItem *item = m_ui->list_fonts->currentItem();
  if (item == nullptr) return;
  const int row = m_ui->list_fonts->currentRow();

  LVGLFontDataCast cast;
  cast.i = item->data(Qt::UserRole + 3).toLongLong();

  if (lvgl->removeFont(cast.ptr)) m_ui->list_fonts->takeItem(row);
}

void MainWindow::on_list_fonts_customContextMenuRequested(const QPoint &pos) {
  QPoint item = m_ui->list_fonts->mapToGlobal(pos);
  QListWidgetItem *listItem = m_ui->list_fonts->itemAt(pos);
  if (listItem == nullptr) return;

  QMenu menu;
  QAction *save = menu.addAction("Save as ...");
  QAction *sel = menu.exec(item);
  if (sel == save) {
    LVGLFontDataCast cast;
    cast.i = listItem->data(Qt::UserRole + 3).toLongLong();

    QStringList options({"C Code (*.c)", "Binary (*.bin)"});
    QString selected;
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save font as c file", cast.ptr->codeName(), options.join(";;"),
        &selected);
    if (fileName.isEmpty()) return;
    bool ok = false;
    if (selected == options.at(0)) ok = cast.ptr->saveAsCode(fileName);
    if (!ok) {
      QMessageBox::critical(this, "Error",
                            tr("Could not save font '%1'").arg(fileName));
    }
  }
}

void MainWindow::on_list_fonts_currentItemChanged(QListWidgetItem *current,
                                                  QListWidgetItem *previous) {
  Q_UNUSED(previous)
  m_ui->button_remove_font->setEnabled(current != nullptr);
}

void MainWindow::on_action_run_toggled(bool run) {
  m_curSimulation->setMouseEnable(run);
  m_curSimulation->setSelectedObject(nullptr);
}

void MainWindow::showEvent(QShowEvent *event) {
  QMainWindow::showEvent(event);
  if (m_project == nullptr && !m_isrun)
    QTimer::singleShot(50, this, SLOT(openNewProject()));
}

QPixmap MainWindow::getPix(int type) {
  QPixmap p;
  switch (type) {
    case 0:
      p.load(":/icons/Arc.png");
      break;
    case 1:
      p.load(":/icons/Bar.png");
      break;
    case 2:
      p.load(":/icons/Button.png");
      break;
    case 3:
      p.load(":/icons/Button Matrix.png");
      break;
    case 4:
      p.load(":/icons/Calendar.png");
      break;
    case 5:
      p.load(":/icons/Canvas.png");
      break;
    case 6:
      p.load(":/icons/Check box.png");
      break;
    case 7:
      p.load(":/icons/Chart.png");
      break;
    case 8:
      p.load(":/icons/Container.png");
      break;
    case 9:
      p.load(":/icons/Color picker.png");
      break;
    case 10:
      p.load(":/icons/Dropdown.png");
      break;
    case 11:
      p.load(":/icons/Gauge.png");
      break;
    case 12:
      p.load(":/icons/Image.png");
      break;
    case 13:
      p.load(":/icons/Image button.png");
      break;
    case 16:
      p.load(":/icons/Keyboard.png");
      break;
    case 17:
      p.load(":/icons/Label.png");
      break;
    case 18:
      p.load(":/icons/LED.png");
      break;
    case 19:
      p.load(":/icons/Line.png");
      break;
    case 20:
      p.load(":/icons/List.png");
      break;
    case 21:
      p.load(":/icons/Line meter.png");
      break;
    case 22:
      p.load(":/icons/Message box.png");
      break;
    case 23:
      p.load(":/icons/ObjectMask.png");
      break;
    case 24:
      p.load(":/icons/Page.png");
      break;
    case 25:
      p.load(":/icons/Roller.png");
      break;
    case 26:
      p.load(":/icons/Slider.png");
      break;
    case 27:
      p.load(":/icons/Spinbox.png");
      break;
    case 28:
      p.load(":/icons/Spinner.png");
      break;
    case 29:
      p.load(":/icons/Switch.png");
      break;
    case 30:
      p.load(":/icons/Table.png");
      break;
    case 31:
      p.load(":/icons/Tabview.png");
      break;
    case 32:
      p.load(":/icons/Text area.png");
      break;
    case 33:
      p.load(":/icons/TileView.png");
      break;
    case 34:
      p.load(":/icons/Window.png");
      break;
  }

  return p;
}

void MainWindow::addWidget(LVGLWidget *w) {
  w->setPreview(getPix(w->type()));
  m_widgets.insert(w->className(), w);
}

void MainWindow::addWidgetDisplayW(LVGLWidget *w) {
  w->setPreview(getPix(w->type()));
  m_widgetsDisplayW.insert(w->className(), w);
}

void MainWindow::addWidgetInputW(LVGLWidget *w) {
  w->setPreview(getPix(w->type()));
  m_widgetsInputW.insert(w->className(), w);
}

void MainWindow::initcodemap() {
  auto pt = lv_obj_create(NULL, NULL);
  m_codemap[0] = lv_arc_create(pt, NULL);
  m_codemap[1] = lv_bar_create(pt, NULL);
  m_codemap[2] = lv_btn_create(pt, NULL);
  m_codemap[3] = lv_btnmatrix_create(pt, NULL);
  m_codemap[4] = lv_calendar_create(pt, NULL);
  m_codemap[5] = lv_canvas_create(pt, NULL);
  m_codemap[6] = lv_checkbox_create(pt, NULL);
  m_codemap[7] = lv_chart_create(pt, NULL);
  m_codemap[8] = lv_cont_create(pt, NULL);
  m_codemap[9] = lv_cpicker_create(pt, NULL);
  m_codemap[10] = lv_dropdown_create(pt, NULL);
  lv_dropdown_open(m_codemap[10]);
  m_codemap[11] = lv_gauge_create(pt, NULL);
  m_codemap[12] = lv_img_create(pt, NULL);
  m_codemap[13] = lv_imgbtn_create(pt, NULL);
  m_codemap[16] = lv_keyboard_create(pt, NULL);
  m_codemap[17] = lv_label_create(pt, NULL);
  m_codemap[18] = lv_led_create(pt, NULL);
  m_codemap[19] = lv_line_create(pt, NULL);
  m_codemap[20] = lv_list_create(pt, NULL);
  m_codemap[21] = lv_linemeter_create(pt, NULL);
  m_codemap[22] = lv_msgbox_create(pt, NULL);
  static const char *btns[] = {"Ok", "Cancel", ""};
  lv_msgbox_add_btns(m_codemap[22], btns);
  m_codemap[23] = lv_objmask_create(pt, NULL);
  m_codemap[24] = lv_page_create(pt, NULL);
  m_codemap[25] = lv_roller_create(pt, NULL);
  m_codemap[26] = lv_slider_create(pt, NULL);
  m_codemap[27] = lv_spinbox_create(pt, NULL);
  m_codemap[28] = lv_spinner_create(pt, NULL);
  m_codemap[29] = lv_switch_create(pt, NULL);
  m_codemap[30] = lv_table_create(pt, NULL);
  m_codemap[31] = lv_tabview_create(pt, NULL);
  m_codemap[32] = lv_textarea_create(pt, NULL);
  m_codemap[33] = lv_tileview_create(pt, NULL);
  m_codemap[34] = lv_win_create(pt, NULL);
}

void MainWindow::initNewWidgets() {
  addWidget(new LVGLButton);
  addWidget(new LVGLButtonMatrix);
  addWidget(new LVGLImageButton);

  addWidgetDisplayW(new LVGLArc);
  addWidgetDisplayW(new LVGLBar);
  addWidgetDisplayW(new LVGLImage);
  addWidgetDisplayW(new LVGLLabel);
  addWidgetDisplayW(new LVGLLED);
  addWidgetDisplayW(new LVGLMessageBox);
  addWidgetDisplayW(new LVGLObjectMask);
  addWidgetDisplayW(new LVGLPage);
  addWidgetDisplayW(new LVGLTable);
  addWidgetDisplayW(new LVGLTabview);
  addWidgetDisplayW(new LVGLTileView);
  addWidgetDisplayW(new LVGLTextArea);
  addWidgetDisplayW(new LVGLWindow);

  addWidgetInputW(new LVGLCalendar);
  addWidgetInputW(new LVGLCanvas);
  addWidgetInputW(new LVGLChart);
  addWidgetInputW(new LVGLCheckBox);
  addWidgetInputW(new LVGLColorPicker);
  addWidgetInputW(new LVGLContainer);
  addWidgetInputW(new LVGLDropDownList);
  addWidgetInputW(new LVGLGauge);
  addWidgetInputW(new LVGLKeyboard);
  addWidgetInputW(new LVGLLine);
  addWidgetInputW(new LVGLList);
  addWidgetInputW(new LVGLLineMeter);
  addWidgetInputW(new LVGLRoller);
  addWidgetInputW(new LVGLSlider);
  addWidgetInputW(new LVGLSpinbox);
  addWidgetInputW(new LVGLSpinner);
  addWidgetInputW(new LVGLSwitch);
}

void MainWindow::initlvglConnect() {
  if (m_objectModel) delete m_objectModel;
  if (m_proxyModel) delete m_proxyModel;
  if (m_proxyModelDPW) delete m_proxyModelDPW;
  if (m_proxyModelIPW) delete m_proxyModelIPW;
  if (m_widgetModel) delete m_widgetModel;
  if (m_widgetModelDPW) delete m_widgetModelDPW;
  if (m_widgetModelIPW) delete m_widgetModelIPW;

  m_objectModel = new LVGLObjectModel(this);
  m_widgetModel = new LVGLWidgetModel(this);
  m_widgetModelDPW = new LVGLWidgetModelDisplay(this);
  m_widgetModelIPW = new LVGLWidgetModelInput(this);
  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModelDPW = new QSortFilterProxyModel(this);
  m_proxyModelIPW = new QSortFilterProxyModel(this);

  m_ui->object_tree->setModel(m_objectModel);
  m_curSimulation->setObjectModel(m_objectModel);

  m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_proxyModel->setSourceModel(m_widgetModel);
  m_proxyModel->sort(0);

  m_proxyModelDPW->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_proxyModelDPW->setSourceModel(m_widgetModelDPW);
  m_proxyModelDPW->sort(0);

  m_proxyModelIPW->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_proxyModelIPW->setSourceModel(m_widgetModelIPW);
  m_proxyModelIPW->sort(0);

  m_ui->list_widgets->getlistview()->setItemDelegate(m_ld1);
  m_ui->list_widgets_2->getlistview()->setItemDelegate(m_ld2);
  m_ui->list_widgets_3->getlistview()->setItemDelegate(m_ld3);

  m_ui->list_widgets->getlistview()->setModel(m_proxyModel);
  m_ui->list_widgets_2->getlistview()->setModel(m_proxyModelDPW);
  m_ui->list_widgets_3->getlistview()->setModel(m_proxyModelIPW);

  m_ui->list_widgets->settoolbtnText(tr("Button"));
  m_ui->list_widgets_2->settoolbtnText(tr("DisplayWidgets"));
  m_ui->list_widgets_3->settoolbtnText(tr("InputWidgts"));

  connect(m_curSimulation, &LVGLSimulator::objectSelected, this,
          &MainWindow::setCurrentObject);
  connect(m_curSimulation, &LVGLSimulator::objectSelected, m_ui->property_tree,
          &QTreeView::expandAll);
  connect(m_curSimulation->item(), &LVGLItem::geometryChanged, this,
          &MainWindow::updateProperty);
  connect(m_curSimulation, &LVGLSimulator::objectAdded, m_ui->object_tree,
          &QTreeView::expandAll);
  connect(m_curSimulation, &LVGLSimulator::objectSelected, m_objectModel,
          &LVGLObjectModel::setCurrentObject);
  connect(m_ui->object_tree, &QTreeView::doubleClicked, this,
          [this](const QModelIndex &index) {
            m_curSimulation->setSelectedObject(m_objectModel->object(index));
          });
  connect(m_zoom_slider, &QSlider::valueChanged, m_curSimulation,
          &LVGLSimulator::setZoomLevel);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModel,
          &QSortFilterProxyModel::setFilterWildcard);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModelDPW,
          &QSortFilterProxyModel::setFilterWildcard);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModelIPW,
          &QSortFilterProxyModel::setFilterWildcard);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets,
          &ListViewItem::slot_toshowtab);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets_2,
          &ListViewItem::slot_toshowtab);
  connect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets_3,
          &ListViewItem::slot_toshowtab);

  if (m_filter != nullptr) delete m_filter;
  m_filter = new LVGLKeyPressEventFilter(m_curSimulation, qApp);
  qApp->installEventFilter(m_filter);

  m_ui->combo_state->clear();
  QStringList statelist;
  statelist << "LV_STATE_DEFAULT"
            << "LV_STATE_CHECKED"
            << "LV_STATE_FOCUSED"
            << "LV_STATE_EDITED"
            << "LV_STATE_HOVERED"
            << "LV_STATE_PRESSED"
            << "LV_STATE_DISABLED";
  m_ui->combo_state->addItems(statelist);
}

void MainWindow::revlvglConnect() {
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModel,
             &QSortFilterProxyModel::setFilterWildcard);
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModelDPW,
             &QSortFilterProxyModel::setFilterWildcard);
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_proxyModelIPW,
             &QSortFilterProxyModel::setFilterWildcard);
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets,
             &ListViewItem::slot_toshowtab);
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets_2,
             &ListViewItem::slot_toshowtab);
  disconnect(m_ui->edit_filter, &QLineEdit::textChanged, m_ui->list_widgets_3,
             &ListViewItem::slot_toshowtab);
  disconnect(m_curSimulation, &LVGLSimulator::objectSelected, this,
             &MainWindow::setCurrentObject);
  disconnect(m_curSimulation, &LVGLSimulator::objectSelected,
             m_ui->property_tree, &QTreeView::expandAll);
  disconnect(m_curSimulation->item(), &LVGLItem::geometryChanged, this,
             &MainWindow::updateProperty);
  disconnect(m_curSimulation, &LVGLSimulator::objectAdded, m_ui->object_tree,
             &QTreeView::expandAll);
  disconnect(m_curSimulation, &LVGLSimulator::objectSelected, m_objectModel,
             &LVGLObjectModel::setCurrentObject);
  disconnect(m_zoom_slider, &QSlider::valueChanged, m_curSimulation,
             &LVGLSimulator::setZoomLevel);
}

void MainWindow::on_combo_state_currentIndexChanged(int index) {
  LVGLObject *obj = m_curSimulation->selectedObject();
  if (obj && (index >= 0)) {
    auto parts = obj->widgetClass()->parts();
    m_styleModel->setPart(parts[m_ui->combo_style->currentIndex()]);
    m_styleModel->setState(m_liststate[index]);
    m_styleModel->setStyle(
        obj->style(m_ui->combo_style->currentIndex(), index),
        obj->widgetClass()->editableStyles(m_ui->combo_style->currentIndex()));
    updateItemDelegate();
  }
}

void MainWindow::tabChanged(int index) {
  if (index != m_curTabWIndex && index >= 0) {
    if (nullptr != m_curSimulation) m_curSimulation->setSelectedObject(nullptr);
    m_curSimulation = m_listTabW[index]->getSimulator();
    lvgl = m_listTabW[index]->getCore();
    m_project = m_listTabW[index]->getProject();  // dont need
    initlvglConnect();

    lvgl->changeResolution(m_coreRes[lvgl]);
    m_curSimulation->changeResolution(m_coreRes[lvgl]);
    m_curSimulation->repaint();
    m_curTabWIndex = index;
  }
}
