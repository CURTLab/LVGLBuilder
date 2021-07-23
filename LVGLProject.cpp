#include "LVGLProject.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "LVGLFontData.h"

#define IS_PAGE_OF_TABVIEW(o) ((o->widgetType() == LVGLWidget::Page) && (o->index() >= 0) && o->parent() && (o->parent()->widgetType() == LVGLWidget::TabView))

LVGLProject::LVGLProject()
	: m_name("App")
	, m_resolution(LV_HOR_RES_MAX, LV_VER_RES_MAX)
{
}

LVGLProject::LVGLProject(const QString &name, QSize resolution)
	: m_name(name)
	, m_resolution(resolution)
{
}

QString LVGLProject::name() const
{
	return m_name;
}

QSize LVGLProject::resolution() const
{
	return m_resolution;
}

LVGLProject *LVGLProject::load(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return nullptr;
	QJsonObject doc = QJsonDocument::fromJson(file.readAll()).object();
	file.close();
	if (!doc.contains("lvgl"))
		return nullptr;

	QJsonObject lvglObj = doc["lvgl"].toObject();
	if (!lvglObj.contains("widgets"))
		return nullptr;

	QSize resolution;
	if (lvglObj.contains("resolution")) {
		QJsonObject res = lvglObj["resolution"].toObject();
		resolution = QSize(res["width"].toInt(), res["height"].toInt());
	} else {
		resolution = QSize(LV_HOR_RES_MAX, LV_VER_RES_MAX);
	}

	QJsonArray imageArr = doc["images"].toArray();
	for (int i = 0; i < imageArr.size(); ++i) {
		LVGLImageData *img = new LVGLImageData(imageArr[i].toObject());
		if (img->isValid()) {
			lvgl.addImage(img);
		} else {
			QMessageBox::critical(nullptr, "Error", QString("Could not load image (%1)").arg(img->fileName()));
			delete img;
		}
	}

	QJsonArray fontArr = doc["fonts"].toArray();
	for (int i = 0; i < fontArr.size(); ++i) {
		QJsonObject object = fontArr[i].toObject();
		lvgl.addFont(LVGLFontData::parse(object));
	}

	if (lvglObj.contains("screen color"))
		lvgl.setScreenColor(lvglObj["screen color"].toVariant().value<QColor>());
	QJsonArray widgetArr = lvglObj["widgets"].toArray();
	for (int i = 0; i < widgetArr.size(); ++i) {
		QJsonObject object = widgetArr[i].toObject();
		LVGLObject::parse(object, nullptr);
	}

	LVGLProject *pro = new LVGLProject(lvglObj["name"].toString(), resolution);
	pro->m_fileName = fileName;
	return pro;
}

bool LVGLProject::save(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	QJsonArray widgetArr;
	for (LVGLObject *o:lvgl.allObjects()) {
		if (o->parent() == nullptr) {
			if (o->doesNameExists())
				o->generateName();
			widgetArr.append(o->toJson());
		}
	}

	QJsonArray imageArr;
	for (LVGLImageData *i:lvgl.images()) {
		if (!i->fileName().isEmpty())
			imageArr.append(i->toJson());
	}

	QJsonArray fontArr;
	for (const LVGLFontData *f:lvgl.customFonts())
		fontArr.append(f->toJson());

	QJsonObject resolution({{"width", m_resolution.width()},
									{"height", m_resolution.height()}
								  });
	QJsonObject screen({{"widgets", widgetArr},
							  {"name", m_name},
							  {"resolution", resolution}
							 });
	if (lvgl.screenColorChanged())
		screen.insert("screen color", QVariant(lvgl.screenColor()).toString());
	QJsonObject lvgl({{"lvgl", screen},
							{"images", imageArr},
							{"fonts", fontArr}
						  });
	QJsonDocument doc(lvgl);

	file.write(doc.toJson());
	file.close();

	m_fileName = fileName;

	return true;
}

bool LVGLProject::exportCode(const QString &path) const
{
	QDir dir(path);

	QFile file;
	QTextStream stream;
	const QString name = m_name.toLower();
	const QString codeName = m_name.toLower().replace(" ", "_");
	const QString defName = m_name.toUpper().replace(" ", "_");

	file.setFileName(dir.path() + "/" + name + ".h");
	if (!file.open(QIODevice::WriteOnly))
		return false;
	stream.setDevice(&file);

	auto objects = lvgl.allObjects();

	stream << "#ifndef " << defName << "_H\n#define " << defName << "_H\n\n";
	stream << "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";
	//includes
	stream << "/**********************\n";
	stream << " *      INCLUDES\n";
	stream << " **********************/\n\n";
	stream << "#include \"lvgl.h\"\n\n";
	stream << "/**********************\n";
	stream << " *       WIDGETS\n";
	stream << " **********************/\n";
	for (LVGLObject *o:objects) {
		Q_ASSERT(!o->doesNameExists());
		if (o->isAccessible())
			stream << "extern lv_obj_t *" << o->codeName() << ";\n";
	}
	stream << "\n";
	// global prototypes
	stream << "/**********************\n";
	stream << " * GLOBAL PROTOTYPES\n";
	stream << " **********************/\n\n";
	stream << "void " << codeName << "_create(lv_obj_t *parent);\n\n";
	stream << "#ifdef __cplusplus\n} /* extern \"C\" */\n#endif\n\n";
	stream << "#endif /*" << defName << "_H*/";
	file.close();

	file.setFileName(dir.path() + "/" + name + ".c");
	if (!file.open(QIODevice::WriteOnly))
		return false;
	stream.setDevice(&file);
	stream << "#include \"" << name << ".h\"\n\n";
	// static variables
	stream << "/**********************\n";
	stream << " *       WIDGETS\n";
	stream << " **********************/\n";
	for (LVGLObject *o:objects) {
		if (o->isAccessible())
			stream << "lv_obj_t *" << o->codeName() << ";\n";
	}

	stream << "\n";
	stream << "/**********************\n";
	stream << " *  STATIC VARIABLES\n";
	stream << " **********************/\n";
	if (lvgl.screenColorChanged()) {
		stream << "static lv_style_t style_screen;\n";
	}
	for (LVGLObject *o:objects) {
		for (int i = 0; i < o->widgetClass()->styles().size(); ++i) {
			if (o->hasCustomStyle(i))
				stream << "static lv_style_t " << o->styleCodeName(i) << ";\n";
		}
	}
	stream << "\n";

	auto images = lvgl.images();
	for (LVGLImageData *img:images) {
		img->saveAsCode(dir.path() + "/" + img->codeName() + ".c");
		stream << "LV_IMG_DECLARE(" << img->codeName() << ");\n";
	}
	stream << "\n";
	auto fonts = lvgl.customFonts();
	for (const LVGLFontData *f:fonts) {
		f->saveAsCode(dir.path() + "/" + f->codeName() + ".c");
		stream << "LV_FONT_DECLARE(" << f->codeName() << ");\n";
	}
	stream << "\n";

	// application
	stream << "void "  << codeName << "_create(lv_obj_t *parent)\n";
	stream << "{\n";
	if (lvgl.screenColorChanged()) {
		QString color = QVariant(lvgl.screenColor()).toString().replace("#", "0x");
		stream << "\tlv_style_copy(&style_screen, &lv_style_scr);\n";
		stream << "\tstyle_screen.body.main_color = lv_color_hex(" << color << ");\n";
		stream << "\tstyle_screen.body.grad_color = lv_color_hex(" << color << ");\n";
		stream << "\tlv_obj_set_style(parent, &style_screen);\n";
	}
	stream << "\n";
	for (LVGLObject *o:objects) {
		QString ifdef = o->widgetClass()->className().toUpper().insert(3, "USE_");
		stream << "#if " << ifdef <<"\n";

		for (int i = 0; i < o->widgetClass()->styles().size(); ++i) {
			if (o->hasCustomStyle(i)) {
				QString style = o->styleCodeName(i);
				QString base = lvgl.baseStyleName(o->widgetClass()->defaultStyle(i));
				if (base.isEmpty()) base = "lv_style_transp";
				stream << "\tlv_style_copy(&" << style << ", &" << base << ");\n";
				QStringList styleSet = o->codeStyle(style, i);
				for (const QString &s:styleSet)
					stream << "\t" << s << "\n";
				stream << "\n";
			}
		}

		QString parent = "parent";
		if (o->parent())
			parent = o->parent()->codeName();

		if (IS_PAGE_OF_TABVIEW(o)) {
			for (LVGLProperty *p:o->widgetClass()->specialProperties()) {
				for (const QString &fn:p->function(o))
					stream << "\t" << fn << "\n";
			}
		} else {
			if (o->isAccessible())
				stream << "\t" << o->codeName() << " = " << o->widgetClass()->className() << "_create(" << parent << ", NULL);\n";
			else
				stream << "\tlv_obj_t *" << o->codeName() << " = " << o->widgetClass()->className() << "_create(" << parent << ", NULL);\n";

			// special case for tabview
			if (o->widgetType() == LVGLWidget::TabView) {
				lv_tabview_ext_t *ext = reinterpret_cast<lv_tabview_ext_t*>(lv_obj_get_ext_attr(o->obj()));
				for (LVGLObject *child:o->childs()) {
					if ((child->widgetType() == LVGLWidget::Page) && (child->index() >= 0))
						stream << "\tlv_obj_t *" << child->codeName() << " = lv_tabview_add_tab(" << o->codeName() << ", \"" << ext->tab_name_ptr[child->index()] << "\");\n";
				}
			}

			// generate properties for widget
			for (LVGLProperty *p:o->widgetClass()->properties()) {
				for (const QString &fn:p->function(o))
					stream << "\t" << fn << "\n";
			}
		}

		for (int i = 0; i < o->widgetClass()->styles().size(); ++i) {
			if (o->hasCustomStyle(i)) {
				QString style = o->styleCodeName(i);
				stream << "\t" << o->widgetClass()->className() << "_set_style(" << o->codeName() << ", " << o->widgetClass()->styles().at(i) << ", &" << style << ");\n";
			}
		}
		stream << "#endif // " << ifdef <<"\n";
		stream << "\n";
	}

	stream << "}\n";

	file.close();

	return true;
}

QString LVGLProject::fileName() const
{
	return m_fileName;
}
