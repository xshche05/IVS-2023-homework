if(EXISTS "/home/xshche05/ivs/cmake-build-debug-wsl/white_box_test[1]_tests.cmake")
  include("/home/xshche05/ivs/cmake-build-debug-wsl/white_box_test[1]_tests.cmake")
else()
  add_test(white_box_test_NOT_BUILT white_box_test_NOT_BUILT)
endif()
