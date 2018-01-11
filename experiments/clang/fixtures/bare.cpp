/** \file bare.cpp
 *
 * \brief A very basic translation unit with no external dependencies.
 *
 */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 4 6 8 0 2 4 6 8 0 2 4 6 8 0 2 4 6 8 0 2 4 6 8 0 2 4 6 8 0 2 4 6 8 0 0 2 4 6 8
//3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9
//    10 ^      20 ^                40 ^            56 ^            72 ^    80 ^
//
// a b c d e f g h i j k l m n o p q r s t u v w x y z
//
// á é í ó ú
// à è ì ò ù
// â ê î ô û
//     ï ö ü   œ ø ¶    ˙¿¿¿ø˙ ß§ ´    ś   ú˙ ȧḃċḋėḟġ
//     ç                     ñ         ṡ   µ       ẏ

/// MAIN() !
///
/// Hey ^^
///
/// \param argc Command line arguments count.
/// \param argv Array of (pointers to) individual arguments.
///
/// \return 2 + 4 + 8
int main(int argc, const char *argv[])
{
  int arr[3] = {1, 2, 3};
  int j = 1;

  for (auto i : arr) {
    j += 1 << i;
  }

  return j;
}

/*
 *
 * That would be a multi-line comment block near the end-of-file.
 *
 */

// And this one is a line-comment just before blank line.