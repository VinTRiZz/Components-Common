if (COMPONENTS_IS_ENABLED_QT)
    COMPONENTS_CONFIGURE_COMPONENT(Common
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )
else()
    COMPONENTS_CONFIGURE_COMPONENT(Common)
endif()

COMPONENTS_LINK_COMPONENT(Common Logger)
COMPONENTS_LINK_COMPONENT(Common ExtraClasses)
COMPONENTS_LINK_COMPONENT(Common Filework)

target_precompile_headers(Common PUBLIC

    # Most usable: AppSettings
    "src/appsettings/appsetting.hpp"

    # Most usable: Dirmanager
    "src/directorymanager.hpp"

    # Medium-used
    "src/utility.hpp"
)

COMPONENTS_ADD_COMPONENT_TEST(Common)
