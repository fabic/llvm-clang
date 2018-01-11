# cxx-playground

[fabic.net](https://fabic.net)'s personal C++ playground.

### Build instructions

Quick build command :

    $ ( [ -d build/ ] && rm -rf build/ || true) &&
        mkcd build &&
        ( cmake .. && make &&
            find -type f -perm -a+x -name \*.so -ls ) ; cd -

### Pointers

* PostgreSql's [libpqxx](http://pqxx.org/development/libpqxx/) &ndash; [libpqxx.readthedocs.io](http://libpqxx.readthedocs.io/en/latest/)
