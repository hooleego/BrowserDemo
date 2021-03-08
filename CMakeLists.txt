PROJECT( BrowserDemo )

CMAKE_MINIMUM_REQUIRED( VERSION 3.19.2 )

SET( BUILDSYS_VENDOR_NAME "HLG" )
SET( BUILDSYS_PRODUCT_NAME "BrowserDemo" )

SET( BUILDSYS_VERSION_MAJOR "1" )
SET( BUILDSYS_VERSION_MINOR "1" )
SET( BUILDSYS_VERSION_PATCH "0" )
SET( BUILDSYS_VERSION "${BUILDSYS_VERSION_MAJOR}.${BUILDSYS_VERSION_MINOR}.${BUILDSYS_VERSION_PATCH}" )

SET( BUILDSYS_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
SET( BUILDSYS_CMAKE_DIR "${BUILDSYS_ROOT_DIR}/cmake" )
SET( BUILDSYS_CMAKE_MODULES_DIR "${BUILDSYS_CMAKE_DIR}/modules" )
SET( BUILDSYS_SRC_DIR "${BUILDSYS_ROOT_DIR}/src" )
SET( BUILDSYS_TEST_DIR "${BUILDSYS_ROOT_DIR}/test" )
SET( BUILDSYS_TOOLS_DIR "${BUILDSYS_ROOT_DIR}/tools" )
SET( BUILDSYS_LIBS_DIR "${BUILDSYS_ROOT_DIR}/libs" )

# OPTION( BUILDSYS_ENABLE_CREATE_TRANSLATIONS "Create translations (lupdate target)" OFF )
# OPTION( BUILDSYS_NO_OBSOLETE_TRANSLATIONS "Add -no-obsolete option to lupdate" OFF )

# ;-list of directories specifying a search path for CMake modules to be loaded by the include() or find_package() commands
# before checking the default modules that come with CMake.
# By default it is empty, it is intended to be set by the project.
SET( CMAKE_MODULE_PATH ${BUILDSYS_CMAKE_MODULES_DIR} ${CMAKE_MODULE_PATH} )

INCLUDE( ${BUILDSYS_CMAKE_DIR}/revision.cmake )

# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( USE_QT5 true )
SET( QT_MIN_VERSION 5.12.6 )
FIND_PACKAGE( Qt5 REQUIRED COMPONENTS Core Gui Widgets Network )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_AUTORCC ON )

# --------------------------------------------------------------------
# Modules
# --------------------------------------------------------------------
INCLUDE( ${BUILDSYS_CMAKE_DIR}/defaults.cmake )
INCLUDE( ${BUILDSYS_CMAKE_DIR}/macros.cmake )

FIND_PACKAGE(CEF)
IF ( CEF_FOUND )
    SET(CEF_STANDARD_LIBS
        comctl32.lib
        rpcrt4.lib
        shlwapi.lib
        ws2_32.lib
    )
ENDIF( CEF_FOUND )
MESSAGE("CEF_LIBRARY_DEBUG=${CEF_LIBRARY_DEBUG}")
MESSAGE("CEF_LIBRARY_RELEASE=${CEF_LIBRARY_RELEASE}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")

# --------------------------------------------------------------------
# Main sources
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( src )



# --------------------------------------------------------------------
# Additional resources
# --------------------------------------------------------------------
#INSTALL(
#    FILES ChangeLog COPYING README
#    DESTINATION "${DOC_INSTALL_DIR}"
#)
