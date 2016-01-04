project(KWalletRunner)

cmake_minimum_required(VERSION 2.8.12)

# KDE Application Version, managed by release script
set(KDE_APPLICATIONS_VERSION_MAJOR "15")
set(KDE_APPLICATIONS_VERSION_MINOR "07")
set(KDE_APPLICATIONS_VERSION_MICRO "80")
set(KWALLET_RUNNER_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)
cmake_policy(SET CMP0002 OLD)

# Find the required Libaries
find_package (Qt5 REQUIRED CONFIG COMPONENTS Widgets Core)
find_package (KF5 REQUIRED COMPONENTS I18n Service Runner Wallet)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   )

# We add our source code here
set(runner_SRCS src/kwalletrunner.cpp src/entrydialog.cpp)

ki18n_wrap_ui(runner_SRCS
    src/entrydialog.ui
)

add_library(krunner_kwallet MODULE ${runner_SRCS})
target_link_libraries(krunner_kwallet
                      Qt5::Widgets
                      KF5::I18n
                      KF5::Service
                      KF5::Runner
                      KF5::Wallet)

install(TARGETS krunner_kwallet
        DESTINATION ${PLUGIN_INSTALL_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plasma-runner-kwallet.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/plasma-runner-kwallet.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/plasma-runner-kwallet.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

