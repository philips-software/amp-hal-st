add_library(integration_test.instantiations_st ${HALST_EXCLUDE_FROM_ALL})

target_sources(integration_test.instantiations_st PRIVATE
    EchoOnSesame.cpp
    EchoOnSesame.hpp
)

target_link_libraries(integration_test.instantiations_st PUBLIC
    integration_test.logic
    hal_st.instantiations
)
