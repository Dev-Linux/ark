project(ark)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION 5.6.0)
set(KF5_MIN_VERSION 5.31.0)

# KDE Application Version, managed by release script
set (KDE_APPLICATIONS_VERSION_MAJOR "17")
set (KDE_APPLICATIONS_VERSION_MINOR "07")
set (KDE_APPLICATIONS_VERSION_MICRO "70")
set (KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(GenerateExportHeader)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING)

include(ECMSetupVersion)
ecm_setup_version(${KDE_APPLICATIONS_VERSION}
                  VARIABLE_PREFIX ARK
                  VERSION_HEADER "ark_version.h")

ecm_setup_version(${ARK_VERSION}
                  VARIABLE_PREFIX KERFUFFLE
                  SOVERSION ${ARK_VERSION_MAJOR})

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
    Concurrent
    Core
    Gui
    Widgets)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Archive
                                                        Config
                                                        Crash
                                                        DBusAddons
                                                        DocTools
                                                        I18n
                                                        IconThemes
                                                        ItemModels
                                                        KIO
                                                        Service
                                                        Parts
                                                        Pty
                                                        WidgetsAddons)

find_package(Qt5Test ${QT_MIN_VERSION} CONFIG QUIET)
set_package_properties(Qt5Test PROPERTIES
    PURPOSE "Required for tests"
    TYPE OPTIONAL)

if(NOT Qt5Test_FOUND)
    set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree.")
endif()

find_package(LibArchive 3.2.0 REQUIRED)
set_package_properties(LibArchive PROPERTIES
                       URL "http://www.libarchive.org/"
                       DESCRIPTION "A library for dealing with a wide variety of archive file formats"
                       PURPOSE "Required for among others tar, tar.gz, tar.bz2 formats in Ark.")

find_package(LibZip 1.2.0)
set_package_properties(LibZip PROPERTIES
                       URL "https://nih.at/libzip/"
                       DESCRIPTION "A library for handling zip archives"
                       PURPOSE "Optional for zip archives.")

find_package(SharedMimeInfo QUIET)
set_package_properties(SharedMimeInfo PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Required for archive formats without an official mimetype.")


option(WITH_TEST_COVERAGE "Build with test coverage support" OFF)
if (WITH_TEST_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif (WITH_TEST_COVERAGE)

set(SUPPORTED_ARK_MIMETYPES "")

add_definitions(-DTRANSLATION_DOMAIN="ark")

add_subdirectory(plugins)
add_subdirectory(kerfuffle)
add_subdirectory(part)
add_subdirectory(app)
add_subdirectory(doc)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

ki18n_install(po)
kdoctools_install(po)

install( FILES ark.categories DESTINATION ${KDE_INSTALL_CONFDIR} )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
