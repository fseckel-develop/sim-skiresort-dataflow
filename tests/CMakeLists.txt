function(add_skiresort_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
            PRIVATE
            skiresort
            skiresort_warnings
            skiresort_sanitizers
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()


add_skiresort_test(test_bistro      bistro_tests.c     )
add_skiresort_test(test_bus_stop    bus_stop_tests.c   )
add_skiresort_test(test_bus         bus_tests.c        )
add_skiresort_test(test_car_park    car_park_tests.c   )
add_skiresort_test(test_car         car_tests.c        )
add_skiresort_test(test_clock       clock_tests.c      )
add_skiresort_test(test_gondola     gondola_tests.c    )
add_skiresort_test(test_hotel       hotel_tests.c      )
add_skiresort_test(test_lift_queue  lift_queue_tests.c )
add_skiresort_test(test_lift        lift_tests.c       )
add_skiresort_test(test_list        list_tests.c       )
add_skiresort_test(test_memory      memory_tests.c     )
add_skiresort_test(test_person      person_tests.c     )
add_skiresort_test(test_probability probability_tests.c)
add_skiresort_test(test_queue       queue_tests.c      )
add_skiresort_test(test_schedule    schedule_tests.c   )
add_skiresort_test(test_slope       slope_tests.c      )
add_skiresort_test(test_station     station_tests.c    )