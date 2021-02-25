# search_engine

## System requirements

* Linux operating system.
* c++ version 11.
* g++ version 8+.


## Configure external libraries

1. Download [Chilkat](https://www.chilkatsoft.com/chilkatLinux.asp) for your operating system.

2. Configure Chilkat library:
    * Extract library from `.tar.gz` file.
    * Copy relative path from extracted folder.
    * Replace `CHILKATPATH` in `Makefile.inc` file with the relative path.
    

3. Download [Gumbo](https://github.com/google/gumbo-parser) v0.10.1.

4. Configure Gumbo:
    * Extract library from `.tar.gz` or `.zip` file.
    * Move to inside library folder.
    * Execute:
    ```
    $ sudo apt-get install libtool
    $ ./autogen.sh
    $ ./configure
    $ make
    $ sudo make install
    ```


## Text Chilkat library installation

## Crawl web pages


## Index and search crawled pages

## Format project files
