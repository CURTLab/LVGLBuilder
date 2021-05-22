#ifndef LVGLWIDGETLISTVIEW_H
#define LVGLWIDGETLISTVIEW_H

#include <QListView>

class LVGLWidgetListView : public QListView
{
	Q_OBJECT
public:
	explicit LVGLWidgetListView(QWidget *parent = nullptr);

signals:

public slots:

protected:
	virtual void startDrag(Qt::DropActions supportedActions);

};

#endif // LVGLWIDGETLISTVIEW_H
