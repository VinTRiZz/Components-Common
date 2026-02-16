
if (${COMPONENTS_IS_ENABLED_QT})
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
