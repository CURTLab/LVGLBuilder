#ifndef LVGLWORKER_H
#define LVGLWORKER_H

#include <QThread>
#include <QTimer>

class LVGLWorker : public QThread
{
	Q_OBJECT
public:
	LVGLWorker(QObject *parent = nullptr);

	void run();

private slots:
	void tick();

};

#endif // LVGLWORKER_H
