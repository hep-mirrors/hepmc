//----------------------------------------------------------------------
//  CleverSteram, written 2007-2011 by Gavin Salam and Gregory Soyez
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

#ifndef HAVENOBOOST
#include "CleverStream.hh"

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>


CleverOFStream::CleverOFStream(const std::string & filename) : 
  CleverFStream(filename,false), boostio::filtering_ostream() {
  switch (type()) {
  case gz:
  case bz2:
  case plain:
    if      (type() == gz ) {push(boostio::gzip_compressor());}
    else if (type() == bz2) {push(boostio::bzip2_compressor());}
    push(ofstr());
    break;
  case stdio:
    push (std::cout);
    break;
  default:
    std::cerr << "ERROR: Unrecognised CleverFStream type" << std::endl;
    exit(-1);
  }
}

CleverIFStream::CleverIFStream(const std::string & filename) : 
    CleverFStream(filename,true), boostio::filtering_istream() {
    switch ( type()) {
    case gz:
    case bz2:
    case plain:
      if      (type() == gz ) {push(boostio::gzip_decompressor());}
      else if (type() == bz2) {push(boostio::bzip2_decompressor());}
      push(ifstr());
      break;
    case stdio:
      push (std::cin);
      break;
    default:
      std::cerr << "ERROR: Unrecognised CleverFStream type" << std::endl;
      exit(-1);
    }
  }

#endif // ifndef HAVENOBOOST
