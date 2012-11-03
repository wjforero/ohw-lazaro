/**
 * $Revision$
 * $Date$
 */

#ifndef VISITOR_H
#define VISITOR_H

#include "base.h"

ST_BEGIN

/** Clase usada en el recorrido de algunas estructuras de datos 
 * para la ejecución de algun procedimiento.
 */
template <class TData>
class Visitor {
public:
  /** Constructor por defecto
   */
  Visitor() {
  }
  /** Destructor
   */
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
  /** retorna el dato almacenado 
   */
  TData& getData() {
    return m_data;
  }
  /** modifica el dato almacenado
   */
  void setData(TData data) {
    m_data = data;
  }
  /** retorna verdadero si el objeto a conseguido su objetivo
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

ST_END

#endif