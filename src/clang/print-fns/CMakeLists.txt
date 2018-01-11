
add_llvm_loadable_module(PrintFunctionNames PrintFunctionNames.cpp PLUGIN_TOOL clang)

# FIXME: this is unused.
if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(PrintFunctionNames PRIVATE
    clangAST
    clangBasic
    clangFrontend
    LLVMSupport
    )
  message(WARNING "FIXME: Dude, can't tell if this target_link_libraries() directive is issued or not.")
endif()
