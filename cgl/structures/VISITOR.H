#ifndef VISITOR_H
#define VISITOR_H

#include "base.h"

CGL_BEGIN

/** Clase usada en el recorrido de algunas estructuras de datos 
 * para la ejecución de algun procedimiento.
 */
template <class TData>
class Visitor {
public:
  Visitor() {
  }
  virtual ~Visitor() {
  }
public:
  /** Ejecuta el procedimiento solicitado en el recorrido de la 
   * estructura de datos
   */
  virtual bool visit(const TData& data) {
    cout<<"Visitor class visit"<<endl;
    return false;
  }
  /**
   */
  TData& getData() {
    return m_data;
  }
  /**
   */
  void setData(TData data) {
    m_data = data;
  }
  /**
   */
  bool isDone() {
    return false;
  }
private:
  /** Apuntador a un objeto del tipo de dato usado.
   * El uso de este atributo es opcional.
   */
  TData m_data;
};

/**
 */
template <class TData>
class PrePostVisitor : public Visitor<TData> {
public:
  /**
   */
  PrePostVisitor() {
  }
public:
  /**
   */
  virtual TData& preVisit(TData& data) {
    return getData();
  };
  /**
   */
  virtual TData& postVisit(TData& data) {
    return getData();
  };
  /**
   */
  virtual bool isDone() {
    return false;
  };
};

/**
 */
template <class TData>
class PreOrder : public PrePostVisitor<TData> {
  /**
   */
  Visitor<TData>& visitor;
public:
  /**
   */
  PreOrder(Visitor<TData>& v) : visitor (v) {
  }
  /**
   */
  void preVisit(const TData& object) { 
    visitor.visit(object); 
  }
};

/**
 */
template <class TData>
class InOrder : public PrePostVisitor<TData> {
  /**
   */
  Visitor<TData>& visitor;
public:
  /**
   */
  InOrder(Visitor<TData>& v) : visitor (v) {
  }
  /**
   */
  void visit(const TData& object) { 
    visitor.visit(object); 
  }
};

/**
 */
template <class TData>
class PostOrder : public PrePostVisitor<TData> {
  /**
   */
  Visitor<TData>& visitor;
public:
  /**
   */
  PostOrder(Visitor<TData>& v) : visitor (v) {
  }
  /**
   */
  void postVisit(const TData& object) {
    visitor.visit(object); 
  }
};

CGL_END

#endif