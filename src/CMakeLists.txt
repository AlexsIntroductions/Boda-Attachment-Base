#Servo Library
add_library(Servo
    servo.h
    servo.c
    )

target_link_libraries(Servo
    pico_stdlib 
    hardware_pwm
)
target_include_directories(Servo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

#Finger Library
add_library( Finger
    finger.h
    finger.c
    )

target_link_libraries(Finger
    Servo
    pico_stdlib
)
target_include_directories(Finger PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

#Attachment Library
add_library( Attachment
    Attachment.h
    Attachment.c
    )

target_link_libraries(Attachment
    pico_stdlib
    hardware_spi
)
target_include_directories(Attachment PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
