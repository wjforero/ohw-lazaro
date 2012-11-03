#ifndef CELL_H
#define CELL_H

CGL_BEGIN

/** Una celda en un espacio afin definido
 */
template<class TAffineSpace>
class Cell {
public:
  /**
   */
  typedef Cell<TAffineSpace> TSelf;
  /**
   */
  typedef typename TAffineSpace TSpace;
  /**
   */
  typedef typename TAffineSpace::TAffinePoint TPoint;
  /**
   */
  typedef typename TAffineSpace::TAffineVector TVector;
public:
  /**
   */
  Cell() {
    m_position = 0;
    m_size = 0;
  }
  /**
   */
  Cell(const TPoint& pos, const TVector& size) {
    setPosition(pos);
    setSize(size);
  }
  /**
   */
  ~Cell() {
  }
public:
  /**
   */
  TPoint& getPosition() const {
    return *m_position;
  }
  /**
   */
  TPoint& getPosition() {
    return *m_position;
  }
  /**
   */
  void setPosition(const TPoint& position) {
    if(m_position!=0)
      delete m_position;
    m_position = new TPoint(position);
  }
  /**
   */
  TVector& getSize() const {
    return *m_size;
  }
  /**
   */
  TVector& getSize() {
    return *m_size;
  }
  /**
   */
  void setSize(const TVector& size) {
    if(m_size!=0)
      delete m_size;
    m_size = new TVector(size);
  }
private:
  /**
   */
  TPoint* m_position;
  /**
   */
  TVector* m_size;
   
};

CGL_END

#endif