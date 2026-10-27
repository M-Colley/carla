include("${CMAKE_SOURCE_DIR}/Util/Tests/EpisodeConcurrency/CMakeLists.txt")
function(carla_add_pair_worker_checks)
  target_sources(carla-episode-tests PRIVATE
    "${CMAKE_SOURCE_DIR}/LibCarla/source/test/client/test_traffic_manager_worker.cpp"
    "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/test_worker_episode_forwarding.cpp")
  target_include_directories(carla-episode-tests PRIVATE
    "${CMAKE_SOURCE_DIR}/LibCarla/source/test/client")
endfunction()
cmake_language(DEFER CALL carla_add_pair_worker_checks)
