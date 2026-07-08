cmake_minimum_required(VERSION 3.10)
project(GuitarPedal)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

include_directories(
	inc
	Drivers/CMSIS/Include
	Drivers/CMSIS/Device/ST/STM32F4xx/Include
	Drivers/stm32f4xx-hal-driver/Inc
)

# ----- Firmware target (ARM, only built when toolchain file is used) -----
if(CMAKE_CROSSCOMPILING)
	add_definitions(-DSTM32F411xE)
	add_compile_options(-Wno-psabi)
	enable_language(ASM)

    set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F411CEUX_FLASH.ld)

    add_compile_options(
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
    )



    set(HAL_SOURCES
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal.c
		Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_cortex.c
		Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_rcc.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_rcc_ex.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_gpio.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_dma.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_dma_ex.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_i2s.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_i2s_ex.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_flash.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_flash_ex.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_flash_ramfunc.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_pwr.c
        Drivers/stm32f4xx-hal-driver/Src/stm32f4xx_hal_pwr_ex.c
    )

    add_executable(GuitarPedal
        src/AudioFrame.cpp
        src/Effects.cpp
        src/mainPedal.cpp
		src/stm32f4xx_hal_msp.c
        Drivers/Startup/startup_stm32f411ceux.s
		Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
        ${HAL_SOURCES}
    )

	add_executable(BlinkTest
		test/stm32Blink.cpp
		${HAL_SOURCES}
        Drivers/Startup/startup_stm32f411ceux.s
		Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
	)

	target_link_options(BlinkTest PRIVATE
		-mcpu=cortex-m4
		-mthumb
		-mfpu=fpv4-sp-d16
		-mfloat-abi=hard
		-T${LINKER_SCRIPT}
		-specs=nosys.specs
		-specs=nano.specs
		-Wl,--gc-sections
		-Wl,-Map=${CMAKE_BINARY_DIR}/BlinkTest.map
	)

	target_link_options(GuitarPedal PRIVATE
		-mcpu=cortex-m4
		-mthumb
		-mfpu=fpv4-sp-d16
		-mfloat-abi=hard
		-T${LINKER_SCRIPT}
		-specs=nosys.specs
		-specs=nano.specs
		-Wl,--gc-sections
		-Wl,-Map=${CMAKE_BINARY_DIR}/GuitarPedal.map
	)

	add_custom_target(flash_pedal
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:GuitarPedal> GuitarPedal.bin
		COMMAND st-flash --reset write GuitarPedal.bin 0x08000000
		DEPENDS GuitarPedal
		COMMENT "Flashing GuitarPedal...")

endif()

# ----- Tests (host compiler, always built — skipped during ARM cross-compile) -----
if(NOT CMAKE_CROSSCOMPILING)
    enable_testing()

    add_executable(test_bypass
        src/AudioFrame.cpp
        src/Effects.cpp
        test/BypassTest.cpp
    )
    add_executable(test_boost
        src/AudioFrame.cpp
        src/Effects.cpp
        test/BoostTest.cpp
    )
    target_compile_options(test_boost PRIVATE -g)

    add_test(NAME BypassTest COMMAND test_bypass)
    add_test(NAME BoostTest COMMAND test_boost)
endif()
