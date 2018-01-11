
asm
(
 ".section .init  \n"
 ".global _init   \n"
 "_init:          \n"
 "  pop %rax      \n"
 "  ret           \n"
 "  nop           \n"
);

asm
(
 ".section .fini  \n"
 ".global _fini   \n"
 "_fini:          \n"
 "  pop %rax      \n"
 "  ret           \n"
 "  nop           \n"
);

