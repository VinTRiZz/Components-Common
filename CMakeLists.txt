if (COMPONENTS_IS_ENABLED_QT)
    COMPONENTS_CONFIGURE_COMPONENT(Common
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )
else()
    COMPONENTS_CONFIGURE_COMPONENT(Common
        boost_system
        boost_program_options
    )
endif()

COMPONENTS_LINK_COMPONENT(Common Logger)

target_precompile_headers(Common PUBLIC

    # Most usable: AppSettings
    "src/applicationsettings.hpp"
    "src/qt/applicationsettings.hpp"
    "src/no_qt/applicationsettings.hpp"

    # Most usable: Dirmanager
    "src/directorymanager.hpp"
    "src/qt/directorymanager.hpp"
    "src/no_qt/directorymanager.hpp"

    # Medium-used
    "src/mixins.hpp"
    "src/utils.hpp"
    "src/commonfunctions.hpp"
    "src/accessmanager.hpp"
)