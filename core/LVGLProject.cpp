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

QString LVGLProject::getProjectName() const { return m_name; }

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
    LVGLImageData *img = nullptr;
    if (0 == LVGLHelper::getInstance().getLoadMethod()) {
      img = new LVGLImageData(imageArr[i].toObject());
    } else {
      auto object = imageArr[i].toObject();
      QString name = object["name"].toString();
      QString picpath = object["fileName"].toString();
      QString dir, fname;
      int index = 0;
      for (int i = fileName.size() - 1; i >= 0; --i) {
        if (fileName[i] == '/') {
          index = i;
          break;
        }
        if (fileName[i] == '\\') {
          index = i;
          break;
        }
      }
      dir = fileName.mid(0, index);
      index = 0;
      for (int i = picpath.size() - 1; i >= 0; --i) {
        if (picpath[i] == '/') {
          index = i;
          break;
        }
        if (picpath[i] == '\\') {
          index = i;
          break;
        }
      }
      fname = picpath.mid(index, picpath.size() - index);
      QString filepath = dir + fname;
      img = new LVGLImageData(filepath, name);
    }
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
  int exportmethod = LVGLHelper::getInstance().getExportMethod();
  if (exportmethod == 1) {
    if (!dir.exists(path + "/" + codeName)) dir.mkdir(path + "/" + codeName);
    if (!dir.exists(path + "/" + codeName + "/images"))
      dir.mkdir(path + "/" + codeName + "/images");
    if (!dir.exists(path + "/" + codeName + "/fonts"))
      dir.mkdir(path + "/" + codeName + "/fonts");
    dir.setPath(path + "/" + codeName);
  }

  file.setFileName(dir.path() + "/" + codeName + ".h");
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

  file.setFileName(dir.path() + "/" + codeName + ".c");
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  stream << "#include \"" << codeName << ".h\"\n";
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
    if (exportmethod == 1)
      img->saveAsCode(dir.path() + "/images/" + img->codeName() + ".c");
    else
      img->saveAsCode(dir.path() + "/" + img->codeName() + ".c");
    stream << "LV_IMG_DECLARE(" << img->codeName() << ");\n";
  }
  stream << "\n";

  auto fonts = lvgl.customFonts();
  for (const LVGLFontData *f : fonts) {
    if (exportmethod == 1)
      f->saveAsCode(dir.path() + "/fonts/" + f->codeName() + ".c");
    else
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
        stream << "\t"
               << QString("lv_obj_set_event_cb(%1,%2_ev_%3_event);")
                      .arg(o->codeName())
                      .arg(o->codeName())
                      .arg(tabindex)
               << "\n";
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
  QString fontdir = QDir::currentPath() + "/fonts/";
  QStringList fontfilelist;
  auto &fontname = LVGLHelper::getInstance().getSaveFontName();
  for (auto s : fontname) fontfilelist.push_back(s + ".c");
  for (auto x : fontfilelist) {
    QString filepath = fontdir + x;
    QString copyfilepath;
    if (exportmethod == 1)
      copyfilepath = dir.path() + "/fonts/" + x;
    else
      copyfilepath = dir.path() + "/" + x;

    QFile file(filepath);
    file.copy(copyfilepath);
  }

  if (LVGLHelper::getInstance().getNeedSetTime())
    if (!exportTimeFuncs(path)) return false;

  if (exportmethod == 1) {
    exportPageMK(dir.path());
    exportMakeFile(path);
  } else
    fontname.clear();

  return exportCodePlus(path);
}

bool LVGLProject::exportCodePlus(const QString &path) const {
  QDir dir(path);
  QFile file;
  int exportmethod = LVGLHelper::getInstance().getExportMethod();
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

  auto &allfontname = LVGLHelper::getInstance().getAllFontName();
  for (auto s : allfontname) stream << "LV_FONT_DECLARE(" + s + ");\n";

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
        stream << QString(
                      "void %1_ev_%2_event(lv_obj_t *obj, lv_event_t event);\n")
                      .arg(o->codeName())
                      .arg(i);
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
  stream << "#include \"app.h\"\n\n";
  stream << "#include <stdlib.h>\n\n";
  if (LVGLHelper::getInstance().getNeedSetTime())
    stream << "#include \"timefuncs.h\"\n";
  stream << "\n";

  for (int i = 0; i < tabW->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    if (1 == exportmethod)
      stream << QString("#include \"%1/%1.h\"\n")
                    .arg(tab->getfilename().toLower().replace(" ", "_"));
    else
      stream << QString("#include \"%1.h\"\n")
                    .arg(tab->getfilename().toLower().replace(" ", "_"));
  }
  stream << "\n";

  for (int i = 0; i < tabW->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    stream << QString("static lv_obj_t *%1;\n")
                  .arg(tab->getfilename().toLower().replace(" ", "_"));
  }

  stream << "\n";

  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      if (objevs.contains(o->obj())) {
        QList<LVGLEvent *> &listev = objevs[o->obj()];
        for (auto e : listev) {
          auto lists = e->eventCode();
          for (auto s : lists) stream << s;
          stream << "\n\n";
        }
        stream
            << QString(
                   "\nvoid %1_ev_%2_event(lv_obj_t *obj, lv_event_t event){\n")
                   .arg(o->codeName())
                   .arg(i);
        for (auto e : listev)
          stream << "\t" << QString("%1(obj, event);\n").arg(e->getEventName());
        stream << "}\n\n";
      }
    }
  }

  stream << "\n";
  stream << "void app(){\n";

  for (int i = 0; i < tabW->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    stream << "\t"
           << QString("%1 = %2_create();\n")
                  .arg(tab->getfilename().toLower().replace(" ", "_"))
                  .arg(tab->getfilename().toLower().replace(" ", "_"));
  }

  stream << "\n";
  stream << "\t"
         << QString("lv_scr_load(%1);\n")
                .arg(static_cast<LVGLTabWidget *>(tabw->widget(0))
                         ->getfilename()
                         .toLower()
                         .replace(" ", "_"));
  stream << "}";

  return true;
}

bool LVGLProject::exportTimeFuncs(const QString &path) const {
  QString timefuncsdir = QDir::currentPath();
  QString filepath = timefuncsdir + "/exfile/timefuncs";
  QString copyfilepath = path + "/timefuncs";

  QFile file(filepath + ".h");
  file.copy(copyfilepath + ".h");

  QFile file2(filepath + ".c");
  file2.copy(copyfilepath + ".c");
  return true;
}

bool LVGLProject::exportPageMK(const QString &path) const {
  QDir dir(path);
  QFile file;
  QTextStream stream;
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  const QString name = m_name.toLower().replace(" ", "_");
  file.setFileName(dir.path() + "/" + name + ".mk");
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream << QString("include $(LVGL_DIR)/%1/images/%1_images.mk\n").arg(name);
  stream << QString("include $(LVGL_DIR)/%1/fonts/%1_fonts.mk\n").arg(name);
  stream << "\n";

  stream << QString("CSRCS += %1.c\n").arg(name);
  stream << "\n";

  stream << QString("DEPPATH += --dep-path $(LVGL_DIR)/%1\n").arg(name);
  stream << QString("VPATH += :$(LVGL_DIR)/%1\n").arg(name);
  stream << "\n";

  stream << QString("CFLAGS += \"-I$(LVGL_DIR)/%1\"").arg(name);
  stream << "\n";
  file.close();

  QString imagemk = QString("%1/images/%2_images.mk").arg(dir.path()).arg(name);
  file.setFileName(imagemk);
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));

  auto images = lvgl.images();
  for (LVGLImageData *img : images) {
    stream << QString("CSRCS += %1.c\n").arg(img->codeName());
  }
  stream << "\n";

  stream << QString("DEPPATH += --dep-path $(LVGL_DIR)/%1/images\n").arg(name);
  stream << QString("VPATH += :$(LVGL_DIR)/%1/images\n").arg(name);
  stream << "\n";

  stream << QString("CFLAGS += \"-I$(LVGL_DIR)/%1/images\"").arg(name);
  stream << "\n";
  file.close();

  QString fontsmk = QString("%1/fonts/%2_fonts.mk").arg(dir.path()).arg(name);
  file.setFileName(fontsmk);
  if (!file.open(QIODevice::WriteOnly)) return false;
  stream.setDevice(&file);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));

  auto fonts = lvgl.customFonts();
  for (const LVGLFontData *f : fonts) {
    stream << QString("CSRCS += %1.c\n").arg(f->codeName());
  }

  auto &fontname = LVGLHelper::getInstance().getSaveFontName();
  for (auto s : fontname) stream << QString("CSRCS += %1.c\n").arg(s);

  stream << "\n";

  stream << QString("DEPPATH += --dep-path $(LVGL_DIR)/%1/fonts\n").arg(name);
  stream << QString("VPATH += :$(LVGL_DIR)/%1/fonts\n").arg(name);
  stream << "\n";

  stream << QString("CFLAGS += \"-I$(LVGL_DIR)/%1/fonts\"").arg(name);
  stream << "\n";
  file.close();

  return true;
}

bool LVGLProject::exportMakeFile(const QString &path) const {
  QDir dir(path);
  QFile file;
  QTextStream stream;
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  file.setFileName(dir.path() + "/Makefile");

  if (!file.open(QIODevice::WriteOnly)) return false;

  stream.setDevice(&file);
  stream << "#\n"
         << "#Makefile\n"
         << "#\n";
  stream << "\n";

  stream << QString("LVGL_DIR ?= %1\n").arg(path);
  stream << QString("OBJ_DIR ?= $(LVGL_DIR)/obj\n");
  stream << "\n";

  stream << QString("CC = arm-none-linux-gnueabi-gcc\n");
  stream << QString("CFLAGS ?= -w -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)\n");
  stream << QString("LDFLAGS ?= -lpthread -llvgl\n");
  stream << QString("LDFLAGS += -L$(LVGL_DIR)/lib\n");
  stream << QString("CVERSION ?= -std=c99\n");
  stream << QString("TARGET = $(LVGL_DIR)/app\n");
  stream << "\n";

  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    stream << QString("include $(LVGL_DIR)/%1/%1.mk\n")
                  .arg(tab->getfilename().toLower());
  }
  stream << "\n";

  stream << QString("MAINSRC += $(wildcard *.c)\n");
  stream << QString("SRCS =  $(CSRCS) $(MAINSRC)\n");
  stream << QString(
      "OBJS = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(notdir "
      "$(SRCS))))\n");
  stream << QString(
      "DELOBJS = $(addprefix .\\obj\\,$(patsubst %.c,%.o,$(notdir "
      "$(SRCS))))\n");
  stream << "\n";

  stream << QString("all: default\n");
  stream << "\n";

  stream << QString("$(OBJ_DIR)/%.o: %.c\n");
  stream << QString("	@$(CC) $(CVERSION) $(CFLAGS) -c $< -o $@\n");
  stream << QString("	@echo \"CC $<\"\n");
  stream << "\n";

  stream << QString("default: $(OBJS)\n");
  stream << QString("	$(CC) -o $(TARGET) $^ $(LDFLAGS) $(CVERSION)\n");
  stream << "\n";

  stream << QString(".PHONY: clean\n");
  stream << QString("clean:\n");
  stream << QString("	del app\n");
  stream << QString("	del $(DELOBJS)\n");

  file.close();
  return true;
}

QString LVGLProject::fileName() const { return m_fileName; }
