/** \file bare.cpp
 *
 * \brief A very basic translation unit with no external dependencies.
 */

/// MAIN() !
int main(int argc, const char *argv[])
{
  int arr[3] = {1, 2, 3};
  int j = 1;

  for (auto i : arr) {
    j += i << 1;
  }

  return j;
}
