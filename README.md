# search_engine

## System requirements

* Linux operating system.
* c++ version 11.
* g++ version 8+.
* POSIX threads.


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
    * Run the following commands on terminal:
    ```
    $ sudo apt-get install libtool
    $ ./autogen.sh
    $ ./configure
    $ make
    $ sudo make install
    ```


## Text Chilkat library installation

The `simple_crwaler` is a simple clawler that receives a parent URL and a number, and outputs the first parameter followed by the requested number of links inside it.

### Build

```
$ cd simples_crawler
$ make
```

### Run

```
$ ./a.out <parentUrl> <numAdditionalLinks>
```
* `<urlToCrawl>`: the parent url.
* `<numAdditionalLinks>`: the number of additional urls to crawl inside parent url.

## Crawl web pages

The `complex_crawler` module is a crawler with two options for crawling: a long term and a short term scheduler. If the crawler is configured with the first option, it will use a single thread and a priroty queue based on the URL size to choose witch URL it should visit first. Otherwise, it will use multiple threads to collect a large number of pages in parallel.

### Build

```
$ cd complex_crawler
$ make
```

### Run

```
$ ./demo/main.out <seedFileName> <storageDir> <mode> <numPages> <numThreads?>
```

* `<seedFileName>`: the file name for seed irls separed by a line break. Ex: `seed/seed.txt`.
* `<storageDir>`: the directory to put the collected HTML pages and a `index.txt` file, that has the collection information. Ex: `storage/` or `storage_extra/`.
* `<mode>`: 0 for long term scheduler and 2 for short term scheduler. The value 1 chooses an option under construction and should not be used.
* `<numPages>`: the number of total pages to crawl.
* `<numThreads?>`: the number of threads to use when choosing the short term scheduler (default value is 1).

## Index and search the crawled pages

The `inverted_index` module indexes the crawled collection and searches for specified pages inside it.

### Build

```
$ cd inverted_index
$ make
```

### Indexing

```
./demo/indexing.out <collectionDir> <outDir>
```

* `<collectionDir>`: the directory where collection is stored. Ex: `../complex_crawler/storage/`.
* `<outDir>`: the directory to store the encoded index and the vocabulary for it.

### Search
```
./demo/indexing.out <indexFileName> <term>
```

* `<indexFileName>`: the index file name. Ex: `storage/inverted_index.txt`.
* `<term>`: the vocabulary term to search for.

## Format project files

1. Install clang-format:

```
$ sudo apt-get install clang-format
```

2. Run:
```
$ clang-format -i <file>
```
