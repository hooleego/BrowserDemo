# This file contains the following macros (or functions):
# BUILDSYS_GET_CONFIGURE_FILE_NAME
# BUILDSYS_ADD_RESOURCES
# BUILDSYS_ADD_LIBRARY_RESOURCES
# BUILDSYS_AUTO_SOURCES
# BUILDSYS_AUTO_LIBRARY_SOURCES
# BUILDSYS_ENABLE_EXCEPTIONS
# BUILDSYS_ENABLE_RTTI

# Get configure file name, by removing .in and .cmake
# Usage: BUILDSYS_GET_CONFIGURE_FILE_NAME( CONFIGURE_FILE_NAME ${CONFIGURE_FILE} ) 
MACRO( BUILDSYS_GET_CONFIGURE_FILE_NAME _out _in )
    STRING( REGEX REPLACE "\\.in" "" _tmp ${_in} )
    STRING( REGEX REPLACE "\\.cmake" "" _tmp ${_tmp} )
    SET( ${_out} ${_tmp} )
ENDMACRO( BUILDSYS_GET_CONFIGURE_FILE_NAME )

# Add database
# Usage: BUILDSYS_ADD_DATABASE( MODULE DB SCHEMA FLAGS ) 
MACRO( BUILDSYS_ADD_DATABASE _db _schema )
    SET( _sources ${_db}db_SOURCES )
    SET( _flags ${ARGN} )
    SET( _force_flags -c ${_flags} )
    FILE( GLOB ${_sources} *.cpp )
    ADD_CUSTOM_TARGET(
        ${_db}db_gen
        COMMAND modelgen -i ${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_SOURCE_DIR} ${_flags} ${_schema}
        DEPENDS modelgen
        SOURCES ${_schema}
        COMMENT "Generating models for ${_schema}"
        VERBATIM
    )
    ADD_CUSTOM_TARGET(
        ${_db}db_regen
        COMMAND modelgen -i ${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_SOURCE_DIR} ${_force_flags} ${_schema}
        DEPENDS modelgen
        SOURCES ${_schema}
        COMMENT "Regenerating models for ${_schema}"
        VERBATIM
    )
ENDMACRO( BUILDSYS_ADD_DATABASE )

# Add databases to module
# Usage: BUILDSYS_ADD_DATABASE_MODULE( MODULE DBS )
MACRO( BUILDSYS_ADD_DATABASE_MODULE _module )
    SET( _sources ${_module}db_module_SOURCES )
    SET( _db_gen )
    SET( _db_regen )
    FOREACH( _db ${ARGN} )
        ADD_SUBDIRECTORY( ${_db} )
        LIST( APPEND _db_gen ${_db}db_gen )
        LIST( APPEND _db_regen ${_db}db_regen )
        SET( _db_sources )
        FILE( GLOB _db_sources ${_db}/*.cpp )
        LIST( APPEND ${_sources} ${_db_sources} )
    ENDFOREACH( _db )
    ADD_CUSTOM_TARGET( ${_module}db_module_gen DEPENDS ${_db_gen} )
    ADD_CUSTOM_TARGET( ${_module}db_module_regen DEPENDS ${_db_regen} )
ENDMACRO( BUILDSYS_ADD_DATABASE_MODULE )

# Add database modules to project
# Usage: BUILDSYS_ADD_DATABASE_PROJECT( MODULES )
MACRO( BUILDSYS_ADD_DATABASE_PROJECT )
    SET( _moduledb_gen )
    SET( _moduledb_regen )
    FOREACH( _module ${ARGN} )
        LIST( APPEND _moduledb_gen ${_module}db_module_gen )
        LIST( APPEND _moduledb_regen ${_module}db_module_regen )
    ENDFOREACH( _module )
    ADD_CUSTOM_TARGET( db_project_gen DEPENDS ${_moduledb_gen} )
    ADD_CUSTOM_TARGET( db_project_regen DEPENDS ${_moduledb_regen} )
ENDMACRO( BUILDSYS_ADD_DATABASE_PROJECT )

# Add resources to sources
# Usage: BUILDSYS_ADD_RESOURCES( SOURCES ${RESOURCES} )
MACRO( BUILDSYS_ADD_RESOURCES _sources )
    IF( WIN32 )
        FOREACH( _rc ${ARGN} )
            BUILDSYS_GET_CONFIGURE_FILE_NAME( _rcname ${_rc} )
            IF( ${_rcname} STREQUAL ${_rc} )
                LIST( APPEND ${_sources} ${_rc} )
            ELSE()
                CONFIGURE_FILE( ${_rc} ${_rcname} )
                LIST( APPEND ${_sources} ${CMAKE_CURRENT_BINARY_DIR}/${_rcname} )
            ENDIF()
        ENDFOREACH( _rc )
    ENDIF( WIN32 )
ENDMACRO( BUILDSYS_ADD_RESOURCES )

# Add resources to library sources
# Usage: BUILDSYS_ADD_LIBRARY_RESOURCES( SOURCES ${RESOURCES} ) 
MACRO( BUILDSYS_ADD_LIBRARY_RESOURCES _sources )
    IF( WIN32 AND BUILD_SHARED_LIBS )
        FOREACH( _rc ${ARGN} )
            BUILDSYS_GET_CONFIGURE_FILE_NAME( _rcname ${_rc} )
            IF( ${_rcname} STREQUAL ${_rc} )
                LIST( APPEND ${_sources} ${_rc} )
            ELSE()
                CONFIGURE_FILE( ${_rc} ${_rcname} )
                LIST( APPEND ${_sources} ${CMAKE_CURRENT_BINARY_DIR}/${_rcname} )
            ENDIF()
        ENDFOREACH( _rc )
    ENDIF( WIN32 AND BUILD_SHARED_LIBS )
ENDMACRO( BUILDSYS_ADD_LIBRARY_RESOURCES )

# Automatically add sources by file extension
# Usage: BUILDSYS_AUTO_SOURCES( OUTPUT ${SOURCES} )
# Following file extensions are checked:
# *.h: QT5_WRAP_CPP
# *.ui: QT5_WRAP_UI
# *.qrc: QT5_ADD_RESOURCES
# *.rc.in: BUILDSYS_ADD_RESOURCES
# *.rc.cmake: BUILDSYS_ADD_RESOURCES
# Others will be add to sources list directly
MACRO( BUILDSYS_AUTO_SOURCES _output )
    SET( _moc )
    SET( _ui )
    SET( _qrc )
    SET( _rc )
    SET( _def )
    FOREACH( _file ${ARGN} )
        IF( ${_file} MATCHES "^.*\\.h$" )
            LIST( APPEND _moc ${_file} )
            LIST( APPEND ${_output} ${_file} ) # Header files are also added to output
        ELSEIF( ${_file} MATCHES "^.*\\.ui$" )
            LIST( APPEND _ui ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.qrc$" )
            LIST( APPEND _qrc ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.rc\\.in$" OR ${_file} MATCHES "^.*\\.rc\\.cmake$" )
            LIST( APPEND _rc ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.def\\.in$" OR ${_file} MATCHES "^.*\\.def\\.cmake$" )
            LIST( APPEND _def ${_file} )
        ELSE()
            LIST( APPEND ${_output} ${_file} )
        ENDIF()
    ENDFOREACH( _file )
	IF( USE_QT5 )
		QT5_WRAP_CPP( ${_output} ${_moc} )
		QT5_WRAP_UI( ${_output} ${_ui} )
		QT5_ADD_RESOURCES( ${_output} ${_qrc} )
	ELSE()
		QT4_WRAP_CPP( ${_output} ${_moc} )
		QT4_WRAP_UI( ${_output} ${_ui} )
		QT4_ADD_RESOURCES( ${_output} ${_qrc} )
	ENDIF()
    BUILDSYS_ADD_RESOURCES( ${_output} ${_rc} )
    BUILDSYS_ADD_RESOURCES( ${_output} ${_def} )
ENDMACRO( BUILDSYS_AUTO_SOURCES )

# Automatically add sources by file extension
# Usage: BUILDSYS_AUTO_LIBRARY_SOURCES( OUTPUT ${SOURCES} )
# Following file extensions are checked:
# *.h: QT5_WRAP_CPP
# *.ui: QT5_WRAP_UI
# *.qrc: QT5_ADD_RESOURCES
# *.rc.in: BUILDSYS_ADD_LIBRARY_RESOURCES
# *.rc.cmake: BUILDSYS_ADD_LIBRARY_RESOURCES
# Others will be add to sources list directly
MACRO( BUILDSYS_AUTO_LIBRARY_SOURCES _output )
    SET( _moc )
    SET( _ui )
    SET( _qrc )
    SET( _rc )
    SET( _def )
    FOREACH( _file ${ARGN} )
        IF( ${_file} MATCHES "^.*\\.h$" )
            LIST( APPEND _moc ${_file} )
            LIST( APPEND ${_output} ${_file} ) # Header files are also added to output
        ELSEIF( ${_file} MATCHES "^.*\\.ui$" )
            LIST( APPEND _ui ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.qrc$" )
            LIST( APPEND _qrc ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.rc\\.in$" OR ${_file} MATCHES "^.*\\.rc\\.cmake$" )
            LIST( APPEND _rc ${_file} )
        ELSEIF( ${_file} MATCHES "^.*\\.def\\.in$" OR ${_file} MATCHES "^.*\\.def\\.cmake$" )
            LIST( APPEND _def ${_file} )
        ELSE()
            LIST( APPEND ${_output} ${_file} )
        ENDIF()
    ENDFOREACH( _file )
	IF( USE_QT5 )
		QT5_WRAP_CPP( ${_output} ${_moc} )
		QT5_WRAP_UI( ${_output} ${_ui} )
		QT5_ADD_RESOURCES( ${_output} ${_qrc} )
	ELSE()
		QT4_WRAP_CPP( ${_output} ${_moc} )
		QT4_WRAP_UI( ${_output} ${_ui} )
		QT4_ADD_RESOURCES( ${_output} ${_qrc} )
	ENDIF()
    
    BUILDSYS_ADD_LIBRARY_RESOURCES( ${_output} ${_rc} )
    BUILDSYS_ADD_LIBRARY_RESOURCES( ${_output} ${_def} )
ENDMACRO( BUILDSYS_AUTO_LIBRARY_SOURCES )

# Enable/disables exception handling
MACRO( BUILDSYS_ENABLE_EXCEPTIONS _enable )
    IF( CMAKE_COMPILER_IS_GNUCXX )
        IF( ${_enable} )
            #REMOVE_DEFINITIONS( -fno-exceptions )
            STRING(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( -fexceptions )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )
        ELSE()
            #REMOVE_DEFINITIONS( -fexceptions )
            STRING(REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( -fno-exceptions )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
        ENDIF()
    ELSEIF( MSVC )
        IF( ${_enable} )
            #REMOVE_DEFINITIONS( -EHsc )
            STRING(REPLACE "-EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( /EHsc )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )
        ELSE()
            #REMOVE_DEFINITIONS( /EHsc )
            #REMOVE_DEFINITIONS( -EHsc )
            STRING(REPLACE "-EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
        ENDIF()
    ELSE()
        MESSAGE( WARNING "BUILDSYS_ENABLE_EXCEPTIONS is not supported on this compiler" )
    ENDIF()
ENDMACRO( BUILDSYS_ENABLE_EXCEPTIONS )

# Enable/disables RTTI
MACRO( BUILDSYS_ENABLE_RTTI _enable )
    IF( CMAKE_COMPILER_IS_GNUCXX )
        IF( ${_enable} )
            #REMOVE_DEFINITIONS( -fno-rtti )
            STRING(REPLACE "-fno-rtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( -frtti )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti" )
        ELSE()
            #REMOVE_DEFINITIONS( -frtti )
            STRING(REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( -fno-rtti )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti" )
        ENDIF()
    ELSEIF( MSVC )
        IF( ${_enable} )
            #REMOVE_DEFINITIONS( /GR- )
            #REMOVE_DEFINITIONS( -GR- )
            #REMOVE_DEFINITIONS( -GR )
            STRING(REPLACE "/GR-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "-GR-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "-GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( /GR )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR" )
        ELSE()
            #REMOVE_DEFINITIONS( /GR )
            #REMOVE_DEFINITIONS( -GR )
            #REMOVE_DEFINITIONS( -GR- )
            STRING(REPLACE "/GR-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "-GR-" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            STRING(REPLACE "-GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
            #ADD_DEFINITIONS( /GR- )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-" )
        ENDIF()
    ELSE()
        MESSAGE( WARNING "BUILDSYS_ENABLE_RTTI is not supported on this compiler" )
    ENDIF()
ENDMACRO( BUILDSYS_ENABLE_RTTI )

# Determine the target output directory based on platform and generator.
MACRO(SET_APPLICATION_TARGET_OUT_DIR)
  IF(GEN_NINJA OR GEN_MAKEFILES)
    # By default Ninja and Make builds don't create a subdirectory named after
    # the configuration.
    SET(APPLICATION_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

    # Output binaries (executables, libraries) to the correct directory.
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APPLICATION_TARGET_OUT_DIR})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${APPLICATION_TARGET_OUT_DIR})
  ELSE()
    SET(APPLICATION_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")
  ENDIF()
ENDMACRO()

# Copy a list of files from one directory to another. Relative files paths are maintained.
# The path component of the source |file_list| will be removed.
MACRO(COPY_FILES target file_list source_dir target_dir)
  FOREACH(FILENAME ${file_list})
    SET(source_file ${source_dir}/${FILENAME})
    GET_FILENAME_COMPONENT(target_name ${FILENAME} NAME)
    SET(target_file ${target_dir}/${target_name})

    STRING(FIND ${source_file} "$<CONFIGURATION>" _pos)
    IF(NOT ${_pos} EQUAL -1)
      # Must test with an actual configuration directory.
      STRING(REPLACE "$<CONFIGURATION>" "Release" existing_source_file ${source_file})
      IF(NOT EXISTS ${existing_source_file})
        STRING(REPLACE "$<CONFIGURATION>" "Debug" existing_source_file ${source_file})
      ENDIF()
    ELSE()
      SET(existing_source_file ${source_file})
    ENDIF()

    IF(IS_DIRECTORY ${existing_source_file})
      ADD_CUSTOM_COMMAND(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_file}" "${target_file}"
        VERBATIM
        )
    ELSE()
      ADD_CUSTOM_COMMAND(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_file}" "${target_file}"
        VERBATIM
        )
    ENDIF()
  ENDFOREACH()
ENDMACRO()

# Copy a list of files from one directory to another. Relative files paths are maintained.
# The path component of the source |file_list| will be removed.
MACRO(INSTALL_FILE file_list source_dir target_dir)
  FOREACH(FILENAME ${file_list})
    SET(source_file ${source_dir}/${FILENAME})
    # GET_FILENAME_COMPONENT(target_name ${FILENAME} NAME)
    # SET(target_file ${target_dir}/${target_name})

    STRING(FIND ${source_file} "$<CONFIGURATION>" _pos)
    IF(NOT ${_pos} EQUAL -1)
      # Must test with an actual configuration directory.
      STRING(REPLACE "$<CONFIGURATION>" "Release" existing_source_file ${source_file})
      IF(NOT EXISTS ${existing_source_file})
        STRING(REPLACE "$<CONFIGURATION>" "Debug" existing_source_file ${source_file})
      ENDIF()
    ELSE()
      SET(existing_source_file ${source_file})
    ENDIF()

    IF(IS_DIRECTORY ${existing_source_file})
      INSTALL( FILES ${source_file} DESTINATION "${target_dir}" )
    ELSE()
      INSTALL( FILES ${source_file} DESTINATION "${target_dir}" )
    ENDIF()
  ENDFOREACH()
ENDMACRO()