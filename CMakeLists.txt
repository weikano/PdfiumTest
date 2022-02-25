cmake_minimum_required(VERSION 3.14)

project(PdfiumTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(ENV{PDFium_DIR} /home/weikano/下载/pdfium-linux-x64/)
message("variable is $ENV{PDFium_DIR}")

find_package(Threads REQUIRED)

#find_package(PDFium)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
include_directories(${PDFium_DIR}/include)
add_executable(PdfiumTest
  main.cpp
  pdfdocument.cpp
  fpdf_stringhelper.cpp
  PageObjMarkValue.h
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
