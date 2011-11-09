
set( pkginclude_HEADERS 
		    CompareGenEvent.h
		    Flow.h	
		    GenEvent.h
		    GenParticle.h
		    GenVertex.h
		    GenCrossSection.h
		    GenRanges.h
		    HeavyIon.h
		    HEPEVT_Wrapper.h
		    HerwigWrapper.h
		    IO_AsciiParticles.h
		    IO_BaseClass.h
		    IO_Exception.h
		    IO_GenEvent.h
		    IO_HEPEVT.h
		    IO_HERWIG.h
		    IteratorRange.h
		    PdfInfo.h
		    Polarization.h
		    PythiaWrapper6_4.h
		    PythiaWrapper6_4_WIN32.h
		    PythiaWrapper.h
		    WeightContainer.h
		    SearchVector.h
		    SimpleVector.h
		    SimpleVector.icc	
		    StreamHelpers.h
		    StreamInfo.h
		    enable_if.h
		    is_arithmetic.h
		    TempParticleMap.h
		    Units.h
		    Version.h
		    HepMCDefs.h
		    )

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake-defs.h.in 
                ${CMAKE_CURRENT_BINARY_DIR}/defs.h  @ONLY )

INSTALL (FILES ${pkginclude_HEADERS}  
               ${CMAKE_CURRENT_BINARY_DIR}/defs.h
         DESTINATION include/HepMC )
