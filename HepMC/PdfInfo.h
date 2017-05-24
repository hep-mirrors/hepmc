#ifndef HEPMC_PDF_INFO_H
#define HEPMC_PDF_INFO_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
//
// Additional PDF information
//////////////////////////////////////////////////////////////////////////

#include <istream>
#include <ostream>

namespace HepMC {

  /// Additional PDF information for a GenEvent
  ///
  /// Creation and use of this information is optional.
  ///
  ///   - int id1;  // flavour code of first parton
  ///   - int id2;  // flavour code of second parton
  ///   - int pdf_id1;  // LHAPDF set id of first parton (zero by default)
  ///   - int pdf_id2;  // LHAPDF set id of second parton  (zero by default)
  ///   - double x1;    // fraction of beam momentum carried by first parton ("beam side")
  ///   - double x2;    // fraction of beam momentum carried by second parton ("target side")
  ///   - double scalePDF;  //  Q-scale used in evaluation of PDF's   (in GeV)
  ///   - double pdf1;      // PDF (id1, x1, Q)
  ///   - double pdf2;      // PDF (id2, x2, Q)
  ///
  /// Input parton flavour codes id1 & id2 are expected to obey the
  /// PDG code conventions, especially g = 21.
  ///
  /// The contents of pdf1 and pdf2 are expected to be x*f(x).
  /// The LHAPDF set ids are the entries in the first column of
  /// http:///projects.hepforge.org/lhapdf/PDFsets.index
  ///
  class PdfInfo {
  public:

    /// Default constructor
    PdfInfo()
      : m_id1(0),
        m_id2(0),
        m_pdf_id1(0),
        m_pdf_id2(0),
        m_x1(0),
        m_x2(0),
        m_scalePDF(0),
        m_pdf1(0),
        m_pdf2(0)
    { }

    /// Valued constructor
    ///
    /// All values except pdf_id1 and pdf_id2 MUST be provided.
    PdfInfo( int i1, int i2, double x1, double x2,
             double q, double p1, double p2,
             int pdf_id1 = 0, int pdf_id2 = 0 );

    /// Default destructor
    /// @todo Remove?
    ~PdfInfo() { }

    /// Copy constructor
    PdfInfo( PdfInfo const & orig );

    /// Copy assignment
    PdfInfo & operator = ( const PdfInfo & rhs );

    /// Swap two PdfInfo objects' contents
    void swap( PdfInfo & other );

    /// Equality test
    bool operator == ( const PdfInfo& ) const;
    /// Inquality test
    bool operator != ( const PdfInfo& a) const { return !(a == *this); }

    /// @name Accessors
    //@{

    /// Flavour code of first parton
    int   id1() const { return m_id1; }
    /// Flavour code of second parton
    int   id2() const { return m_id2; }
    /// LHAPDF set ID used for first parton's beam
    int   pdf_id1() const { return m_pdf_id1; }
    /// LHAPDF set ID used for second parton's beam
    int   pdf_id2() const { return m_pdf_id2; }
    /// Fraction of beam momentum carried by first parton ("beam side")
    double x1() const { return m_x1; }
    /// Fraction of beam momentum carried by second parton ("target side")
    double x2() const { return m_x2; }
    /// Q-scale used in evaluation of PDF's (in GeV)
    double scalePDF() const { return m_scalePDF;  }
    /// PDF x*f(x) value for (id1, x1, Q)
    double pdf1() const { return m_pdf1; }
    /// PDF x*f(x) value for (id2, x2, Q)
    double pdf2() const { return m_pdf2; }

    //@}

    /// Verify that the instance contains non-zero information
    bool is_valid() const;

    /// @name Mutators
    //@{

    /// Set flavour code of first parton
    /// @todo Just pass an int by value -- no performance saving in an int reference
    void   set_id1(const int &i) { m_id1 = i; }
    /// Set flavour code of second parton
    /// @todo Just pass an int by value -- no performance saving in an int reference
    void   set_id2(const int &i) { m_id2 = i; }
    /// Set LHAPDF set ID of first parton
    /// @todo Just pass an int by value -- no performance saving in an int reference
    void   set_pdf_id1(const int &i) { m_pdf_id1 = i; }
    /// Set LHAPDF set ID of second parton
    /// @todo Just pass an int by value -- no performance saving in an int reference
    void   set_pdf_id2(const int &i) { m_pdf_id2 = i; }
    /// Set fraction of beam momentum carried by first parton ("beam side")
    /// @todo Just pass an int by value -- no performance saving in a double reference
    void   set_x1(const double &f) { m_x1 = f; }
    /// Set fraction of beam momentum carried by second parton ("target side")
    /// @todo Just pass an int by value -- no performance saving in a double reference
    void   set_x2(const double &f) { m_x2 = f; }
    /// Set Q-scale used in evaluation of PDF's (in GeV)
    /// @todo Just pass an int by value -- no performance saving in a double reference
    void   set_scalePDF(const double &f) { m_scalePDF = f;  }
    /// Set x*f(x) of first parton
    /// @todo Just pass an int by value -- no performance saving in a double reference
    void   set_pdf1(const double &f) { m_pdf1 = f; }
    /// Set x*f(x) of second parton
    /// @todo Just pass an int by value -- no performance saving in a double reference
    void   set_pdf2(const double &f) { m_pdf2 = f; }

  private:

    /// Data members
    //@{
    int   m_id1;
    int   m_id2;
    int   m_pdf_id1;
    int   m_pdf_id2;
    double m_x1;
    double m_x2;
    double m_scalePDF;
    double m_pdf1;
    double m_pdf2;
    //@}

  };



  /// Alias for forward compatibility
  typedef PdfInfo GenPdfInfo;


  /// @name PdfInfo free functions
  //@{

  /// Stream output
  std::ostream & operator << (std::ostream &, PdfInfo const *);
  /// Stream input
  std::istream & operator >> (std::istream &, PdfInfo *);


  inline PdfInfo::PdfInfo( int i1, int i2, double xin1, double xin2,
                           double q, double p1, double p2,
                           int pid1, int pid2 )
    : m_id1(i1),
      m_id2(i2),
      m_pdf_id1(pid1),
      m_pdf_id2(pid2),
      m_x1(xin1),
      m_x2(xin2),
      m_scalePDF(q),
      m_pdf1(p1),
      m_pdf2(p2)
  {}

  inline PdfInfo::PdfInfo( PdfInfo const & orig )
    : m_id1(orig.m_id1),
      m_id2(orig.m_id2),
      m_pdf_id1(orig.m_pdf_id1),
      m_pdf_id2(orig.m_pdf_id2),
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
    std::swap(m_pdf_id1, other.m_pdf_id1);
    std::swap(m_pdf_id2, other.m_pdf_id2);
    std::swap(m_x1, other.m_x1);
    std::swap(m_x2, other.m_x2);
    std::swap(m_scalePDF, other.m_scalePDF);
    std::swap(m_pdf1, other.m_pdf1);
    std::swap(m_pdf2, other.m_pdf2);
  }

  inline bool PdfInfo::operator==( const PdfInfo& a ) const
  {
    /// equality requires that each member match
    return (    a.id1() == this->id1()
                && a.id2() == this->id2()
                && a.pdf_id1() == this->pdf_id1()
                && a.pdf_id2() == this->pdf_id2()
                && a.x1() == this->x1()
                && a.x2() == this->x2()
                && a.scalePDF() == this->scalePDF()
                && a.pdf1() == this->pdf1()
                && a.pdf2() == this->pdf2() );
  }

  inline bool PdfInfo::is_valid() const
  {
    if ( m_id1 != 0 ) return true;
    if ( m_id2 != 0 ) return true;
    if ( m_pdf_id1 != 0 ) return true;
    if ( m_pdf_id2 != 0 ) return true;
    if ( m_x1 != 0 ) return true;
    if ( m_x2 != 0 ) return true;
    if ( m_scalePDF != 0 ) return true;
    if ( m_pdf1 != 0 ) return true;
    if ( m_pdf2 != 0 ) return true;
    return false;
  }

} // HepMC

#endif  // HEPMC_PDF_INFO_H
