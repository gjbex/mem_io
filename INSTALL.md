# Installation instructions for mem_io

## Prerequites for installation
* A C99 complient C compiler (e..g, GCC or Intel icc)
* The hiredis library (https://github.com/redis/hiredis),
    tested with version 0.13.3


## Obtaining mem_io
Either clone the Github repository, or download a specific release
(https://github.com/gjbex/mem_io)
To download a release, and unpack it:
```
$ VERSION='1.1'
$ wget  -O mem_io-${VERSION}.tar.gz \
    https://github.com/gjbex/mem_io/archive/${VERSION}.tar.gz
$ tar  xaf  mem_io-${VERSION}.tar.gz
```
Modify the version number to the release of interest.


## Building and installing
The usual mantra applies:
```
$ cd mem_io-${VERSION}
$ ./configure
$ make
$ make install
```
If you would prefer not to install in `/usr/local/`, set `configure`'s
prefix option, e.g.,
```
$ ./configure  --prefix=/opt/software/mem_io/${VERSION}
```
Check the `mem_io_global.conf` file in the `share/mem_io` subdirectory of
the installation directory, and modify according to your local setup.


## Runtime requirements
To run, mem_io expects some software to be available on your system:
* redis: data structure server (http://redis.io/)
* m4 macro processor (normally part of standard Linux distros)

Set the paths to `redis-server` and `m4` in the `mem_io_globalconf` file,
located in the `share/mem_io` subdirectory of your installation directory.
