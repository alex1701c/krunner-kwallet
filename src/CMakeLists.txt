set(runner_SRCS kwalletrunner.cpp entrydialog/entrydialog.cpp editdialog/editdialog.cpp)
ki18n_wrap_ui(runner_SRCS entrydialog/entrydialog.ui)
ki18n_wrap_ui(runner_SRCS editdialog/editdialog.ui)

kcoreaddons_add_plugin(krunner_kwallet SOURCES ${runner_SRCS} INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/krunner")
target_link_libraries(krunner_kwallet
    Qt::Widgets
    KF${QT_MAJOR_VERSION}::CoreAddons
    KF${QT_MAJOR_VERSION}::Notifications
    KF${QT_MAJOR_VERSION}::Runner
    KF${QT_MAJOR_VERSION}::Wallet
)
