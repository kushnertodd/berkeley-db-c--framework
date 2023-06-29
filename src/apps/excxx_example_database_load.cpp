/*-
 * Copyright (c) 2005, 2020 Oracle and/or its affiliates.  All rights reserved.
 *
 * See the file EXAMPLES-LICENSE for license information.
 *
 * $Id$
 */

// File: excxx_example_database_load.cpp
#include <iostream>

#include "bdb_dao.hpp"
#include "bdb_databases.hpp"
#include "bdb_databases_config.hpp"
#include "example_keys.hpp"
#include "inventory.hpp"
#include "vendor.hpp"

#ifdef _WIN32
extern "C" {
  extern int getopt(int, char * const *, const char *);
  extern char *optarg;
}
#else
#include <unistd.h>
#endif

// Forward declarations
void load_inventory_db(Bdb_dbp &inventory_db, const std::string &inventoryFile, Bdb_errors &errors);
void load_vendor_db(Bdb_dbp &vendor_db, const std::string &vendorFile, Bdb_errors &errors);

int usage(const std::string &errors) {
  std::cout << errors << std::endl;
  std::cout << "example_database_load [-b <path to data files>]"
            << " [-h <database home directory>]" << std::endl;
  return (-1);
}

std::string char_to_string(char c) {
  std::string s(1, c);
  return s;
}

// Loads the contents of vendors.txt and inventory.txt into
// Berkeley DB databases.
int main(int argc, char *argv[]) {
  Bdb_databases_config bdb_databases_config;
  Bdb_errors errors;

  // Initialize the path to the database files
  std::string base_name{"."};
  std::string db_home{"."};

  // Parse the command line arguments
  int ch;
  while ((ch = getopt(argc, argv, ":b:d:h:")) != EOF)
    switch (ch) {
      case 'b':
        base_name = optarg;
        break;
      case 'h':
        db_home = optarg;
        break;
      case ':':
        errors.add("excxx_example_database_load",
                   "1",
                   "Option -" + char_to_string((char) ch) + " requires an argument");
        break;
      case '?':
      default:
        errors.add("excxx_example_database_load", "2", "Invalid option '-" + char_to_string((char) ch));
        break;
    }

  // Identify the full name for our input files, which should
  // also include some path information.
  std::string inventoryFile = base_name + "/inventory.txt";
  std::string vendorFile = base_name + "/vendors.txt";
  std::string db_config_filename = base_name + "/example-databases.json";

  if (!errors.has()) {
    json_object *bdb_json = json_object_from_file(db_config_filename.c_str());
    if (bdb_json != nullptr) {
      bdb_databases_config.from_json(bdb_json, errors);
    } else
      errors.add("excxx_example_database_load",
                 "3",
                 "invalid json file " + db_config_filename + "(error: " + db_strerror(errno) + ")");
  }
  if (errors.has())
    usage(errors.to_string());

  try {
    Primary_database_config inventory_primary_database_config;
    bdb_databases_config.select("inventory", inventory_primary_database_config, errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> example_key_extractor = std::make_unique<Example_key_extractor>();
      Primary_database inventory_db(inventory_primary_database_config, example_key_extractor.get(), db_home, errors);
      if (!errors.has()) {
        Primary_database_config vendor_primary_database_config;
        bdb_databases_config.select("vendor", vendor_primary_database_config, errors);
        Primary_database vendor_db(vendor_primary_database_config, example_key_extractor.get(), db_home, errors);
        if (!errors.has())
          // Load the inventory database
          load_inventory_db(inventory_db.bdb_db, inventoryFile, errors);
        if (!errors.has())
          // Load the vendor database
          load_vendor_db(vendor_db.bdb_db, vendorFile, errors);
      }
    }
  } catch (DbException &e) {
    std::cerr << "Error loading databases. " << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << errors.to_string() << std::endl;
    return (e.get_errno());
  } catch (std::exception &e) {
    std::cerr << "Error loading databases. " << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << errors.to_string() << std::endl;
    return (-1);
  }

  if (errors.has())
    std::cerr << errors.to_string();

  // Bdb_db class constructors will close the databases when they
  // go out of scope.
  return (0);
} // End main

// Loads the contents of the inventory.txt file into a database
void load_inventory_db(Bdb_dbp &inventory_db, const std::string &inventoryFile, Bdb_errors &errors) {
  Bdb_DAO::load<Inventory_DTO_key, Inventory_DTO>(inventory_db, inventoryFile, errors, '#');
}

// Loads the contents of the vendors.txt file into a database
void load_vendor_db(Bdb_dbp &vendor_db, const std::string &vendorFile, Bdb_errors &errors) {
  Bdb_DAO::load<Vendor_DTO_key, Vendor_DTO>(vendor_db, vendorFile, errors, '#');
}
