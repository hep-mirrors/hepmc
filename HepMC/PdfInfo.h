//--------------------------------------------------------------------------
#ifndef HEPMC_PDF_INFO_H
#define HEPMC_PDF_INFO_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
//
// Additional PDF information
//////////////////////////////////////////////////////////////////////////
//
//      int id1; 	// flavour code of first parton
//      int id2; 	// flavour code of second parton 
//      double x1;  	// fraction of beam momentum carried by first parton ("beam side") 
//      double x2;  	// fraction of beam momentum carried by second parton ("target side") 
//      double scalePDF;  //  Q-scale used in evaluation of PDF's   (in GeV) 
//      double pdf1;  	// PDF (id1, x1, Q) 
//      double pdf2;  	// PDF (id2, x2, Q)   
//
//////////////////////////////////////////////////////////////////////////

namespace HepMC {

class PdfInfo {

public:
  // ---  birth/death:
  //
  PdfInfo()
    : m_id1(0), 
      m_id2(0),
      m_x1(0),
      m_x2(0),
      m_scalePDF(0),
      m_pdf1(0),
      m_pdf2(0)
    {}

  inline PdfInfo( int i1, int i2, double x1, double x2, double q, double p1, double p2 );

  ~PdfInfo() {}
    
  // ---  copying:
  //
  inline PdfInfo( PdfInfo const & orig );
  inline PdfInfo &  operator = ( PdfInfo const & rhs );
  inline void swap( PdfInfo & other );

  // ---  equivalence:
  //
  inline bool    operator==( const PdfInfo& ) const;
  inline bool    operator!=( const PdfInfo& ) const;

  // ---  accessors:
    int   id1()                   const { return m_id1; }
    int   id2()                   const { return m_id2; }
    double x1()                   const { return m_x1; }
    double x2()                   const { return m_x2; }
    double scalePDF()             const { return m_scalePDF;  }
    double pdf1()                 const { return m_pdf1; }
    double pdf2()                 const { return m_pdf2; }

  // ---  mutators:
    void   set_id1(const int &i)              { m_id1=i; }
    void   set_id2(const int &i)              { m_id2=i; }
    void   set_x1(const double &f)            { m_x1=f; }
    void   set_x2(const double &f)            { m_x2=f; }
    void   set_scalePDF(const double &f)      { m_scalePDF=f;  }
    void   set_pdf1(const double &f)          { m_pdf1=f; }
    void   set_pdf2(const double &f)          { m_pdf2=f; }

private: // data members
    int   m_id1; 
    int   m_id2; 
    double m_x1;
    double m_x2;
    double m_scalePDF; 
    double m_pdf1;
    double m_pdf2;

};

// inline operators
PdfInfo::PdfInfo( int i1, int i2, double x1, double x2, double q, double p1, double p2 )
    : m_id1(i1), 
      m_id2(i2),
      m_x1(x1),
      m_x2(x2),
      m_scalePDF(q),
      m_pdf1(p1),
      m_pdf2(p2)
   {}

PdfInfo::PdfInfo( PdfInfo const & orig )
    : m_id1(orig.m_id1), 
      m_id2(orig.m_id2),
      m_x1(orig.m_x1),
      m_x2(orig.m_x2),
      m_scalePDF(orig.m_scalePDF),
      m_pdf1(orig.m_pdf1),
      m_pdf2(orig.m_pdf2)
   {}

PdfInfo &  PdfInfo::operator = ( PdfInfo const & rhs ) 
{
  PdfInfo temp( rhs );
  swap( temp );
  return *this;
}

void PdfInfo::swap( PdfInfo & other ) 
{
  std::swap(m_id1, other.m_id1); 
  std::swap(m_id2, other.m_id2);
  std::swap(m_x1, other.m_x1);
  std::swap(m_x2, other.m_x2);
  std::swap(m_scalePDF, other.m_scalePDF);
  std::swap(m_pdf1, other.m_pdf1);
  std::swap(m_pdf2, other.m_pdf2);
}

bool    PdfInfo::operator==( const PdfInfo& a ) const
{
    return (    a.id1() == this->id1() 
             && a.id2() == this->id2() 
	     && a.x1() == this->x1() 
	     && a.x2() == this->x2()
	     && a.scalePDF() == this->scalePDF() 
	     && a.pdf1() == this->pdf1()
	     && a.pdf2() == this->pdf2() );
}

bool    PdfInfo::operator!=( const PdfInfo& a ) const
{
    return !( a == *this );
}

} // HepMC

#endif  // HEPMC_PDF_INFO_H
