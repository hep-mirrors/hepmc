//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Container for the Weights associated with an event or vertex.
// Basically just an interface to STL vector with extra map-like attributes
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

#include "HepMC/WeightContainer.h"

namespace HepMC {



  namespace {
    std::string _default_name(size_t n) {
      std::ostringstream name;
      name << n;
      return name.str();
    }
  }

WeightContainer::WeightContainer( size_type n, double value ) 
    : m_weights(n,value), m_names()
{ set_default_names(n); }

void WeightContainer::set_default_names( size_type n )
{
    // internal program used by the constructors
    std::ostringstream name;
    for ( size_type count = 0; count<n; ++count ) 
    { 
	name.str(std::string());
	name << count;
	//FIXME! m_names[name.str()] = count;
    }
}

  WeightContainer::WeightContainer(const std::vector<double>& wgts )
    : m_weights(wgts), m_names()
  {
    for (size_t i = 0; i < m_weights.size(); ++i)
      m_names.push_back(_default_name(i));
  }


  size_t WeightContainer::index(const std::string& key) const {
    if (!has_key(key)) return size(); //< Invalid, cf. end()
    return std::distance(m_names.begin(), std::find(m_names.begin(), m_names.end(), key));
  }

void WeightContainer::pop_back() 
{
    // this needs to remove the last entry in the vector 
    // and ALSO the associated map entry
/*FIXME!
    size_type vit = size() - 1;
    for ( map_iterator m = m_names.begin(); m != m_names.end(); ++m ) 
    { 
        if( m->second == vit ) { 
	    m_names.erase(m->first); 
	    continue;
	}
    }
    m_weights.pop_back(); 
*/
}

  void WeightContainer::push_back(const std::string& key, double wgt) {
    m_names.push_back(key.empty() ? _default_name(m_names.size()) : key);
    m_weights.push_back(wgt);
  }

void WeightContainer::push_back( const double& value) 
{ 
push_back("",value);
}


  void WeightContainer::set(size_t n, const std::string& key, double wgt) {
    // Pad the arrays to contain index n if necessary
    if (n > size()) {
      for (size_t i = 0; i < n+1-size(); ++i) push_back(0);
    }
    // Set the key and value for index n
    if (!key.empty()) m_names[n] = key;
    m_weights[n] = wgt;
  }


  double WeightContainer::get(size_t n, double def) const {
    if (n < 0 || n >= size()) return def;
    return m_weights[n];
  }

  double WeightContainer::get(const std::string& key, double def) const {
    if (!has_key(key)) return def;
    return get(index(key), def);
  }


  double& WeightContainer::operator[](const std::string& key) {
    if (!has_key(key)) push_back(key, 0);
    return m_weights[index(key)];
  }

  const double& WeightContainer::operator[]( const std::string& key) const {
    if (!has_key(key)) throw std::out_of_range("const WeightContainer::operator[] ERROR: key '"+key+"' not found in WeightContainer");
    return m_weights[index(key)];
  }


  bool WeightContainer::operator == (const WeightContainer& other) const {
    if (size() != other.size()) return false;
    if (m_names != other.m_names) return false;
    if (m_weights != other.m_weights) return false;
    return true;
  }

bool WeightContainer::operator!=( const WeightContainer & other ) const
{
   return !(*this == other );
}

  void WeightContainer::print(std::ostream& ostr) const {
    for (size_t i = 0; i < size(); ++i)
      ostr << "(" << key(i) << "," << get(i) << ") ";
    ostr << std::endl;
  }

bool WeightContainer::has_key( const std::string&  i_key ) const
{
    // look up the name in the map
return std::find(m_names.begin(), m_names.end(), i_key) != m_names.end(); 
}

  void WeightContainer::write(std::ostream& ostr) const {
    for (size_t i = 0; i < size(); ++i)
      ostr << "Weight " << std::setw(4) << i << " with name " << std::setw(10) << key(i) << " is " << get(i) << std::endl;
  }


} // HepMC
