#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "LVGLObjectModel.h"
#include "LVGLPropertyModel.h"
#include "LVGLWidgetModel.h"
#include "LVGLStyleModel.h"

#include "widgets/LVGLWidgets.h"

#include "LVGLDialog.h"
#include "NewDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QSettings>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow)
	, m_zoom_slider(new QSlider(Qt::Horizontal))
	, m_maxFileNr(5)
{
	m_ui->setupUi(this);

	m_zoom_slider->setRange(-2, 2);
	connect(m_zoom_slider, &QSlider::valueChanged, m_ui->simulation, &LVGLSimulator::setZoomLevel);
	m_ui->statusbar->addPermanentWidget(m_zoom_slider);

	m_propertyModel = new LVGLPropertyModel();
	connect(m_ui->simulation, &LVGLSimulator::objectSelected,
			  this, &MainWindow::setCurrentObject
			  );
	connect(m_ui->simulation, &LVGLSimulator::objectSelected,
			  m_ui->property_tree, &QTreeView::expandAll
			  );
	connect(m_ui->simulation->item(), &LVGLItem::geometryChanged,
			  this, &MainWindow::updateProperty
			  );
	m_ui->property_tree->setModel(m_propertyModel);
	m_ui->property_tree->setItemDelegate(new LVGLPropertyDelegate);

	m_objectModel = new LVGLObjectModel;
	connect(m_ui->simulation, &LVGLSimulator::objectSelected,
			  m_objectModel, &LVGLObjectModel::setCurrentObject
			  );
	connect(m_ui->object_tree, &QTreeView::doubleClicked,
			  this, [this](const QModelIndex &index) {
					m_ui->simulation->setSelectedObject(m_objectModel->object(index));
				});
	m_ui->object_tree->setModel(m_objectModel);

	LVGLWidgetModel *widgetModel = new LVGLWidgetModel;
	QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	connect(m_ui->edit_filter, &QLineEdit::textChanged,
			  proxyModel, &QSortFilterProxyModel::setFilterWildcard
			  );

	proxyModel->setSourceModel(widgetModel);
	proxyModel->sort(0);

	m_ui->list_widgets->setModel(proxyModel);

	m_styleModel = new LVGLStyleModel;
	connect(m_styleModel, &LVGLStyleModel::styleChanged,
			  this, &MainWindow::styleChanged
			  );
	m_ui->style_tree->setModel(m_styleModel);
	m_ui->style_tree->setItemDelegate(new LVGLStyleDelegate(m_styleModel->styleBase()));
	m_ui->style_tree->expandAll();

	// recent configurations
	QAction *recentFileAction = nullptr;
	for(int i = 0; i < m_maxFileNr; i++){
		recentFileAction = new QAction(this);
		recentFileAction->setVisible(false);
		connect(recentFileAction, &QAction::triggered,
				  this, &MainWindow::loadRecent
				  );
		m_recentFileActionList.append(recentFileAction);
		m_ui->menu_resent_filess->addAction(recentFileAction);
	}
	updateRecentActionList();

	// add style editor dock to property dock and show the property dock
	tabifyDockWidget(m_ui->PropertyEditor, m_ui->StyleEditor);
	m_ui->PropertyEditor->raise();
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

LVGLSimulator *MainWindow::simulator() const
{
	return m_ui->simulation;
}

void MainWindow::updateProperty()
{
	LVGLObject *o = m_ui->simulation->selectedObject();
	if (o == nullptr)
		return;
	LVGLProperty *p = o->widgetClass()->property("Geometry");
	if (p == nullptr)
		return;

	for (int i = 0; i < p->count(); ++i) {
		auto index = m_propertyModel->propIndex(p->child(i), o->widgetClass(), 1);
		emit m_propertyModel->dataChanged(index, index);
	}
}

void MainWindow::setCurrentObject(LVGLObject *obj)
{
	m_ui->combo_style->clear();
	m_propertyModel->setObject(obj);
	if (obj) {
		m_ui->combo_style->addItems(obj->widgetClass()->styles());
		m_styleModel->setStyle(obj->style(0), obj->widgetClass()->editableStyles(0));
	} else {
		m_styleModel->setStyle(nullptr);
	}
}

void MainWindow::styleChanged()
{
	LVGLObject *obj = m_ui->simulation->selectedObject();
	if (obj) {
		int index = m_ui->combo_style->currentIndex();
		obj->widgetClass()->setStyle(obj->obj(), index, obj->style(index));
		//refresh_children_style(obj->obj());
		//lv_obj_refresh_style(obj->obj());
	}
}

void MainWindow::loadRecent()
{
	QAction *action = qobject_cast<QAction*>(QObject::sender());
	if (action == nullptr) return;
	loadUI(action->data().toString());
}

void MainWindow::updateImages()
{
	m_ui->list_images->clear();
	for (LVGLImageData *i:lvgl.images()) {
		if (i->fileName().isEmpty())
			continue;
		QString name = QFileInfo(i->fileName()).baseName() + QString(" [%1x%2]").arg(i->width()).arg(i->height());
		addImage(i, name);
	}
}

void MainWindow::addImage(LVGLImageData *img, QString name)
{
	union {
		LVGLImageData *ptr;
		qintptr i;
	} cast;

	cast.ptr = img;

	QListWidgetItem *item = new QListWidgetItem(img->icon(), name);
	item->setData(Qt::UserRole + 3, cast.i);
	m_ui->list_images->addItem(item);
}

void MainWindow::updateRecentActionList()
{
	QSettings settings("at.fhooe.lvgl", "LVGL Builder");
	QStringList recentFilePaths;
	for (const QString &f:settings.value("recentFiles").toStringList()) {
		if (QFile(f).exists())
			recentFilePaths.push_back(f);
	}

	int itEnd = m_maxFileNr;
	if(recentFilePaths.size() <= m_maxFileNr)
		itEnd = recentFilePaths.size();

	for (int i = 0; i < itEnd; i++) {
		QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
		m_recentFileActionList.at(i)->setText(strippedName);
		m_recentFileActionList.at(i)->setData(recentFilePaths.at(i));
		m_recentFileActionList.at(i)->setVisible(true);
	}

	for (int i = itEnd; i < m_maxFileNr; i++)
		m_recentFileActionList.at(i)->setVisible(false);
}

void MainWindow::adjustForCurrentFile(const QString &fileName)
{
	QSettings settings("at.fhooe.lvgl", "LVGL Builder");
	QStringList recentFilePaths =
			settings.value("recentFiles").toStringList();
	recentFilePaths.removeAll(fileName);
	recentFilePaths.prepend(fileName);
	while (recentFilePaths.size() > m_maxFileNr)
		recentFilePaths.removeLast();
	settings.setValue("recentFiles", recentFilePaths);

	updateRecentActionList();
}

void MainWindow::loadUI(const QString &fileName)
{
	if (!m_ui->simulation->load(fileName, &m_project)) {
		QMessageBox::critical(this, "Error", "Could not load lvgl file!");
	} else {
		updateImages();
		adjustForCurrentFile(fileName);
	}
}

void MainWindow::on_action_new_triggered()
{
	NewDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted) {
		m_project.setName(dialog.name());
		m_ui->simulation->clear();
		lvgl.removeAllImages();
	}
}

void MainWindow::on_action_load_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Load lvgl", "", "LVGL (*.lvgl)");
	if (fileName.isEmpty())
		return;
	loadUI(fileName);
}

void MainWindow::on_action_save_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save lvgl", "", "LVGL (*.lvgl)");
	if (fileName.isEmpty())
		return;
	if (!m_ui->simulation->save(fileName, &m_project)) {
		QMessageBox::critical(this, "Error", "Could not save lvgl file!");
	} else {
		adjustForCurrentFile(fileName);
	}
}

void MainWindow::on_combo_style_currentIndexChanged(int index)
{
	LVGLObject *obj = m_ui->simulation->selectedObject();
	if (obj && (index >= 0) && (index < obj->widgetClass()->styles().size()))
		m_styleModel->setStyle(obj->style(index), obj->widgetClass()->editableStyles(index));
}

void MainWindow::on_list_images_customContextMenuRequested(const QPoint &pos)
{
	QPoint item = m_ui->list_images->mapToGlobal(pos);
	QListWidgetItem *listItem = m_ui->list_images->itemAt(pos);
	if (listItem == nullptr)
		return;

	QMenu menu;
	QAction *save = menu.addAction("Save as ...");
	QAction *color = menu.addAction("Set output color ...");
	QAction *sel = menu.exec(item);
	if (sel == save) {
		union {
			LVGLImageData *ptr;
			qintptr i;
		} cast;

		cast.i = listItem->data(Qt::UserRole + 3).toLongLong();

		QStringList options({"C Code (*.c)", "Binary (*.bin)"});
		QString selected;
		QString fileName = QFileDialog::getSaveFileName(this, "Save image as c file", cast.ptr->codeName(), options.join(";;"), &selected);
		if (fileName.isEmpty())
			return;
		bool ok = false;
		if (selected == options.at(0))
			ok = cast.ptr->saveAsCode(fileName);
		else if (selected == options.at(1))
			ok = cast.ptr->saveAsBin(fileName);
		if (!ok) {
			QMessageBox::critical(this, "Error", tr("Could not save image '%1'").arg(fileName));
		}
	} else if (sel == color) {
		union {
			LVGLImageData *ptr;
			qintptr i;
		} cast;

		cast.i = listItem->data(Qt::UserRole + 3).toLongLong();
		int index = static_cast<int>(cast.ptr->colorFormat());
		QString ret = QInputDialog::getItem(this, "Output color", "Select output color", LVGLImageData::colorFormats(), index, false);
		index = LVGLImageData::colorFormats().indexOf(ret);
		if (index >= 0)
			cast.ptr->setColorFormat(static_cast<LVGLImageData::ColorFormat>(index));
	}
}

void MainWindow::on_action_export_c_triggered()
{
	QString path = QFileDialog::getExistingDirectory(this, "Export C files");
	if (path.isEmpty())
		return;
	if (m_ui->simulation->exportCode(path, &m_project))
		QMessageBox::information(this, "Export", "C project exported!");
}

void MainWindow::on_button_add_clicked()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Import image", "", "Image (*.png *.jpg *.bmp *.jpeg)");
	for (const QString &fileName:fileNames) {
		QImage image(fileName);
		if (image.isNull())
			continue;
		if (image.width() >= 2048 || image.height() >= 2048) {
			QMessageBox::critical(this, "Error Image Size", tr("Image size must be under 2048! (Src: '%1')").arg(fileName));
			continue;
		}

		QString name = QFileInfo(fileName).baseName();
		LVGLImageData *i = lvgl.addImage(fileName, name);
		name += QString(" [%1x%2]").arg(i->width()).arg(i->height());
		addImage(i, name);
	}
}

void MainWindow::on_button_remove_clicked()
{
	QListWidgetItem *item = m_ui->list_images->currentItem();
	if (item == nullptr)
		return;
	const int row = m_ui->list_images->currentRow();

	union {
		LVGLImageData *ptr;
		qintptr i;
	} cast;
	cast.i = item->data(Qt::UserRole + 3).toLongLong();

	if (lvgl.removeImage(cast.ptr))
		m_ui->list_images->takeItem(row);
}

void MainWindow::on_action_run_toggled(bool arg1)
{
	m_ui->simulation->setMouseEnable(arg1);
	m_ui->simulation->setSelectedObject(nullptr);
}
