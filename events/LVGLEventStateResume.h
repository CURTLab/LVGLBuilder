#ifndef LVGLEVENTSTATERESUME_H
#define LVGLEVENTSTATERESUME_H
#include <QJsonObject>
#include <QMap>

class LVGLObject;

class LVGLEventStateResume {
 public:
  ~LVGLEventStateResume(){};
  LVGLEventStateResume(const LVGLEventStateResume&) = delete;
  LVGLEventStateResume operator=(const LVGLEventStateResume&) = delete;
  static LVGLEventStateResume& getInstance() {
    static LVGLEventStateResume instance;
    return instance;
  }

  void addEvent(LVGLObject* obj);
  void stateResume();

 private:
  LVGLEventStateResume();

 private:
  static LVGLEventStateResume* m_instance;
  QMap<LVGLObject*, QJsonObject> m_objProp;
};

#endif  // LVGLEVENTSTATERESUME_H
