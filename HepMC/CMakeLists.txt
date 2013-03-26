
set( pkginclude_HEADERS 
		    CompareGenEvent.h
		    enable_if.h
		    Flow.h
		    GenCrossSection.h
		    GenEvent.h
		    GenParticle.h
		    GenVertex.h
		    HeavyIon.h
		    HepMCDefs.h
                    IO_Exception.h
		    is_arithmetic.h
		    PdfInfo.h
		    Polarization.h
		    SearchVector.h
		    SimpleVector.h
		    SimpleVector.icc
		    StreamHelpers.h
		    Units.h
		    Version.h
		    WeightContainer.h
		    LinkDef.h
		    )

INSTALL (FILES ${pkginclude_HEADERS}  
         DESTINATION include/HepMC )
