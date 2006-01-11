//--------------------------------------------------------------------------
#ifndef HEPMC_WEIGHT_CONTAINER_H
#define HEPMC_WEIGHT_CONTAINER_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Container for the Weights associated with an event or vertex.
// Basically just an interface to STL vector.
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

namespace HepMC {

    class WeightContainer {

    public:
	WeightContainer( unsigned int n = 0, const double& value = 0. );
	WeightContainer( const std::vector<double>& weights );
	WeightContainer( const WeightContainer& in );
	virtual ~WeightContainer();

	WeightContainer& operator=( const WeightContainer& );
	WeightContainer& operator=( const std::vector<double>& in );

	void          print( std::ostream& ostr = std::cout ) const;

	int           size() const;
	bool          empty() const;
	void          push_back( const double& );
	void          pop_back();
	void          clear();

	double&       operator[]( unsigned int n );  // unchecked access
	const double& operator[]( unsigned int n ) const;
	
	double&       front();         // returns the first element
	const double& front() const;   
	double&       back();          // returns the last element
	const double& back() const;

	typedef std::vector<double>::iterator iterator;
	typedef std::vector<double>::const_iterator const_iterator;
	iterator            begin();
	iterator            end();
	const_iterator      begin() const;
	const_iterator      end() const;
	
    private:
	std::vector<double>  m_weights;
    };

    ///////////////////////////
    // INLINES               //
    ///////////////////////////

    inline WeightContainer::WeightContainer( unsigned int n,
					     const double& value ) 
	: m_weights(n,value)
    {}

    inline WeightContainer::WeightContainer( const std::vector<double>& wgts )
	: m_weights(wgts)
    {}

    inline WeightContainer::WeightContainer( const WeightContainer& in )
	: m_weights(in.m_weights)
    {}

    inline WeightContainer::~WeightContainer() {}

    inline WeightContainer& WeightContainer::operator=
    ( const WeightContainer& in ) {
	m_weights = in.m_weights;
	return *this;
    }

    inline WeightContainer& WeightContainer::operator=
    ( const std::vector<double>& in ) {
	m_weights = in;
	return *this;
    }

    inline void WeightContainer::print( std::ostream& ostr ) const 
    { 
	for ( const_iterator w = begin(); w != end(); ++w ) 
	{ 
	    ostr << *w << " ";
	}
	ostr << std::endl; 
    }

    inline int WeightContainer::size() const { return m_weights.size(); }

    inline bool WeightContainer::empty() const { return m_weights.empty(); }

    inline void WeightContainer::push_back( const double& value) 
    { m_weights.push_back(value); }

    inline void WeightContainer::pop_back() { m_weights.pop_back(); }

    inline void WeightContainer::clear() { m_weights.clear(); }

    inline double& WeightContainer::operator[]( unsigned int n ) 
    { return m_weights[(int)n]; }

    inline const double& WeightContainer::operator[]( unsigned int n ) const
    { return m_weights[(int)n]; }

    inline double& WeightContainer::front() { return m_weights.front(); }

    inline const double& WeightContainer::front() const 
    { return m_weights.front(); }

    inline double& WeightContainer::back() { return m_weights.back(); }

    inline const double& WeightContainer::back() const 
    { return m_weights.back(); }

    inline WeightContainer::iterator WeightContainer::begin() 
    { return m_weights.begin(); }

    inline WeightContainer::iterator WeightContainer::end() 
    { return m_weights.end(); }

    inline WeightContainer::const_iterator WeightContainer::begin() const 
    { return m_weights.begin(); }

    inline WeightContainer::const_iterator WeightContainer::end() const 
    { return m_weights.end(); }

} // HepMC

#endif  // HEPMC_WEIGHT_CONTAINER_H
//--------------------------------------------------------------------------



