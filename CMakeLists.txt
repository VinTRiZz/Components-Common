if (COMPONENTS_IS_ENABLED_QT)
    COMPONENTS_CONFIGURE_COMPONENT(Common "1.0.0"
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )
else()
    COMPONENTS_CONFIGURE_COMPONENT(Common "1.0.0")
endif()

COMPONENTS_LINK_COMPONENT(Components-Common Logger)
COMPONENTS_LINK_COMPONENT(Components-Common ExtraClasses)
COMPONENTS_LINK_COMPONENT(Components-Common Filework)

target_precompile_headers(Components-Common PUBLIC

    # Most usable: AppSettings
    "src/appsettings/appsetting.hpp"

    # Most usable: Dirmanager
    "src/directorymanager.hpp"

    # Medium-used
    "src/utility.hpp"
)

COMPONENTS_ADD_COMPONENT_TEST(Common)
