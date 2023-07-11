# berkeley-db-cpp-framework
C++ framework to improve Berkeley DB developer efficiency and reduce programming errors.

# Table of Contents
- [Background](#background)
    - [The purpose of Databases](#database-purpose)
    - [Advantages of Berekley DB](#berekley-db-advantages)
    - [Berekley DB resources](#berekley-db-resources)
- [Installing the necessary Berkeley DB components](#installing-the-components)
- [Using the Berkeley DB C++ Framework](#using-the-framework)
    - [Sample application](#sample-application)
## Background <a name="background"/>
[Oracle Berkeley DB](https://www.oracle.com/database/technologies/related/berkeleydb.html)  provides highly efficient 
storing, managaging, and accessing of large sets of data.  


The Berkeley DB C++ Framework (the Framework) are C++ classes and functions created to 
hide some of the complexity of using Berkeley DB.
Berkeley DB is simpler and requires less resources than common Relational Database 
Systems (RDBMS) such as that provided by Oracle, Microsoft SQL Server or MySQL.
As a result, it is much faster and easier to use.
It is an embedded database, called as a library from programs 
at the cost of not having an interactive user facility 
such as Structured Query Language (SQL) that makes
databases available to non-programmers.

### The purpose of Databases <a name="database-purpose"/>

A significant feature of databases is using *indexes* that greatly speed data access.
Assuming each data item is identified by a *key*, such a an account, order, part, or social 
security number, an index allows finding a given item in time almost
independent of the amount of data stored. Indexed databases have been used by the 
earliest data processing systems, and RDBMS incorporate indexes to be more 
efficient. Ironically, though, because of the extensive features they provide,
RDBMS are still substantially slower than the earlier simple indexed databases.

RDBMS allow  flexible data access to explore developing database applications,
and this makes them among the most popular databases.
However, when data applications are put into production, as data processing or web
data access applications, the patterns of data access are generally fixed
and the flexibility of a RDBMS is not required. Hence, a much simpler indexed
 database can be substituted to greatly enhance performance at the
cost reimplementing the application to use the new database.

Berkeley DB is an example of such a simpler indexed database systems.
Because it requires programming, changes take more time,
but for commercial applications the time is repaid by the cost saved
such as charges from a cloud service such as AWS. 
As a result, it is available for use by technical IT personnel rather than general users. 

### Berekley DB advantages<a name="berekley-db-advantages"/>

RDBMS provide other capabilities such as allowing users to concurrently access
a database, ensuring data is always valid (such as an account referenced
in an order always exists in the system), and that databases recover properly
when systems fail. 
Berkeley DB, however, also provides those capabilities as well as
much higher efficiency. 
While Berkeley DB is simpler, bacause it is programmed 
there is necessarily some complexity using it.
The Framework was created to simplify some of those
programming tasks to make developing and maintaining applications
using Berkeley DB easier.

### Berekley DB resources<a name="berekley-db-resources"/>

The rest of this description is intended for programmers.
Before continuing, a basic understanding of Berkeley DB and C++ is needed.
The following resources should be used to understand the 
how the Berkeley DB works and have an introduction to how
the Berkeley DB is programmed. 
While it is not essential to know all details of how
the original libraries work to use this framework,
it is necessary to know the primary C++ classes
to understand how the framework works 'under the hood'.

- [Berkeley DB Overview and software downloads](https://www.oracle.com/database/technologies/related/berkeleydb.html) (Oracle provides free access to Berkeley DB).
- [Getting Started with
  Berkeley DB
  for C++](https://www.oracle.com/technetwork/products/berkeleydb/berkeleydb-core-cxx-gsg-160938.pdf)
- [Berkeley DB Programmer's Reference Guide](https://docs.oracle.com/cd/E17276_01/html/programmer_reference/index.html)
  (as HTML, also as [PDF](https://docs.oracle.com/cd/E17076_05/html/programmer_reference/BDB_Prog_Reference.pdf)).
- [Berkeley DB book by Sleepycat Software](https://www.amazon.com/Berkeley-DB-SLEEPYCAT-Sleepycat-Software/dp/0735710643)
- [The Berkeley DB Book by Himanshu Yadava](https://www.amazon.com/Berkeley-DB-Book-Himanshu-Yadava/dp/1430211954)

## Installing the necessary Berkeley DB components<a name="installing-the-components"/>
The Framework requires a number of software packages are installed.
Instructions to install them are provide in [this document](src/INSTALL.md).
These include:  
- The Oracle Berkeley DB library
- The `json-c` JSON library
- The `cmake` tool for building C++ applications

The current system is designed to be built and run under Linux, 
such as Ubuntu or running under Windows Subsystem for Linux (WSL2)
on Windows. 
After release the ability to build and run under native Windows
using Visual Studio will be added.
`Git` is recommended for version control of your developing application.

## Using the Berkeley DB C++ Framework<a name="using the framework"/>

### Sample application<a name="sample-application"/>
A sample simple database application using the Framework is included.
It is based on the example application included in the basic
Berkeley DB software distribution described in
[Getting Started with
Berkeley DB
for C++](https://www.oracle.com/technetwork/products/berkeleydb/berkeleydb-core-cxx-gsg-160938.pdf).
Read that reference before reading this description of the application implemented in
the Framework, it will provide an introduction to the basic library
and allow a comparison of the ease of implementing the application
with the basic library and the framework.

## Berkeley DB Framework components
The Framework is a set of C++ template classes and functions that simplify Berkeley DB programming and remove a lot of 
boilerplate code making programs more compact and error free. The principal template classes and functions used 
are these.

### Data Transfer Object (DTO) classes
The programmer determines the data stored in the database, so they program a set of DTO classes that
provide objects containing the data in a C++ format. 
To enable the Framework they encode functions to serialize and deserialize the data into and from,Berkeley DB records. Berkeley DB enforces no format for the data so the serialization and 
deserialization functions written by the programmer converts their data into binary
data stored in database records. 
Defining these classes enables the function of the rest of the template classes and functions.
If the DTO classes are C++ Plain Old Data (POD) objects, serialization and deserialization
may be as simple as copying the in-memory data structures to and from Berkeley DB records.
For serializing and deserializing 
DTO classes containing variable length character or numeric data,
the Framework provides functions to simplify serialization and deserialization.
Examples of coded DTO classes are provided.

### Data Access Object (DAO) template functions
Once DTO classes are defined, DAO template functions simplify Berkeley DB operations such as 
reading database records, saving data records, and selecting all records with unique keys
or duplicate records sharing a primary or secondary database key. 
The example Getting Started application included in the Berkeley DB distribution 
is recoded as an example of using the Framework.

### Wrappers for Berkeley DB classes
Specific template classes are provided to simplify use of individual Berkeley DB C++ 
library classes. 
They provide simpler and more regular access to class functions, 
use DTOs as template types to make
the Framework classes more simply available to different data collections, and use 
[RAII](https://stroustrup.com/bs_faq2.html#finally) to safely manage
resources such as database handles, cursors, and allocated free store memory.

#### Dbt record management class wrapper
The Bdb_dbt class simplifies using the Berkeley DB C and C++ `Dbt` data type.
Using DTO serialization and deserialization, it simplifies encoding and
decoding data contained in Dbt objects and manages free store memory 
to prevent memory leaks

#### Db database handle wropper
The Dbd_db class manages database handles for primary and secondary databases
provided by the Berkeley DB C++ `Db` data type.
A JSON file describing the primary and secondary database relationships
provided by the programmer
automatically generates database handles and associates primary
and secondary databases. The class manages database handles
to ensure properly acquiring and releasing database handles.

#### Dbc cursor handle wrapper
The Dbd_cursor class simplifies cursor use to select all primary and
secondary keyed database record, and all duplicate primary and
secondary database records with a given key.
The class also manages cursors to properly ensure
acquiring and releasing cursors.

### Additional convenience classes
The Framwork provides various additional classes to simplify other
functions encountered increating Framwork programs.

#### File system IO classes
The Bdb_file_io class simplifies access to operating system files by
providing functions to obtain and properly release file handles,
and to read and write file data through C++ streams.

#### JSON utility class
The Framework assists in serialization and deserialization data in
JSON format. The Bdb_json_utils class wraps the `json-c` open
source library routines
and provides simpler access to the library.

#### Utility classes
Utility classes such as Bdb_errors and Bdb_tokens are provided to 
simplify error tracking and string token processing.

### The Framework code
The Framework was developed for Linux using the `cmake` build utility.
An early version worked with Visual Studio in Windows
and presumably could be modified to support the 
Berkeley DB Windows library version readily.

The Framework is distributed from a Github repository
currently under the MIT license.
The distribution can be cloned with Git from
https://github.com/kushnertodd/berkeley-db-cpp-framework.
Installation instructions, requiring the Berkeley DB
and `json-c` distributions, are included in INSTALL.db
markdown file. Build instructions are in 
BUILD.db.

Jetbrains CLion was used as an environment to develop
the Framework and is a nice option for development.
Building the Framework with the cmake `docs` target
generated Doxygen documentation for the low lever classes
in HTML format.
Enlisted additional developers as collaborators is encouraged.

## Summary
The Framework classes work together to give the programmer an intuitive
interface to the Berkeley C++ library. 
They work with the classes in the Berkeley
DB distribution and those may
be incorporated with the Framework classes as needed, though needing
their usage should be eliminated or reduced.
Various Berekey DB functions such as managing locks, 
threads, and certain specific operations such as deleting
records are omitted because they were not needed for the initial application
for which the Framework was developed, but classes to support them can 
added in a manner consistent with the rest of the Framework.

  
