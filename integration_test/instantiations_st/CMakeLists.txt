add_library(integration_test.instantiations_st)
emil_build_for(integration_test.instantiations_st TARGET_MCU stm32f767)

target_sources(integration_test.instantiations_st PRIVATE
    EchoOnSesame.cpp
    EchoOnSesame.hpp
)

target_link_libraries(integration_test.instantiations_st PUBLIC
    integration_test.logic
    hal_st.instantiations
)
