# CMake generated Testfile for 
# Source directory: /media/wudi/BAK/text_sum/LDA/cppjieba
# Build directory: /media/wudi/BAK/text_sum/LDA/cppjieba/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(./test/test.run "./test/test.run")
set_tests_properties(./test/test.run PROPERTIES  _BACKTRACE_TRIPLES "/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;26;ADD_TEST;/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;0;")
add_test(./load_test "./load_test")
set_tests_properties(./load_test PROPERTIES  _BACKTRACE_TRIPLES "/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;27;ADD_TEST;/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;0;")
add_test(./demo "./demo")
set_tests_properties(./demo PROPERTIES  _BACKTRACE_TRIPLES "/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;28;ADD_TEST;/media/wudi/BAK/text_sum/LDA/cppjieba/CMakeLists.txt;0;")
subdirs("deps")
subdirs("test")
