cmake_minimum_required(VERSION 3.21.1)

project(PdfiumTest LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PDFium_DIR /home/weikano/下载/pdfium-linux-x64/)
set(PDFium_DIR ${PROJECT_SOURCE_DIR}/pdfium-linux-x64/)
set(ENV{PDFium_DIR} ${PDFium_DIR})
message("variable is $ENV{PDFium_DIR}")
message("variable is ${PDFium_DIR}")

find_package(Threads REQUIRED)

#find_package(PDFium)

#find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
include_directories(${PDFium_DIR}/include)
add_executable(PdfiumTest
  main.cpp
  pdfdocument.cpp
  fpdf_stringhelper.cpp
  PageObjMarkValue.h  

  PDFUtil.cpp
  filewriterimpl.cpp
  annot_const.cpp
  ireadernote.cpp
  test_ireadnote.cpp
#  fs_rectf_helper.cpp

#  converter.cpp
)
target_link_libraries(PdfiumTest
#    Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Threads::Threads
    ${PDFium_DIR}/lib/libabsl.so
    ${PDFium_DIR}/lib/libc++.so
    ${PDFium_DIR}/lib/libchrome_zlib.so
    ${PDFium_DIR}/lib/libicui18n.so
    ${PDFium_DIR}/lib/libicuuc.so
    ${PDFium_DIR}/lib/libpdfium.so
    )
