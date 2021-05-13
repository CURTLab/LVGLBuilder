#include "LVGLProject.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>

#include "LVGLCore.h"
#include "LVGLFontData.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"
#include "events/LVGLEvent.h"

#define IS_PAGE_OF_TABVIEW(o)                                    \
  ((o->widgetType() == LVGLWidget::Page) && (o->index() >= 0) && \
   o->parent() && (o->parent()->widgetType() == LVGLWidget::TabView))

LVGLProject::LVGLProject()
    : m_name("App"), m_resolution(LV_HOR_RES_MAX, LV_VER_RES_MAX) {}

LVGLProject::LVGLProject(const QString &name, QSize resolution)
    : m_name(name), m_resolution(resolution) {}

QString LVGLProject::name() const { return m_name; }

QSize LVGLProject::resolution() const { return m_resolution; }

LVGLProject *LVGLProject::load(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) return nullptr;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  file.close();
  if (!doc.object().contains("lvgl")) return nullptr;

  QJsonObject lvglObj = doc["lvgl"].toObject();
  if (!lvglObj.contains("widgets")) return nullptr;

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
      QMessageBox::critical(
          nullptr, "Error",
          QString("Could not load image (%1)").arg(img->fileName()));
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

bool LVGLProject::save(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) return false;

  QJsonArray widgetArr;
  for (LVGLObject *o : lvgl.allObjects()) {
    if (o->parent() == nullptr) {
      if (o->doesNameExists()) o->generateName();
      widgetArr.append(o->toJson());
    }
  }

  QJsonArray imageArr;
  for (LVGLImageData *i : lvgl.images()) {
    if (!i->fileName().isEmpty()) imageArr.append(i->toJson());
  }

  QJsonArray fontArr;
  for (const LVGLFontData *f : lvgl.customFonts()) fontArr.append(f->toJson());

  QJsonObject resolution(
      {{"width", m_resolution.width()}, {"height", m_resolution.height()}});
  QJsonObject screen(
      {{"widgets", widgetArr}, {"name", m_name}, {"resolution", resolution}});
  screen.insert("screen color", QVariant(lvgl.screenColor()).toString());
  QJsonObject lvgl(
      {{"lvgl", screen}, {"images", imageArr}, {"fonts", fontArr}});
  QJsonDocument doc(lvgl);

  file.write(doc.toJson());
  file.close();

  m_fileName = fileName;

  return true;
}

bool LVGLProject::exportCode(const QString &path) const {
  QDir dir(path);
  QFile file;
  QTextStream stream;
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  const QString name = m_name.toLower();
  const QString codeName = m_name.toLower().replace(" ", "_");
  const QString defName = m_name.toUpper().replace(" ", "_");

  file.setFileName(dir.path() + "/" + name + ".h");
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);

  auto objects = lvgl.allObjects();

  stream << "#ifndef " << defName << "_H\n#define " << defName << "_H\n\n";
  stream << "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";
  // includes
  stream << "/**********************\n";
  stream << " *      INCLUDES\n";
  stream << " **********************/\n\n";
  stream << "#include \"lvgl/lvgl.h\"\n\n";
  stream << "/**********************\n";
  stream << " *       WIDGETS\n";
  stream << " **********************/\n";
  for (LVGLObject *o : objects) {
    Q_ASSERT(!o->doesNameExists());
    if (o->isAccessible())
      stream << "extern lv_obj_t *" << o->codeName() << ";\n";
  }

  stream << "\n";

  int tabindex =
      LVGLHelper::getInstance().getMainW()->getTabW()->currentIndex();
  for (auto o : objects) {
    stream << QString("extern lv_obj_t* %1_ev_%2;\n")
                  .arg(o->codeName())
                  .arg(tabindex);
  }

  stream << "\n";
  // global prototypes
  stream << "/**********************\n";
  stream << " * GLOBAL PROTOTYPES\n";
  stream << " **********************/\n\n";
  stream << "lv_obj_t* " << codeName << "_create();\n\n";
  stream << "#ifdef __cplusplus\n} /* extern \"C\" */\n#endif\n\n";
  stream << "#endif /*" << defName << "_H*/";
  file.close();

  file.setFileName(dir.path() + "/" + name + ".c");
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  stream << "#include \"" << name << ".h\"\n";
  stream << "#include \"app.h\"\n\n";

  // static variables
  stream << "/**********************\n";
  stream << " *       WIDGETS\n";
  stream << " **********************/\n";
  for (LVGLObject *o : objects) {
    if (o->isAccessible()) stream << "lv_obj_t *" << o->codeName() << ";\n";
  }

  stream << "\n";
  stream << "/**********************\n";
  stream << " *  STATIC VARIABLES\n";
  stream << " **********************/\n";

  auto images = lvgl.images();
  for (LVGLImageData *img : images) {
    img->saveAsCode(dir.path() + "/" + img->codeName() + ".c");
    stream << "LV_IMG_DECLARE(" << img->codeName() << ");\n";
  }
  stream << "\n";

  auto fonts = lvgl.customFonts();
  for (const LVGLFontData *f : fonts) {
    f->saveAsCode(dir.path() + "/" + f->codeName() + ".c");
    stream << "LV_FONT_DECLARE(" << f->codeName() << ");\n";
  }
  stream << "\n";

  tabindex = LVGLHelper::getInstance().getMainW()->getTabW()->currentIndex();

  for (LVGLObject *o : objects) {
    stream << QString("lv_obj_t* %1_ev_%2;\n").arg(o->codeName()).arg(tabindex);
  }
  stream << "\n";

  // application
  stream << "lv_obj_t* " << codeName << "_create(){\n";
  stream << "\t"
         << "lv_obj_t *parent = lv_obj_create(NULL, NULL);\n";

  if (lvgl.screenColorChanged()) {
    QString color = QVariant(lvgl.screenColor()).toString().replace("#", "0x");
    stream << "\t_lv_obj_set_style_local_color(parent,0,LV_STYLE_BG_COLOR,lv_"
              "color_hex("
           << color << "));\n";
  }
  stream << "\n";

  QMap<lv_obj_t *, QList<LVGLEvent *>> &objev =
      LVGLHelper::getInstance().getObjEvents();

  int lvglStateType = 7;
  for (LVGLObject *o : objects) {
    for (int index = 0; index < o->widgetClass()->styles().size(); ++index) {
      stream << "\t"
             << "static lv_style_t " << o->styleCodeName(index) << ";\n";
      stream << "\t"
             << "lv_style_init(&" << o->styleCodeName(index) << ");\n";
      auto codemap = LVGLHelper::getInstance().getMainW()->getCodeMap();
      for (int i = 0; i < lvglStateType; ++i) {
        if (o->widgetClass()->name() == "Dropdown") {
          lv_dropdown_open(o->obj());
          lv_dropdown_open(codemap[o->widgetClass()->type()]);
        }
        QStringList styleset =
            o->codeStyle(o->styleCodeName(index), o->obj(),
                         codemap[o->widgetClass()->type()], index, i);
        for (const QString &s : styleset) stream << "\t" << s << "\n";
      }
      if (o->widgetClass()->name() == "Dropdown") lv_dropdown_close(o->obj());
    }

    QString parent = "parent";
    if (o->parent()) parent = o->parent()->codeName();

    if (IS_PAGE_OF_TABVIEW(o)) {
      for (LVGLProperty *p : o->widgetClass()->specialProperties()) {
        for (const QString &fn : p->function(o))
          stream << "\t" << fn.toUtf8() << "\n";
      }
    } else {
      if (o->isAccessible())
        stream << "\t" << o->codeName() << " = "
               << o->widgetClass()->className() << "_create(" << parent
               << ", NULL);\n";
      else
        stream << "\tlv_obj_t *" << o->codeName() << " = "
               << o->widgetClass()->className() << "_create(" << parent
               << ", NULL);\n";

      // special case for tabview
      if (o->widgetType() == LVGLWidget::TabView) {
        lv_tabview_ext_t *ext =
            reinterpret_cast<lv_tabview_ext_t *>(lv_obj_get_ext_attr(o->obj()));
        for (LVGLObject *child : o->childs()) {
          if ((child->widgetType() == LVGLWidget::Page) &&
              (child->index() >= 0))
            stream << "\tlv_obj_t *" << child->codeName()
                   << " = lv_tabview_add_tab(" << o->codeName() << ", \""
                   << ext->tab_name_ptr[child->index() - 1] << "\");\n";
        }
      }

      auto parts = o->widgetClass()->parts();
      for (int i = 0; i < o->widgetClass()->styles().size(); ++i) {
        QString style = o->styleCodeName(i);
        stream << "\t"
               << "lv_obj_add_style(" << o->codeName() << ", "
               << QString::number(parts[i]) << ", &" << o->styleCodeName(i)
               << ");\n";
      }

      // generate properties for widget
      for (LVGLProperty *p : o->widgetClass()->properties()) {
        for (const QString &fn : p->function(o)) stream << "\t" << fn << "\n";
      }

      if (objev.contains(o->obj())) {
        stream << "\t"
               << QString("lv_obj_set_click(%1, true);").arg(o->codeName())
               << "\n";

        QList<LVGLEvent *> &listev = objev[o->obj()];
        for (auto e : listev) {
          auto strlist = e->objCode(o->codeName());
          for (auto s : strlist) stream << s;
        }
      }

      stream << QString("\n\t%1_ev_%2 = %3;\n")
                    .arg(o->codeName())
                    .arg(tabindex)
                    .arg(o->codeName());

      stream << "\n\n";
    }
  }

  stream << "\t"
         << "return parent;\n";
  stream << "}\n";

  file.close();

  // output font file
  QString fontdir = QDir::currentPath() + "/../../../font/";
  QStringList fontfilelist;
  QSet<QString> &fontname = LVGLHelper::getInstance().getSaveFontName();
  auto itor = fontname.begin();
  for (; itor != fontname.end(); ++itor) fontfilelist.push_back(*itor + ".c");
  for (auto x : fontfilelist) {
    QString filepath = fontdir + x;
    QString copyfilepath = path + "/" + x;

    QFile file(filepath);
    file.copy(copyfilepath);
  }

  return exportCodePlus(path);
}

bool LVGLProject::exportCodePlus(const QString &path) const {
  QDir dir(path);
  QFile file;
  const QString name = "app";
  file.setFileName(dir.path() + "/" + name + ".h");
  if (!file.open(QIODevice::WriteOnly)) return false;

  QTextStream stream;
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  stream.setDevice(&file);
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  int tabindex = tabw->currentIndex();

  stream << "#ifndef APP_H\n";
  stream << "#define APP_H\n\n";
  stream << "#include \"lvgl/lvgl.h\"\n\n";

  QSet<QString> &fontname = LVGLHelper::getInstance().getSaveFontName();
  auto itor = fontname.begin();
  for (; itor != fontname.end(); ++itor)
    stream << "LV_FONT_DECLARE(" + *itor + ");\n";

  stream << "\n";
  auto objs = lvgl.allObjects();
  QMap<lv_obj_t *, QList<LVGLEvent *>> &objevs =
      LVGLHelper::getInstance().getObjEvents();

  auto curtab = static_cast<LVGLTabWidget *>(tabw->widget(tabindex));
  curtab->setAllObjects(lvgl.allObjects());
  curtab->setAllImages(lvgl.allImages());

  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      if (objevs.contains(o->obj())) {
        QList<LVGLEvent *> &listev = objevs[o->obj()];
        for (auto e : listev) {
          stream << e->eventHeadCode();
        }
      }
    }
  }

  stream << "\n";
  stream << "void app();\n";
  stream << "\n";

  stream << "#endif";
  file.close();

  file.setFileName(dir.path() + "/" + name + ".c");
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));

  auto tabW = LVGLHelper::getInstance().getMainW()->getTabW();
  stream << "#include \"app.h\"\n";
  for (int i = 0; i < tabW->count(); ++i) {
    stream << "#include \"page_" << QString::number(i + 1) << ".h\"\n";
  }
  stream << "#include \"stdlib.h\"\n";
  stream << "\n";

  for (int i = 0; i < tabW->count(); ++i) {
    stream << "static lv_obj_t *page" + QString::number(i + 1) + ";\n";
  }

  stream << "\n";

  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      QList<LVGLEvent *> &listev = objevs[o->obj()];
      for (auto e : listev) {
        auto lists = e->eventCode();
        for (auto s : lists) stream << s;
        stream << "\n\n";
      }
    }
  }

  stream << "\n";
  stream << "void app(){\n";

  for (int i = 0; i < tabW->count(); ++i) {
    stream << "\t"
           << "page" << QString::number(i + 1) << " = page_"
           << QString::number(i + 1) << "_create();\n";
  }

  stream << "\n";
  stream << "\t"
         << "lv_scr_load(page1);\n";
  stream << "}";

  return true;
}

QString LVGLProject::fileName() const { return m_fileName; }
