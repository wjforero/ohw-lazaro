#ifndef CGLVIEWERDOCUMENT_H
#define CGLVIEWERDOCUMENT_H

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "cglview.h"

/**
 */
class CglDocument {
public:
  /**
   */
  CglDocument() {  
  }
public:
  /**
   */
  void setModified(const bool& mod) {
		if(mod){
			unsigned int i=0;      
			for(i=0;i<m_vectView.size();i++) {
				m_vectView[i]->updateView();
			}
		}
  }
  /**
   */
  void addView(CglView* view) {
    m_vectView.push_back(view);
  }
public:
  /**
   */
  virtual bool onNewDocument() {
    return true;
  }
  /**
   */
  virtual void onOpen(string pathFile) {
  }
  /**
   */
  virtual bool onCloseDocument() {
    return true;
  }
  /**
   */
  virtual void onSaveDocument() {  
  }
protected:
  /**
   */
  vector<CglView*> m_vectView;
};

#endif