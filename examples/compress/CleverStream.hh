#ifndef  __CLEVERSTREAM_HH__
#define __CLEVERSTREAM_HH__

//----------------------------------------------------------------------
//  CleverStream, written 2007-2011 by Gavin Salam and Gregory Soyez 
//
//  CleverStream is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  CleverStream is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>

#ifndef HAVENOBOOST
//--------------------------------------
// have a version which really is clever in the case where
// we have boost

// boost-related stuff
#include <boost/iostreams/filtering_stream.hpp>
namespace boostio=boost::iostreams;

#include <memory>
#include <cstdlib>
#include <algorithm>

//----------------------------------------------------------------------
/// \class CleverFStream 
///
/// a helper class that underlies CleverOFStream and CleverIFStream,
/// providing functionality to detect the appropriate filetype and
/// open the relevant raw filestream
///
class CleverFStream {
public:
  /// ctr:
  /// \param name  the filename
  /// \param in    true if it's being used for input
  CleverFStream(const std::string & name, bool in) : _name(name) {
    if (type() != stdio) {
      if (in) {
        _ifstr.reset( new std::ifstream(_name.c_str()));
        if (! _ifstr->good() ) {
          std::cerr << "ERROR: could not open " << _name << std::endl;
          exit(-1);
        }
      } else {
        _ofstr.reset( new std::ofstream(_name.c_str()) );
        if (! _ofstr->good() ) {
          std::cerr << "ERROR: could not open " << _name << std::endl;
          exit(-1);
        }
      }
    }
  }

  /// return a reference to the raw input stream
  std::istream & ifstr() {return *_ifstr;}
  /// return a reference to the raw output stream
  std::ostream & ofstr() {return *_ofstr;}

  /// dtr that does nothing
  virtual ~CleverFStream() {}

  /// return the name
  const std::string & name() {return _name;}

  /// a list of file types
  enum Type {plain,gz,bz2,stdio};

  /// return the type of file we're dealing with 
  Type type() const {
    if (_name.find(std::string(".gz"), std::max(0,int(_name.length()-3)))
        != std::string::npos) { return gz;}
    else if (_name.find(std::string(".bz2"), std::max(0,int(_name.length()-4)))
        != std::string::npos) { return bz2;}
    else if (_name == "-") {return stdio;}
    else {return plain;}
  }

private:
  std::string _name;
  std::auto_ptr<std::ofstream> _ofstr;
  std::auto_ptr<std::ifstream> _ifstr;
};


//----------------------------------------------------------------------
/// a class that provides an ostream that automatically deals with
/// compression according to the filename
///
/// This replacement for ofstream has the following behaviour
///  - if the filename ends in .gz or .bz2 it is compressed on the fly
///  - if the filename is "-", output is sent to standard output
///  - otherwise the file is written normally
///
// NB: ordering of CleverFstream and filtering stream is crucial in the definition
//     otherwise destruction proceeds in wrong order
class CleverOFStream : public CleverFStream,
                       public boostio::filtering_ostream {
public:
  /// Constructor, which just takes a filename for output
  CleverOFStream(const std::string & filename);
private:
  std::auto_ptr<std::ofstream> _ofstr;
};


//----------------------------------------------------------------------
/// a class that provides an istream that automatically deals with
/// compression according to the filename
///
/// This replacement for ifstream has the following behaviour
///  - if the filename ends in .gz or .bz2 it is decompressed on the fly
///  - if the filename is "-", input is read from standard input
///  - otherwise the file is read normally
///
// NB: ordering of CleverFstream and filtering stream is crucial in the definition
//     otherwise destruction proceeds in wrong order
class CleverIFStream : public CleverFStream, 
                       public boostio::filtering_istream  {
public:
  /// Constructor, which just takes a filename for input
  CleverIFStream(const std::string & filename);
private:
  std::auto_ptr<std::ifstream> _ifstr;
};


//---------------------------------------------
#else // wrt ifndef HAVENOBOOST
//---------------------------------------------
// put the dumbed down (boost-free) version here
//---------------------------------------------

//----------------------------------------------------------------------
/// Fallback version of CleverOFStream for use without boost (does not
/// do compression)
///
class CleverOFStream : public std::ofstream {
public:
  CleverOFStream(const std::string & filename) {
    if (filename == "-") open("/dev/stdout");
    else open(filename.c_str());
  }
};

//----------------------------------------------------------------------
/// Fallback version of CleverIFStream for use without boost (does not
/// do decompression)
///
class CleverIFStream : public std::ifstream{
public:
  CleverIFStream(const std::string & filename) {
    if (filename == "-") open("/dev/stdin");
    else open(filename.c_str());
  }
};

#endif // undef HAVENOBOOST


#endif// __CLEVERSTREAM_HH__
