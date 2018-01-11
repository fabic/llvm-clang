
llvm_map_components_to_libnames(
    LLVM_CLANG_LIBS
        Support Option
        # X86CodeGen
        # X86AsmParser
        # X86Desc
        # X86Info
    )

add_executable(find-all-symbols
    FindAllSymbolsMain.cpp
    FindAllSymbols.cpp FindAllSymbolsAction.cpp FindAllMacros.cpp
    HeaderMapCollector.cpp PathConfig.cpp PragmaCommentHandler.cpp
    STLPostfixHeaderMap.cpp SymbolInfo.cpp
    )

target_link_libraries(find-all-symbols
    clangAST clangASTMatchers clangBasic clangFrontend clangLex clangTooling
    ${LLVM_CLANG_LIBS}
  )

