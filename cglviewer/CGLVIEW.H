#ifndef CGLVIEW_H
#define CGLVIEW_H

class CglDocument;

/**
 */
class CglView {
public:
  /**
   */
  virtual void updateView()=0;
  /**
   */
  void setDocument(CglDocument* doc){
    m_doc=doc;
  }
  /**
   */
  CglDocument& getDocument(){
    return *m_doc;
  }
private:
  /**
   */
  CglDocument* m_doc;
};

#endif