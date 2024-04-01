cmake_minimum_required(VERSION 3.16)

project(KWalletRunner)

set(QT_MIN_VERSION 5.15.0)
set(KF_MIN_VERSION 5.105.0)
set(KDE_COMPILERSETTINGS_LEVEL 5.82.0)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(QtVersionOption)
include(ECMDeprecationSettings)
include(KDEClangFormat)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED Widgets)
find_package(KF${QT_MAJOR_VERSION} ${KF5_VERSION} REQUIRED
    I18n
    CoreAddons
    Notifications
    Runner
    Wallet
)

ecm_set_disabled_deprecation_versions(
	KF ${KF_MIN_VERSION}
	QT ${QT_MIN_VERSION}
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
