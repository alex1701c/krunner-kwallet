cmake_minimum_required(VERSION 2.8.12)

project(KWalletRunner)

set(KF5_VERSION "5.44.0")
find_package(ECM ${KF5_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

# Find the required Libaries
find_package(Qt5 5.7.0 CONFIG REQUIRED Widgets)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMSetupVersion)

find_package(KF5 ${KF5_VERSION} REQUIRED
    I18n
    CoreAddons
    Notifications
    Service
    Runner
    Wallet
)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
)

add_subdirectory(src)

# CPack configuration
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Krunner Plugin which allows you to copy and edit kwallet entries")
set(CPACK_PACKAGE_NAME krunner-kwallet)
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "alex1701c")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
SET(CPACK_GENERATOR "DEB;RPM")
# KUbuntu 18.04 dependencies
set(CPACK_DEBIAN_PACKAGE_DEPENDS "kwalletmanager, plasma-workspace (>= 5.12)")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_REQUIRES "kwalletmanager plasma-workspace >= 5.12")
include(CPack)