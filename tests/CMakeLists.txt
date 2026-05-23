cmake_minimum_required(VERSION 3.20.0)

# Always built targets (no dependency on BUILD_TTTCORE)
add_executable(test_my_player test_my_player.cpp)
target_link_libraries(test_my_player tttplayer)
add_test(NAME smoke_test_player COMMAND ./test_my_player)

add_executable(test_stats test_stats.cpp)
target_link_libraries(test_stats tttplayer)
add_test(NAME test_player_stats COMMAND ./test_stats)

# My fast board tests
add_executable(test_fastboard test_fastboard.cpp)
target_link_libraries(test_fastboard tttplayer)
add_test(NAME test_fastboard COMMAND ./test_fastboard)

# My value score tests
add_executable(test_valuescore test_value_score.cpp)
target_link_libraries(test_valuescore tttplayer)
add_test(NAME test_valuescore COMMAND ./test_value_score)

# My negamax tests
add_executable(test_negamax test_negamax.cpp)
target_link_libraries(test_negamax tttplayer)
add_test(NAME test_negamax COMMAND ./test_negamax)


# Targets that require full or prebuilt tttcore
if((BUILD_TTTCORE STREQUAL "FULL") OR (BUILD_TTTCORE STREQUAL "PREBUILT"))
  # Baseline tests
  add_executable(test_with_baseline test_baseline.cpp)
  target_link_libraries(test_with_baseline tttplayer)
  add_test(NAME test_againts_baseline COMMAND ./test_with_baseline)

  # New stat test
  add_executable(test_stats_vs_baseline test_stats_vs_baseline.cpp)
  target_link_libraries(test_stats_vs_baseline tttplayer)
  add_test(NAME test_stats_vs_baseline COMMAND ./test_stats_vs_baseline)
  
  # Human player test
  add_executable(test_my_player_vs_human test_my_player_vs_human.cpp human_player.cpp)
  target_link_libraries(test_my_player_vs_human tttplayer)

endif()
