consbri
=======

console brightness utility.


INSTALLING
----------

* Globally:

```
./configure && make && sudo make install
```

* Locally:

```
./configure --prefix=${HOME} && make && make install
```


USAGE
-----

Writing to `/sys` generally requires root privileges, so you will probably
need to run it with `sudo` privileges. The program takes a single argument,
the LCD brightness as a percentage (e.g. 25 for 25%).

```
echo "[+] LCD brightness to 25%"
consbri 25

echo "[+] turn off LCD"
consbri 0

echo "[+] full brightness"
consbri 100

echo "[+] half brightness"
consbri 50
```


AUTHOR
------

`consbri` was written by `Kyle Isom <kyle@tyrfingr.is>`.


LICENSE
-------

`consbri` is released under the ISC license. See the preamble in the
source file or the `LICENSE` file.
