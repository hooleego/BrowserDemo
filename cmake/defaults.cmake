# Set a default build type for single-configuration CMake generators if no build type is set
IF( NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release )
ENDIF( NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )

# --------------------------------------------------------------------
# Platform specific configurations
# --------------------------------------------------------------------
# Put all runtimes/libraries in the same directory
# MESSAGE("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
# IF( NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY )
#     SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
# ENDIF( NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY )
# IF( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
#     SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
# ENDIF( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
# IF( NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY )
#     SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
# ENDIF( NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY )

IF( WIN32 )
    IF( CYGWIN )
        MESSAGE( FATAL_ERROR "Cygwin is NOT supported, use MinGW or MSVC to build." )
    ENDIF( CYGWIN )

    IF( MSYS OR MINGW )
        SET( CMAKE_SHARED_LIBRARY_PREFIX "" )
        #SET( CMAKE_IMPORT_LIBRARY_PREFIX "" )
        #SET( CMAKE_IMPORT_LIBRARY_SUFFIX ".lib" )
        #SET( CMAKE_STATIC_LIBRARY_PREFIX "lib" )
        #SET( CMAKE_STATIC_LIBRARY_SUFFIX ".lib" )
    ENDIF( MSYS OR MINGW )

    #SET( CMAKE_DEBUG_POSTFIX "d" )

    IF( MSVC )
        ADD_DEFINITIONS(
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_WARNINGS
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_SCL_SECURE_NO_WARNINGS
        )
    ENDIF( MSVC )

    # We don't support anything below w2k and all winapi calls are unicodes
    SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0501 -DUNICODE -D_UNICODE" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0501 -DUNICODE -D_UNICODE" )

    ### FIXME Need this for WinMain entry for gcc compiler on Windows
    #IF( CMAKE_COMPILER_IS_GNUCC )
    #    SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DQT_NEEDS_QMAIN" )
    #ENDIF( CMAKE_COMPILER_IS_GNUCC )
    #IF( CMAKE_COMPILER_IS_GNUCXX )
    #    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NEEDS_QMAIN" )
    #ENDIF( CMAKE_COMPILER_IS_GNUCXX )

    # For windres RC compiler, set codepage to UTF-8
    GET_FILENAME_COMPONENT( _rcc ${CMAKE_RC_COMPILER} NAME_WE )
    STRING( TOLOWER ${_rcc} _rcc )
    IF( _rcc STREQUAL "rc" )
        SET( CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /c 65001" )
    ELSEIF( _rcc STREQUAL "windres" )
        SET( CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --codepage=65001" )
    ENDIF()
ENDIF( WIN32 )

# --------------------------------------------------------------------
# Compiler specific configurations
# --------------------------------------------------------------------
IF( MSVC )
    # Qt disables the native wchar_t type, do it too to avoid linking issues
    #SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zc:wchar_t-" )
ENDIF( MSVC )

IF( CMAKE_COMPILER_IS_GNUCC )
    SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fmessage-length=0" )
ENDIF( CMAKE_COMPILER_IS_GNUCC )

IF( CMAKE_COMPILER_IS_GNUCXX )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fmessage-length=0" )
ENDIF( CMAKE_COMPILER_IS_GNUCXX )

# This platform information is missing in CMake <= 2.8.4.
# IF( NOT CMAKE_LINK_DEF_FILE_FLAG AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" )
#     SET( CMAKE_LINK_DEF_FILE_FLAG "-Wl," )
# ENDIF()

# --------------------------------------------------------------------
# Installation layout
# --------------------------------------------------------------------
IF( WIN32 )
    SET( BIN_INSTALL_DIR . )
    SET( LIB_INSTALL_DIR . )
    SET( DOC_INSTALL_DIR . )
ELSE( WIN32 )
    SET( BIN_INSTALL_DIR bin )
    SET( LIB_INSTALL_DIR lib )
    SET( DOC_INSTALL_DIR share/doc )
ENDIF( WIN32 )

IF( APPLE )
    SET( BUNDLE_INSTALL_DIR . )
ENDIF( APPLE )

IF( WIN32 )
    SET(
        INSTALL_TARGETS_DEFAULT_ARGS
        RUNTIME DESTINATION "${BIN_INSTALL_DIR}" COMPONENT RUNTIME
    )
ELSE( WIN32 )
    SET(
        INSTALL_TARGETS_DEFAULT_ARGS
        RUNTIME DESTINATION "${BIN_INSTALL_DIR}" COMPONENT RUNTIME
        LIBRARY DESTINATION "${LIB_INSTALL_DIR}" COMPONENT DEVEL
        ARCHIVE DESTINATION "${LIB_INSTALL_DIR}" COMPONENT DEVEL
    )
ENDIF( WIN32 )

IF( APPLE )
    SET(
        INSTALL_TARGETS_DEFAULT_ARGS
        ${INSTALL_TARGETS_DEFAULT_ARGS}
        BUNDLE DESTINATION "${BUNDLE_INSTALL_DIR}" COMPONENT RUNTIME
    )
ENDIF( APPLE )
