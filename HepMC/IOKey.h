#ifndef HEPMC_IOKEY_H
#define HEPMC_IOKEY_H
// ----------------------------------------------------------------------
//
// IOKey.h
// Author:  Lynn Garren
//
//  Allowed keys used at the beginning and end of HepMC data dumps
//
// ----------------------------------------------------------------------

#include <string>

namespace HepMC {

static std::string IO_GenEvent_Key("HepMC::IO_GenEvent-START_EVENT_LISTING");
static std::string IO_GenEvent_End("HepMC::IO_GenEvent-END_EVENT_LISTING");
// IO_Ascii and IO_ExtendedAscii are deprecated
// These variables are used by IO_GenEvent to recognize the old formats
static std::string IO_Ascii_Key("HepMC::IO_Ascii-START_EVENT_LISTING");
static std::string IO_ExtendedAscii_Key("HepMC::IO_ExtendedAscii-START_EVENT_LISTING");
static std::string IO_Ascii_End("HepMC::IO_Ascii-END_EVENT_LISTING");
static std::string IO_ExtendedAscii_End("HepMC::IO_ExtendedAscii-END_EVENT_LISTING");

}

#endif // HEPMC_IOKEY_H
