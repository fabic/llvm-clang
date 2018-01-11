
set(LLVM_ENABLE_EH ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_llvm_loadable_module(
  clong-09
    main.cpp
    ClongPluginASTAction.cpp
    Clong.cpp
    $<TARGET_OBJECTS:PimplioTerminal>
    $<TARGET_OBJECTS:TypePrinter>
    #$<TARGET_OBJECTS:DudeHackRoutines>
  PLUGIN_TOOL clang
  )

# target_compile_options(     clong-09 PUBLIC ${LIBxxxx_CFLAGS} )
# target_include_directories( clong-09 PUBLIC ${LIBxxxx_INCLUDE_DIRS} )

# target_link_libraries( clong-09 -lunwind -lunwind-x86_64 )
target_link_libraries( clong-09 DudeHackRoutines )
# FIXME.
# target_link_libraries( clong-09 LocalLibunwindStatic )
# target_link_libraries( clong-09 ${CMAKE_SOURCE_DIR}/local/lib/libunwind-x86_64.a)

