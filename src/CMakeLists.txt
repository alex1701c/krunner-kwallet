set(runner_SRCS kwalletrunner.cpp entrydialog/entrydialog.cpp editdialog/editdialog.cpp)
ki18n_wrap_ui(runner_SRCS entrydialog/entrydialog.ui)
ki18n_wrap_ui(runner_SRCS editdialog/editdialog.ui)

kcoreaddons_add_plugin(krunner_kwallet SOURCES ${runner_SRCS} INSTALL_NAMESPACE "kf5/krunner")
target_link_libraries(krunner_kwallet
    Qt5::Widgets
    KF5::CoreAddons
    KF5::Notifications
    KF5::Runner
    KF5::Wallet
)
