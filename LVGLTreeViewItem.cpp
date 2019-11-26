#include "LVGLTreeViewItem.h"

LVGLTreeViewItem::LVGLTreeViewItem(lv_obj_t *obj, QTreeWidgetItem *parent)
	: QTreeWidgetItem(parent), m_obj(obj)
{
}

lv_obj_t *LVGLTreeViewItem::get_lv_obj() const
{
	return m_obj;
}
