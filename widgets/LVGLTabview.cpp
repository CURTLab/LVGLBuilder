#include "LVGLTabview.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyTextList.h"
#include "LVGLCore.h"

class LVGLPropertyTabBtnPos : public LVGLPropertyEnum
{
public:
	LVGLPropertyTabBtnPos()
		: LVGLPropertyEnum({"Top", "Bottom", "Left", "Right"})
		, m_values({"LV_TABVIEW_BTNS_POS_TOP", "LV_TABVIEW_BTNS_POS_BOTTOM", "LV_TABVIEW_BTNS_POS_LEFT", "LV_TABVIEW_BTNS_POS_RIGHT"})
	{}

	QString name() const { return "Button position"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) != LV_TABVIEW_BTNS_POS_TOP)
			return QStringList() << QString("lv_tabview_set_btns_pos(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
		return QStringList();
	}

protected:
	int get(LVGLObject *obj) const { return lv_tabview_get_btns_pos(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_tabview_set_btns_pos(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyTabs : public LVGLPropertyTextList
{
public:
	QString name() const { return "Tabs"; }

protected:
	QStringList get(LVGLObject *obj) const {
		QStringList ret;
		lv_tabview_ext_t * ext = reinterpret_cast<lv_tabview_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		for (uint16_t i = 0; i < ext->tab_cnt; ++i)
			ret << QString(ext->tab_name_ptr[i]);
		return ret;
	}

	void set(LVGLObject *obj, QStringList list) {
		lv_tabview_ext_t * ext = reinterpret_cast<lv_tabview_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		// rename
		for (uint16_t i = 0; i < qMin(ext->tab_cnt, static_cast<uint16_t>(list.size())); ++i) {
			QByteArray name = list.at(i).toLatin1();
			if (strcmp(ext->tab_name_ptr[i], name.data()) == 0)
				continue;

			char * name_dm = reinterpret_cast<char*>(lv_mem_alloc(name.size()));
			if (name_dm == nullptr)
				continue;

			memcpy(name_dm, name, name.size());
			name_dm[name.size()] = '\0';
			ext->tab_name_ptr[i] = name_dm;

			lv_btnm_set_map(ext->btns, ext->tab_name_ptr);
			lv_btnm_set_btn_ctrl(ext->btns, ext->tab_cur, LV_BTNM_CTRL_NO_REPEAT);
		}

		// add new
		for (uint16_t i = ext->tab_cnt; i < list.size(); ++i) {
			//lv_tabview_add_tab(obj->obj(), qPrintable(list.at(i)));
			lv_obj_t *page_obj = lv_tabview_add_tab(obj->obj(), qPrintable(list.at(i)));
			LVGLObject *page = new LVGLObject(page_obj, lvgl.widget("lv_page"), obj, false, i);
			lvgl.addObject(page);
		}
	}

};

class LVGLPropertyTabCurrent : public LVGLPropertyInt
{
public:
	LVGLPropertyTabCurrent() : LVGLPropertyInt(0, UINT16_MAX) {}

	QString name() const { return "Current tab"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_tabview_set_tab_act(%1, %2, LV_ANIM_OFF);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_tabview_get_tab_act(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_tabview_set_tab_act(obj->obj(), static_cast<uint16_t>(value), LV_ANIM_OFF); }
};

class LVGLPropertyTabBtnHide : public LVGLPropertyBool
{
public:
	QString name() const { return "Hide buttons"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_tabview_set_btns_hidden(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_tabview_get_btns_hidden(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_tabview_set_btns_hidden(obj->obj(), boolean); }
};

class LVGLPropertyTabAnimTime : public LVGLPropertyInt
{
public:
	LVGLPropertyTabAnimTime() : LVGLPropertyInt(0, UINT16_MAX) {}

	QString name() const { return "Animation time"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_preload_set_spin_time(%1, %2);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_preload_get_spin_time(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_preload_set_spin_time(obj->obj(), static_cast<uint16_t>(value)); }
};

class LVGLPropertyTabSliding : public LVGLPropertyBool
{
public:
	QString name() const { return "Sliding"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_tabview_set_sliding(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_tabview_get_sliding(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_tabview_set_sliding(obj->obj(), boolean); }
};

class LVGLPropertyTabScrollbars : public LVGLPropertyEnum
{
public:
	LVGLPropertyTabScrollbars()
		: LVGLPropertyEnum(QStringList() << "Off" << "On" << "Drag" << "Auto")
		, m_values({"LV_SB_MODE_OFF", "LV_SB_MODE_ON", "LV_SB_MODE_DRAG", "LV_SB_MODE_AUTO"})
	{}

	QString name() const { return "Scrollbars"; }

protected:
	int get(LVGLObject *obj) const {
		lv_sb_mode_t mode = LV_SB_MODE_AUTO;
		for (uint16_t i = 0; i < lv_tabview_get_tab_count(obj->obj()); ++i) {
			lv_obj_t *page = lv_tabview_get_tab(obj->obj(), i);
			mode = lv_page_get_sb_mode(page);
		}
		return mode;
	}
	void set(LVGLObject *obj, int index) {
		for (uint16_t i = 0; i < lv_tabview_get_tab_count(obj->obj()); ++i) {
			lv_obj_t *page = lv_tabview_get_tab(obj->obj(), i);
			lv_page_set_sb_mode(page, index & 0xff);
		}
	}

	QStringList m_values;
};

LVGLTabview::LVGLTabview()
{
	m_properties << new LVGLPropertyTabs;
	m_properties << new LVGLPropertyTabBtnPos;
	m_properties << new LVGLPropertyTabCurrent;
	m_properties << new LVGLPropertyTabBtnHide;
	m_properties << new LVGLPropertyTabSliding;
	//m_properties << new LVGLPropertyTabScrollbars;

	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_INDIC
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BTN_BG
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BTN_REL
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BTN_PR
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BTN_TGL_REL
	m_editableStyles << LVGL::Body; // LV_TABVIEW_STYLE_BTN_TGL_PR
}

QString LVGLTabview::name() const
{
	return "Tabview";
}

QString LVGLTabview::className() const
{
	return "lv_tabview";
}

LVGLWidget::Type LVGLTabview::type() const
{
	return Tabview;
}

QPixmap LVGLTabview::preview() const
{
	return QPixmap();
}

QIcon LVGLTabview::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLTabview::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_tabview_create(parent, nullptr);
	return obj;
}

QSize LVGLTabview::minimumSize() const
{
	return QSize(100, 150);
}

QStringList LVGLTabview::styles() const
{
	return QStringList() << "LV_TABVIEW_STYLE_BG"
								<< "LV_TABVIEW_STYLE_INDIC"
								<< "LV_TABVIEW_STYLE_BTN_BG"
								<< "LV_TABVIEW_STYLE_BTN_REL"
								<< "LV_TABVIEW_STYLE_BTN_PR"
								<< "LV_TABVIEW_STYLE_BTN_TGL_REL"
								<< "LV_TABVIEW_STYLE_BTN_TGL_PR";
}

lv_style_t *LVGLTabview::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_tabview_get_style(obj, type & 0xff));
}

void LVGLTabview::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_tabview_set_style(obj, static_cast<lv_tabview_style_t>(type), style);
}

lv_style_t *LVGLTabview::defaultStyle(int type) const
{
	if (type == LV_TABVIEW_STYLE_BG)
		return &lv_style_plain;
	else if (type == LV_TABVIEW_STYLE_INDIC)
		return &lv_style_plain_color;
	else if (type == LV_TABVIEW_STYLE_BTN_BG)
		return &lv_style_transp;
	else if (type == LV_TABVIEW_STYLE_BTN_REL)
		return &lv_style_btn_rel;
	else if (type == LV_TABVIEW_STYLE_BTN_PR)
		return &lv_style_btn_pr;
	else if (type == LV_TABVIEW_STYLE_BTN_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_TABVIEW_STYLE_BTN_TGL_PR)
		return &lv_style_btn_tgl_pr;
	return nullptr;
}
