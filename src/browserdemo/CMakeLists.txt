# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
# 因为windows下链接时分为 console 和 windows 两个子系统，所以windows下有些问题需要特殊处理。

# 用 qmake 时：
# 	默认是 windows 子系统
# 	可以通过 CONFIG += console 使用 console 子系统
# 用 cmake 是：
# 	默认是 console 子系统
# 	使用 windows 子系统需要
# 		SET(QT_USE_QTMAIN TRUE)
# 		ADD_EXECUTABLE(example WIN32 main.cpp mainwindow.cpp ${example_MOCS})
# 前者启用 qtmain.lib 库来提供windows下的 WinMain 入口函数。后者链接 windows 子系统

CMAKE_POLICY(SET CMP0020 NEW)
CMAKE_POLICY(SET CMP0043 NEW)

IF( COMMAND CMAKE_POLICY )
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF( COMMAND CMAKE_POLICY )

IF( WIN32 )
    SET( QT_USE_QTMAIN TRUE )
ENDIF( WIN32 )

SET(APPLICATION_TARGET "BrowserDemo")
SET_APPLICATION_TARGET_OUT_DIR()

BUILDSYS_AUTO_SOURCES(
  app_SOURCES
  main.cpp
  mainwindow.h
  mainwindow.cpp
  mainwindow.ui
  appdefine.h
  application.qrc
  application.rc.in
  browserclient.cpp
  browserclient.h
  browserclient_win.cpp
  browserapp.h
  browserapp.cpp
  browserview.h
  browserview.cpp
  browserview.ui
  browserv8handler.h
  browserv8handler.cpp
  browsercallimpl.h
  browsercallimpl.cpp
  config/appconfig.h
  config/appconfig.cpp
  # ${APPLICATION_TARGET}.json
)

ADD_EXECUTABLE( ${APPLICATION_TARGET} WIN32 ${app_SOURCES} )

TARGET_INCLUDE_DIRECTORIES( ${APPLICATION_TARGET} PUBLIC ${CEF_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES( ${APPLICATION_TARGET} 
  debug ${CEF_LIBRARY_DEBUG} 
  optimized ${CEF_LIBRARY_RELEASE} 
)
TARGET_LINK_LIBRARIES( ${APPLICATION_TARGET} 
  debug ${CEF_LIBRARY_WRAPPER_DEBUG} 
  optimized ${CEF_LIBRARY_WRAPPER_RELEASE} 
)
TARGET_LINK_LIBRARIES( ${APPLICATION_TARGET} ${CEF_STANDARD_LIBS} )

TARGET_LINK_LIBRARIES( ${APPLICATION_TARGET} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::WinMain )
SET_TARGET_PROPERTIES( ${APPLICATION_TARGET} PROPERTIES OUTPUT_NAME ${BUILDSYS_TELEPHONE_OUTPUT_NAME} )

# Copy binary and resource files to the target output directory. 
SET(CEF_BINARY_FILES
  chrome_elf.dll
  d3dcompiler_47.dll
  libcef.dll
  libEGL.dll
  libGLESv2.dll
  snapshot_blob.bin
  v8_context_snapshot.bin
  swiftshader
  cef.pak
  cef_100_percent.pak
  cef_200_percent.pak
  cef_extensions.pak
  devtools_resources.pak
  icudtl.dat
  locales
)
COPY_FILES("${APPLICATION_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}/bin" "${APPLICATION_TARGET_OUT_DIR}") 
# Copy finished

SET( CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION . )
SET( BIN_INSTALL_DIR . )
SET( INSTALL_TARGETS_DEFAULT_ARGS RUNTIME DESTINATION "${BIN_INSTALL_DIR}" COMPONENT RUNTIME )

INSTALL( TARGETS ${APPLICATION_TARGET} ${INSTALL_TARGETS_DEFAULT_ARGS} )
INCLUDE( InstallRequiredSystemLibraries )
