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

    //! Container for the Weights associated with an event or vertex.

    ///
    /// \class  WeightContainer
    /// Basically just an interface to STL vector.
    class WeightContainer {

    public:
        /// default constructor
	WeightContainer( unsigned int n = 0, const double& value = 0. );
        /// construct from a vector of weights
	WeightContainer( const std::vector<double>& weights );
        /// copy
	WeightContainer( const WeightContainer& in );
	virtual ~WeightContainer();

        /// swap
        void swap( WeightContainer & other);
        /// copy
	WeightContainer& operator=( const WeightContainer& );
        /// copy
	WeightContainer& operator=( const std::vector<double>& in );

        /// print weights
	void          print( std::ostream& ostr = std::cout ) const;

        /// size of weight container
	int           size() const;
	/// return true if weight container is empty
	bool          empty() const;
	/// push onto weight container
	void          push_back( const double& );
	/// pop from weight container
	void          pop_back();
	/// clear the weight container
	void          clear();

        /// access the weight container
	double&       operator[]( unsigned int n );  // unchecked access
        /// access the weight container
	const double& operator[]( unsigned int n ) const;
	
	/// returns the first element
	double&       front();
	/// returns the first element
	const double& front() const;   
	/// returns the last element
	double&       back();
	/// returns the last element
	const double& back() const;

        /// iterator for the weight container
	typedef std::vector<double>::iterator iterator;
        /// const iterator for the weight container
	typedef std::vector<double>::const_iterator const_iterator;
	/// begining of the weight container
	iterator            begin();
	/// end of the weight container
	iterator            end();
	/// begining of the weight container
	const_iterator      begin() const;
	/// end of the weight container
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

    inline void WeightContainer::swap( WeightContainer & other)
    { m_weights.swap( other.m_weights ); }

    inline WeightContainer& WeightContainer::operator=
    ( const WeightContainer& in ) 
    {
        /// best practices implementation
	WeightContainer tmp( in );
	swap( tmp );
	return *this;
    }

    inline WeightContainer& WeightContainer::operator=
    ( const std::vector<double>& in ) {
        /// best practices implementation
	WeightContainer tmp( in );
	swap( tmp );
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



