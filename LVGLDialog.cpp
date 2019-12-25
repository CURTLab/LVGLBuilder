#include "LVGLDialog.h"

#include "LVGLCore.h"

LVGLDialog::LVGLDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(QString("LVGL [%1x%2]").arg(lvgl.width()).arg(lvgl.height()));
	setFixedSize(lvgl.size());
	setSizeGripEnabled(false);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}
