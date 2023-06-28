# IMDB Berkeley DB
This project implements an IMDB search website using Berkeley DB.
- this repo:  
 https://github.com/kushnertodd/imdb-berkeley-db

# Table of Contents
- [Usage](#usage)
  - [Web interface](#web-interface-usage)
- [Architecture](#architecture)
  - [Linux](#linux-architecture)
  - [Berekley DB](#berekley-db-architecture)
  - [Json-c](#json-c-architecture)
  - [OATPP++ application server](#oatpp++-application-server-architecture)
  - [IMDB database](#imdb-database-architecture)

## Usage <a name="usage"/>
### Web interface <a name="web-interface-usage"/>
## Architecture <a name="architecture"/>
It will consist of:
- a back-end feeding data from a database and web APIs
- a web server to serve it up on the web
Other featuers are:
- For now, the project will cross-compile on Windows 10 and WSL2 Linux, running under Windows.
- Berkeley DB, a key-indexed database, much faster than a relational DB if we don't need full SQL functionality 
It interfaces with this [Organizer page](Organizer.md)
### Linux <a name="linux-architecture">

Useful resources for learning and using Linux are:
- [Linux.org](https://www.linux.org/)
- [The Linux Documentation Prject](https://tldp.org)
- [The Linux command line for beginners](https://ubuntu.com/tutorials/command-line-for-beginners#1-overview)
- [Learn Linux, 101: A roadmap for LPIC-1](https://developer.ibm.com/tutorials/l-lpic1-map/?mhsrc=ibmsearch_a&mhq=lpic-1%20exam)
- [The Linux Command Line](http://linuxclass.heinz.cmu.edu/doc/tlcl.pdf)
- [TutorialsPoint UNIX / LINUX Tutorial](https://www.tutorialspoint.com/unix/index.htm)
- [Javatpoint Linux/Unix Tutorial](https://www.javatpoint.com/linux-tutorial)
- [Vim Tips Wiki](https://vim.fandom.com/wiki/Vim_Tips_Wiki)

### Berekley DB <a name="berekley-db-architecture"/>
- Resources
Various websites have useful information and tutorials for using Berkeley DB:
- http://web.mit.edu/ghudson/dev/third/rpm/db/docs/reftoc.html
- https://github.com/SerenadeEXE/BerkeleyDBTutorial
The latter can be downloaded with:
```
$ git clone github.com/SerenadeEXE/BerkeleyDBTutorial
```
The `examples` directory has some C examples and instructions in `examples/README`.

### Json-c <a name="json-c-architecture"/>
These are useful tutorials and resources for using json-c:  
- [A simple and complete json parser](https://linuxprograms.wordpress.com/category/json-c])  
- [json-c / libjson Tutorial with Examples](https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/)
- [Json-c Tutorial](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/index.md)  
- [JSON-C Tutorial: How to Parse JSON in C](https://progur.com/2018/12/how-to-parse-jffson-in-c.html)
- [json-c Tutorial introduction & outline](https://alan-mushi.github.io/2014/10/28/json-c-tutorial-intro-outline.html)
- [json-c Documentation](https://json-c.github.io/json-c/json-c-0.16/doc/html/index.html)
- API Documentation
json-c API documentation can be found at:
- [json-c Documentation](https://json-c.github.io/json-c/json-c-0.16/doc/html/index.html)

### OATPP++ application server <a name="oatpp++-application-server-architecture"/>
### IMDB database <a name="imdb-database-architecture"/>

## Releases
### v0.1
#### database
- names
  - name.db
  - title.db
  - principals.db
  - principals_name_id.sdb
  - principals_title_id.sdb
#### web
- movies.html, movies.js work with imdb_app
- bdb_lookup works
- bdb_bench measures query, insert
#### files
- bdb_env initializes environment
- bdb_init initializes context
- imdb_app uses bdb_env, bdb_init
#### arguments
             -b <cache bytes>
             -d <request db config json file>
             -e <environment home directory>
             -f <request json file>
             -g <cache gbytes>
             -h <database home directory>
             -n <number of repetitions> (default: 1)

