set(runner_SRCS 
    kwalletrunner.cpp
        entrydialog/entrydialog.cpp
)

ki18n_wrap_ui(runner_SRCS
        entrydialog/entrydialog.ui
)

add_library(krunner_kwallet MODULE ${runner_SRCS})
target_link_libraries(krunner_kwallet
                      Qt5::Widgets
                      KF5::CoreAddons
                      KF5::I18n
                      KF5::Notifications
                      KF5::Service
                      KF5::Runner
                      KF5::Wallet)

install(TARGETS krunner_kwallet DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-kwallet.desktop DESTINATION ${SERVICES_INSTALL_DIR})
