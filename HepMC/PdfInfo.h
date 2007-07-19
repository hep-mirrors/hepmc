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

//! The PdfInfo class stores PDF information

///
/// \class  PdfInfo
/// HepMC::PdfInfo stores additional PDF information for a GenEvent.
/// Creation and use of this information is optional. 
///
class PdfInfo {

public:
  // ---  birth/death:
  //
  /// default constructor
  PdfInfo()
    : m_id1(0), 
      m_id2(0),
      m_x1(0),
      m_x2(0),
      m_scalePDF(0),
      m_pdf1(0),
      m_pdf2(0)
    {}

  /// all values must be provided
  PdfInfo( int i1, int i2, double x1, double x2, double q, double p1, double p2 );

  ~PdfInfo() {}
    
  // ---  copying:
  //
  PdfInfo( PdfInfo const & orig );	//!< copy constructor
  PdfInfo &  operator = ( PdfInfo const & rhs ); //!< make a copy
  void swap( PdfInfo & other );	//!< swap two PdfInfo objects

  // ---  equivalence:
  //
  bool    operator==( const PdfInfo& ) const; //!< check for equality
  bool    operator!=( const PdfInfo& ) const; //!< check for inequality

  // ---  accessors:
    /// flavour code of first parton
    int   id1()                   const { return m_id1; }
    /// flavour code of second parton 
    int   id2()                   const { return m_id2; }
    /// fraction of beam momentum carried by first parton ("beam side") 
    double x1()                   const { return m_x1; } 
    /// fraction of beam momentum carried by second parton ("target side") 
    double x2()                   const { return m_x2; }
    ///  Q-scale used in evaluation of PDF's   (in GeV) 
    double scalePDF()             const { return m_scalePDF;  }
    /// PDF (id1, x1, Q) 
    double pdf1()                 const { return m_pdf1; }
    /// PDF (id2, x2, Q)   
    double pdf2()                 const { return m_pdf2; }

  // ---  mutators:
    /// set flavour code of first parton
    void   set_id1(const int &i)              { m_id1=i; }
    /// set flavour code of second parton 
    void   set_id2(const int &i)              { m_id2=i; }
    /// set fraction of beam momentum carried by first parton ("beam side") 
    void   set_x1(const double &f)            { m_x1=f; }
    /// set fraction of beam momentum carried by second parton ("target side") 
    void   set_x2(const double &f)            { m_x2=f; }
    /// set Q-scale used in evaluation of PDF's   (in GeV) 
    void   set_scalePDF(const double &f)      { m_scalePDF=f;  }
    /// set PDF (id1, x1, Q) 
    void   set_pdf1(const double &f)          { m_pdf1=f; }
    /// set PDF (id2, x2, Q)   
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
inline PdfInfo::PdfInfo( int i1, int i2, double x1, double x2, double q, double p1, double p2 )
    : m_id1(i1), 
      m_id2(i2),
      m_x1(x1),
      m_x2(x2),
      m_scalePDF(q),
      m_pdf1(p1),
      m_pdf2(p2)
   {}

inline PdfInfo::PdfInfo( PdfInfo const & orig )
    : m_id1(orig.m_id1), 
      m_id2(orig.m_id2),
      m_x1(orig.m_x1),
      m_x2(orig.m_x2),
      m_scalePDF(orig.m_scalePDF),
      m_pdf1(orig.m_pdf1),
      m_pdf2(orig.m_pdf2)
   {}

inline PdfInfo &  PdfInfo::operator = ( PdfInfo const & rhs ) 
{
  PdfInfo temp( rhs );
  swap( temp );
  return *this;
}

inline void PdfInfo::swap( PdfInfo & other ) 
{
  std::swap(m_id1, other.m_id1); 
  std::swap(m_id2, other.m_id2);
  std::swap(m_x1, other.m_x1);
  std::swap(m_x2, other.m_x2);
  std::swap(m_scalePDF, other.m_scalePDF);
  std::swap(m_pdf1, other.m_pdf1);
  std::swap(m_pdf2, other.m_pdf2);
}

inline bool    PdfInfo::operator==( const PdfInfo& a ) const
{
    /// equality requires that each member match
    return (    a.id1() == this->id1() 
             && a.id2() == this->id2() 
	     && a.x1() == this->x1() 
	     && a.x2() == this->x2()
	     && a.scalePDF() == this->scalePDF() 
	     && a.pdf1() == this->pdf1()
	     && a.pdf2() == this->pdf2() );
}

inline bool    PdfInfo::operator!=( const PdfInfo& a ) const
{
    /// any nonmatching member generates inequality
    return !( a == *this );
}

} // HepMC

#endif  // HEPMC_PDF_INFO_H
