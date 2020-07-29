#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "LVGLObjectModel.h"
#include "LVGLPropertyModel.h"
#include "LVGLWidgetModel.h"
#include "LVGLStyleModel.h"

#include "widgets/LVGLWidgets.h"

#include "LVGLDialog.h"
#include "LVGLFontData.h"
#include "LVGLNewDialog.h"
#include "LVGLFontDialog.h"

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
	, m_project(nullptr)
	, m_maxFileNr(5)
{
	m_ui->setupUi(this);

	lvgl.init(320, 480);

	m_zoom_slider->setRange(-2, 2);
	connect(m_zoom_slider, &QSlider::valueChanged, m_ui->simulation, &LVGLSimulator::setZoomLevel);
	m_ui->statusbar->addPermanentWidget(m_zoom_slider);

	m_ui->button_remove_image->setEnabled(false);
	m_ui->button_remove_font->setEnabled(false);

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
	connect(m_ui->action_new, &QAction::triggered,
			  this, &MainWindow::openNewProject
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

	// add font editor dock to image dock and show the image dock
	tabifyDockWidget(m_ui->ImageEditor, m_ui->FontEditor);
	m_ui->ImageEditor->raise();
}

MainWindow::~MainWindow()
{
	delete m_ui;
	delete m_project;
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
	loadProject(action->data().toString());
}

void MainWindow::openNewProject()
{
	LVGLNewDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted) {
		delete m_project;
		m_project = new LVGLProject(dialog.selectedName(), dialog.selectedResolution());
		m_ui->simulation->clear();
		setEnableBuilder(true);
		setWindowTitle("LVGL Builder - [" + m_project->name() + "]");
		const auto res = dialog.selectedResolution();
		lvgl.changeResolution(res);
		m_ui->simulation->changeResolution(res);
	} else if (m_project == nullptr) {
		setEnableBuilder(false);
		setWindowTitle("LVGL Builder");
	}
}

void MainWindow::addImage(LVGLImageData *img, QString name)
{
	LVGLImageDataCast cast;
	cast.ptr = img;

	QListWidgetItem *item = new QListWidgetItem(img->icon(), name);
	item->setData(Qt::UserRole + 3, cast.i);
	m_ui->list_images->addItem(item);
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

void MainWindow::addFont(LVGLFontData *font, QString name)
{
	LVGLFontDataCast cast;
	cast.ptr = font;

	QListWidgetItem *item = new QListWidgetItem(name);
	item->setData(Qt::UserRole + 3, cast.i);
	m_ui->list_fonts->addItem(item);
}

void MainWindow::updateFonts()
{
	m_ui->list_fonts->clear();
	for (const LVGLFontData *f:lvgl.customFonts())
		addFont(const_cast<LVGLFontData*>(f), f->name());
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

void MainWindow::loadProject(const QString &fileName)
{
	delete m_project;
	m_ui->simulation->clear();
	m_project = LVGLProject::load(fileName);
	if (m_project == nullptr) {
		QMessageBox::critical(this, "Error", "Could not load lvgl file!");
		setWindowTitle("LVGL Builder");
		setEnableBuilder(false);
	} else {
		adjustForCurrentFile(fileName);
		setWindowTitle("LVGL Builder - [" + m_project->name() + "]");
		lvgl.changeResolution(m_project->resolution());
		m_ui->simulation->changeResolution(m_project->resolution());
		setEnableBuilder(true);
	}
	updateImages();
	updateFonts();
}

void MainWindow::setEnableBuilder(bool enable)
{
	m_ui->action_save->setEnabled(enable);
	m_ui->action_export_c->setEnabled(enable);
	m_ui->action_run->setEnabled(enable);

	m_ui->WidgeBox->setEnabled(enable);
	m_ui->ImageEditor->setEnabled(enable);
	m_ui->FontEditor->setEnabled(enable);
}

void MainWindow::on_action_load_triggered()
{
	QString path;
	if (m_project != nullptr)
		path = m_project->fileName();
	QString fileName = QFileDialog::getOpenFileName(this, "Load lvgl", path, "LVGL (*.lvgl)");
	if (fileName.isEmpty())
		return;
	loadProject(fileName);
}

void MainWindow::on_action_save_triggered()
{
	QString path;
	if (m_project != nullptr)
		path = m_project->fileName();
	QString fileName = QFileDialog::getSaveFileName(this, "Save lvgl", path, "LVGL (*.lvgl)");
	if (fileName.isEmpty())
		return;
	if (!m_project->save(fileName)) {
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

void MainWindow::on_action_export_c_triggered()
{
	QString dir;
	if (m_project != nullptr) {
		QFileInfo fi(m_project->fileName());
		dir = fi.absoluteFilePath();
	}
	QString path = QFileDialog::getExistingDirectory(this, "Export C files", dir);
	if (path.isEmpty())
		return;
	if (m_project->exportCode(path))
		QMessageBox::information(this, "Export", "C project exported!");
}

void MainWindow::on_button_add_image_clicked()
{
	QString dir;
	if (m_project != nullptr) {
		QFileInfo fi(m_project->fileName());
		dir = fi.absoluteFilePath();
	}
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Import image", dir, "Image (*.png *.jpg *.bmp *.jpeg)");
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

void MainWindow::on_button_remove_image_clicked()
{
	QListWidgetItem *item = m_ui->list_images->currentItem();
	if (item == nullptr)
		return;
	const int row = m_ui->list_images->currentRow();

	LVGLImageDataCast cast;
	cast.i = item->data(Qt::UserRole + 3).toLongLong();

	if (lvgl.removeImage(cast.ptr))
		m_ui->list_images->takeItem(row);
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
		LVGLImageDataCast cast;
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
		LVGLImageDataCast cast;
		cast.i = listItem->data(Qt::UserRole + 3).toLongLong();
		int index = static_cast<int>(cast.ptr->colorFormat());
		QString ret = QInputDialog::getItem(this, "Output color", "Select output color", LVGLImageData::colorFormats(), index, false);
		index = LVGLImageData::colorFormats().indexOf(ret);
		if (index >= 0)
			cast.ptr->setColorFormat(static_cast<LVGLImageData::ColorFormat>(index));
	}
}

void MainWindow::on_list_images_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(previous)
	m_ui->button_remove_image->setEnabled(current != nullptr);
}

void MainWindow::on_button_add_font_clicked()
{
	LVGLFontDialog dialog(this);
	if (dialog.exec() != QDialog::Accepted)
		return;
	LVGLFontData *f = lvgl.addFont(dialog.selectedFontPath(), dialog.selectedFontSize());
	if (f)
		addFont(f, f->name());
	else
		QMessageBox::critical(this, "Error", "Could not load font!");
}

void MainWindow::on_button_remove_font_clicked()
{
	QListWidgetItem *item = m_ui->list_fonts->currentItem();
	if (item == nullptr)
		return;
	const int row = m_ui->list_fonts->currentRow();

	LVGLFontDataCast cast;
	cast.i = item->data(Qt::UserRole + 3).toLongLong();

	if (lvgl.removeFont(cast.ptr))
		m_ui->list_fonts->takeItem(row);
}

void MainWindow::on_list_fonts_customContextMenuRequested(const QPoint &pos)
{
	QPoint item = m_ui->list_fonts->mapToGlobal(pos);
	QListWidgetItem *listItem = m_ui->list_fonts->itemAt(pos);
	if (listItem == nullptr)
		return;

	QMenu menu;
	QAction *save = menu.addAction("Save as ...");
	QAction *sel = menu.exec(item);
	if (sel == save) {
		LVGLFontDataCast cast;
		cast.i = listItem->data(Qt::UserRole + 3).toLongLong();

		QStringList options({"C Code (*.c)", "Binary (*.bin)"});
		QString selected;
		QString fileName = QFileDialog::getSaveFileName(this, "Save font as c file", cast.ptr->codeName(), options.join(";;"), &selected);
		if (fileName.isEmpty())
			return;
		bool ok = false;
		if (selected == options.at(0))
			ok = cast.ptr->saveAsCode(fileName);
		if (!ok) {
			QMessageBox::critical(this, "Error", tr("Could not save font '%1'").arg(fileName));
		}
	}
}

void MainWindow::on_list_fonts_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(previous)
	m_ui->button_remove_font->setEnabled(current != nullptr);
}

void MainWindow::on_action_run_toggled(bool run)
{
	m_ui->simulation->setMouseEnable(run);
	m_ui->simulation->setSelectedObject(nullptr);
}

void MainWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);
	if (m_project == nullptr)
		QTimer::singleShot(50, this, SLOT(openNewProject()));
}
