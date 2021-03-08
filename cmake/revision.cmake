FIND_PACKAGE( Subversion )

IF( Subversion_FOUND )
    Subversion_WC_INFO( ${CMAKE_SOURCE_DIR} BUILDSYS )
    SET( BUILDSYS_REVISION ${BUILDSYS_WC_REVISION} )
ELSE( Subversion_FOUND )
    SET( BUILDSYS_REVISION "0" )
ENDIF( Subversion_FOUND )
