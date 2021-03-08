
IF( NOT WIN32 )
    #MESSAGE( WARNING "Only support packaging on WIN32" )
    RETURN()
ENDIF( NOT WIN32 )

FIND_PATH( QT_BIN_DIR QtCore4.dll )

# --------------------------------------------------------------------
# Install
# --------------------------------------------------------------------
IF( NOT QT_IS_STATIC )
    ### FIXME Using QT_IS_STATIC to determine static system library is INCORRECT ###
    SET( CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION . )
    INCLUDE( InstallRequiredSystemLibraries )
    ### FIXME ###

	INSTALL(
		FILES "${QT_BIN_DIR}/QtCored4.dll" "${QT_BIN_DIR}/QtGuid4.dll" "${QT_BIN_DIR}/QtNetworkd4.dll" "${QT_BIN_DIR}/QtWebKit4d.dll" "${QT_BIN_DIR}/QtSqld4.dll" "${QT_BIN_DIR}/QtXmld4.dll" "${QT_BIN_DIR}/QtMultimediad4.dll" "${QT_BIN_DIR}/QtOpenGLd4.dll" "MediaCore.dll" "smtp.dll" "${QTMOBILITY_LIBRARY_DIR}/QtMultimediaKitd1.dll" "${QTMOBILITY_LIBRARY_DIR}/QtSystemInfod1.dll"
		DESTINATION "${BIN_INSTALL_DIR}"
		CONFIGURATIONS Debug
		COMPONENT RUNTIME
	)
	INSTALL(
		FILES "${QT_BIN_DIR}/QtCore4.dll" "${QT_BIN_DIR}/QtGui4.dll" "${QT_BIN_DIR}/QtNetwork4.dll" "${QT_BIN_DIR}/QtWebKit4.dll" "${QT_BIN_DIR}/QtSql4.dll" "${QT_BIN_DIR}/QtXml4.dll" "${QT_BIN_DIR}/QtMultimedia4.dll" "${QT_BIN_DIR}/QtOpenGL4.dll" "MediaCore.dll" "smtp.dll" "libeay32.dll" "ssleay32.dll" "${QTMOBILITY_LIBRARY_DIR}/QtMultimediaKit1.dll" "${QTMOBILITY_LIBRARY_DIR}/QtSystemInfo1.dll"
		DESTINATION "${BIN_INSTALL_DIR}"
		CONFIGURATIONS Release MinSizeRel RelWithDebInfo
		COMPONENT RUNTIME
	)
	INSTALL(
		FILES "${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll" "${QT_PLUGINS_DIR}/imageformats/qgif4.dll"
		DESTINATION "${BIN_INSTALL_DIR}/imageformats"
		CONFIGURATIONS Release MinSizeRel RelWithDebInfo
		COMPONENT RUNTIME
	)
	INSTALL(
		FILES "${QT_PLUGINS_DIR}/sqldrivers/qsqlite4.dll"
		DESTINATION "${BIN_INSTALL_DIR}/sqldrivers"
		CONFIGURATIONS Release MinSizeRel RelWithDebInfo
		COMPONENT RUNTIME
	)
	INSTALL(
		FILES "${QTMOBILITY_PLUGINS_DIR}/mediaservice/dsengine.dll" "${QTMOBILITY_PLUGINS_DIR}/mediaservice/qtmedia_audioengine.dll"
		DESTINATION "${BIN_INSTALL_DIR}/mediaservice"
		CONFIGURATIONS Release MinSizeRel RelWithDebInfo
		COMPONENT RUNTIME
	)
	
ENDIF( NOT QT_IS_STATIC )

# --------------------------------------------------------------------
# Package
# --------------------------------------------------------------------
SET( BUILDSYS_TELEPHONE_EXE_NAME YesPo )
SET( BUILDSYS_TRANSLATOR_EXE_NAME YesPoPartner )
SET( BUILDSYS_CAPABILITY_EXE_NAME VoiceEasyTest )

IF ( BUILDSYS_PACKAGE_PHONE )
	SET( BUILDSYS_TARGET_EXE_NAME ${BUILDSYS_TELEPHONE_EXE_NAME} )
ELSE ( BUILDSYS_PACKAGE_PHONE )
	SET( BUILDSYS_TARGET_EXE_NAME ${BUILDSYS_TRANSLATOR_EXE_NAME} )
ENDIF( BUILDSYS_PACKAGE_PHONE )

# MESSAGE ( ${tonego_SOURCE_DIR} )

#SET( CPACK_INSTALL_CMAKE_PROJECTS "${yespo_BINARY_DIR};yespo;RUNTIME;/" )
SET( CPACK_PACKAGE_NAME ${BUILDSYS_TARGET_EXE_NAME} )
SET( CPACK_PACKAGE_VENDOR ${BUILDSYS_VENDOR_NAME} )
SET( CPACK_PACKAGE_VERSION_MAJOR ${BUILDSYS_VERSION_MAJOR} )
SET( CPACK_PACKAGE_VERSION_MINOR ${BUILDSYS_VERSION_MINOR} )
# IF( BUILDSYS_DEMO OR BUILDSYS_LOCAL)
	SET( CPACK_PACKAGE_VERSION_PATCH ${BUILDSYS_VERSION_PATCH} )
#ENDIF( BUILDSYS_DEMO OR BUILDSYS_LOCAL)
SET( CPACK_PACKAGE_INSTALL_DIRECTORY ${BUILDSYS_TARGET_EXE_NAME} )
# SET( CPACK_RESOURCE_FILE_LICENSE "${yespo_SOURCE_DIR}/COPYING" )
# SET( CPACK_RESOURCE_FILE_README "${yespo_SOURCE_DIR}/README" )
SET( CPACK_STRIP_FILES TRUE )
SET( CPACK_MONOLITHIC_INSTALL TRUE )

SET( CPACK_NSIS_EXECUTABLES_DIRECTORY . )
SET( CPACK_PACKAGE_EXECUTABLES "${BUILDSYS_TARGET_EXE_NAME};${BUILDSYS_TARGET_EXE_NAME}" )
SET( CPACK_NSIS_INSTALLED_ICON_NAME "${BUILDSYS_TARGET_EXE_NAME}.exe" )
#SET( CPACK_NSIS_COMPRESSOR "/SOLID lzma" )
SET( CPACK_NSIS_COMPRESSOR lzma )

#Create desktop shortcut, both the following two command are needed to make effect
SET( CPACK_CREATE_DESKTOP_LINKS "${BUILDSYS_TARGET_EXE_NAME}" ) 
SET( CPACK_NSIS_MODIFY_PATH "ON" )
# SET( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )

#Icon
# SET ( CPACK_NSIS_MUI_ICON "${yespo_SOURCE_DIR}/package/installer.ico")
# SET ( CPACK_NSIS_MUI_UNIICON "${yespo_SOURCE_DIR}/package/uninstaller.ico")
# SET ( CPACK_PACKAGE_ICON "${yespo_SOURCE_DIR}/package/installer.bmp")

#Finish to run
SET ( CPACK_NSIS_MUI_FINISHPAGE_RUN "${BUILDSYS_TARGET_EXE_NAME}.exe" )

INCLUDE( CPack )
