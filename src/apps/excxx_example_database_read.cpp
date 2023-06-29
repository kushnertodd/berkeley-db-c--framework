/*-
 * Copyright (c) 2005, 2020 Oracle and/or its affiliates.  All rights reserved.
 *
 * See the file EXAMPLES-LICENSE for license information.
 *
 * $Id$ 
 */

// File: excxx_example_database_read.cpp

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
int show_item(Bdb_dbp &item_name_sdb,
              Bdb_dbp &inventory_db,
              Bdb_dbp &vendor_db,
              const std::string &item_name,
              Bdb_errors &errors);
int show_all_records(Bdb_dbp &inventory_db, Bdb_dbp &vendor_db, Bdb_errors &errors);
int show_vendor(Bdb_dbp &vendor_db, const std::string &vendor, Bdb_errors &errors);

int usage(const std::string &errors) {
  std::cerr << errors << std::endl;
  std::cout << "example_database_read [-b <path to data files>]"
            << " [-h <database home directory>]"
            << " [-i <item name>]" << std::endl;
  return (-1);
}

std::string char_to_string(char c) {
  std::string s(1, c);
  return s;
}

int main(int argc, char *argv[]) {
  Bdb_databases_config bdb_databases_config;
  Bdb_errors errors;

  // Initialize the path to the database files
  std::string base_name{"."};
  std::string db_home{"."};
  std::string item_name;

  // Parse the command line arguments
  int ch;
  while ((ch = getopt(argc, argv, ":b:h:i:")) != EOF)
    switch (ch) {
      case 'b':
        base_name = optarg;
        break;
      case 'h':
        db_home = optarg;
        break;
      case 'i':
        item_name = optarg;
        break;
      case ':':
        errors.add("excxx_example_database_read",
                   "1",
                   "Option -" + char_to_string((char) ch) + " requires an argument");
      case '?':
      default:
        errors.add("excxx_example_database_read", "2", "Invalid option '-" + char_to_string((char) ch));
        break;
    }

  std::string db_config_filename = base_name + "/example-databases.json";

  if (!errors.has()) {
    json_object *bdb_json = json_object_from_file(db_config_filename.c_str());
    if (bdb_json != nullptr) {
      bdb_databases_config.from_json(bdb_json, errors);
    } else
      errors.add("excxx_example_database_read",
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

        Secondary_database_config itemname_secondary_database_config;
        inventory_primary_database_config.select("item_name", itemname_secondary_database_config, errors);
        Secondary_database item_name_sdb(itemname_secondary_database_config, db_home, errors);

        if (!errors.has()) {
          if (item_name.empty()) {
            show_all_records(inventory_db.bdb_db, vendor_db.bdb_db, errors);
          } else {
            show_item(item_name_sdb.bdb_db, inventory_db.bdb_db, vendor_db.bdb_db, item_name, errors);
          }
        }
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

  // Bdb_db class constructors will close the databases when they
  // go out of scope.
  return (0);
} // End main



// Shows the records in the inventory database that
// have a specific item name. For each inventory record
// shown, the appropriate vendor record is also displayed.
int show_item(Bdb_dbp &item_name_sdb,
              Bdb_dbp &inventory_db,
              Bdb_dbp &vendor_db,
              const std::string &item_name,
              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(item_name_sdb, errors);
  if (!errors.has()) {
    Inventory_DTO_key_list inventory_dto_key_list;
    Inventory_item_name_key inventory_item_name_key(item_name);
    bdb_cursor.dto_list_get_key<Inventory_item_name_key,
                                Inventory_DTO_key,
                                Inventory_DTO_key_list>(inventory_item_name_key,
                                                        inventory_dto_key_list,
                                                        errors);
    Inventory_DTO_list inventory_dto_list;

    Bdb_DAO::select_by_key_list<Inventory_DTO_key,
                                Inventory_DTO,
                                Inventory_DTO_key_list,
                                Inventory_DTO_list>
        (inventory_db, inventory_dto_key_list, inventory_dto_list, errors);
    for (Inventory_DTO inventoryItem: inventory_dto_list.list) {
      inventoryItem.show();
      show_vendor(vendor_db, inventoryItem.getVendor(), errors);
    }
  }
  if (errors.has())
    std::cerr << errors.to_string();

  // Bdb_db class constructors will close the databases when they
  // go out of scope.
  return (0);
}

// Shows all the records in the inventory database.
// For each inventory record shown, the appropriate
// vendor record is also displayed.
int show_all_records(Bdb_dbp &inventory_db, Bdb_dbp &vendor_db, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(inventory_db, errors);
  if (!errors.has()) {
    Inventory_DTO_list inventory_dto_list;
    bdb_cursor.dto_list_get_all<Inventory_DTO_key,
                                Inventory_DTO,
                                Inventory_DTO_list>(inventory_dto_list, errors);
    for (Inventory_DTO inventoryItem: inventory_dto_list.list) {
      inventoryItem.show();
      show_vendor(vendor_db, inventoryItem.getVendor(), errors);
    }
  }
  return (0);
}

// Shows a vendor record. Each vendor record is an instance of
// a vendor structure. See load_vendor_db() in
// example_database_load for how this structure was originally
// put into the database.
int show_vendor(Bdb_dbp &vendor_db, const std::string &vendor, Bdb_errors &errors) {
  Vendor_DTO vendor_dto;
  Vendor_DTO_key vendor_dto_key(vendor);
  Bdb_DAO::lookup<Vendor_DTO_key,
                  Vendor_DTO>(vendor_db, vendor_dto_key, vendor_dto, errors);
  std::cout << "        " << vendor_dto.street << "\n"
            << "        " << vendor_dto.city << ", "
            << vendor_dto.state << "\n"
            << "        " << vendor_dto.zipcode << "\n"
            << "        " << vendor_dto.phone_number << "\n"
            << "        Contact: " << vendor_dto.sales_rep << "\n"
            << "                 " << vendor_dto.sales_rep_phone
            << std::endl;

  return (0);
}
