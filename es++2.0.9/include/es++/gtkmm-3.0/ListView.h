/******************************************************************************
 *
 * Copyright (c) 2019  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *  
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *  ListView.h
 *
 *****************************************************************************/

#pragma once

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include <gtkmm/window.h>
#include <gtkmm/treeview.h>

#include <es++/File.h>
#include <es++/CharFileReader.h>
#include <es++/StringTokenizerT.h>

namespace Es {

//Simple TextListView
//All TreeModelColumns are ustring-type :
//Gtk::TreeModelColumn<Glib::ustring> type 

class ListView: public Gtk::TreeView { 
private:
  size_t n_columns;
  std::vector<Gtk::TreeModelColumn<Glib::ustring >> 
                                columnsModel;
  Gtk::TreeModel::ColumnRecord  record;
  Glib::RefPtr<Gtk::ListStore>  listStore;

public:
  ListView()
  :n_columns(0)
  ,listStore(nullptr)
  {

  }

  void createModel(const char* column_names[], size_t columns)
  {
    clear();

    n_columns= columns;

    //1. Create columnsModel having a number of n_columns of ustring type
    //All columns are ustring-type :Gtk::TreeModelColumn<Glib::ustring> type 
    Gtk::TreeModelColumn<Glib::ustring> column_string;

    for (size_t i = 0; i<n_columns; i++) {
       columnsModel.push_back(column_string);
       record.add(columnsModel.at(i) );
    }

    //2. Create a listStore model from the ColumnRecord record. 
    listStore = Gtk::ListStore::create(record);

    //3. Set the model listStore to this treeview.
    set_model(listStore);

    //4. Append columns column_names to this list_view.
    appendColumns(column_names, n_columns);
  }

  
  void createModel(std::vector<std::string>& names)
  {
    clear();

    n_columns= names.size();

    //1. Create columnsModel having a number of n_columns of ustring type
    //All columns are ustring-type :Gtk::TreeModelColumn<Glib::ustring> type 
    Gtk::TreeModelColumn<Glib::ustring> column_string;

    for (size_t i = 0; i<n_columns; i++) {
       columnsModel.push_back(column_string);
       record.add(columnsModel.at(i) );
    }
 
    //2. Create a listStore model from the record.
    listStore = Gtk::ListStore::create(record);

    //3. Set the model listStore to this treeview.
    set_model(listStore);

    //4. Append columns column_names to this list_view.
    appendColumns(names);
  }

  void appendColumn(size_t column, const char* name)
  {
    if (column <n_columns) {
      Gtk::TreeModelColumn<Glib::ustring > col_string= columnsModel.at(column);
      append_column(name,  col_string);
    } else {
      //Ignore
    }
  }
  
  void appendColumn(size_t column, const std::string& name)
  {
    if (column <n_columns) {
      Gtk::TreeModelColumn<Glib::ustring > col_string = columnsModel.at(column);
      append_column(name.c_str(),  col_string );
    } else {
      //Ignore
    }
  }

  void appendColumns(const char** names, size_t len)
  {
    if (names != nullptr && len <= n_columns) {
      for (size_t i = 0;i<len; i++) {
        if (names[i] != nullptr) {
          appendColumn(i, names[i]);
        }
      }
    } else {

    }
  }

  void appendColumns(std::vector<std::string>& names)
  {
    if (names.size() <= n_columns) {
      size_t i = 0;
      for (auto it = names.begin(); it != names.end(); it++) {
        const std::string& name = *it;   
        appendColumn(i++, name);
      }  
    } else {
    }
  }


  void addRow(int column, std::string& item)
  {
    Gtk::TreeModel::Row  row = *(listStore->append());
    row[columnsModel.at(column)] = item.c_str();
  }
  
  void addRow(const char* items[], size_t len)
  {
    Gtk::TreeModel::Row  row = *(listStore->append());
    size_t column = 0;
    for (size_t i = 0; i<len; i++) {
        row[columnsModel.at(column++)] = items[i];
    }
  }

  void addRow(std::vector<std::string>& items)
  {
    Gtk::TreeModel::Row  row = *(listStore->append());
    size_t column = 0;
    if (items.size() <= n_columns) {
      for (auto it = items.begin(); it != items.end(); it++) {
        row[columnsModel.at(column++)] = *it;
      }
    }
  }

  void clear()
  {
    clearColumnsModel();
    clearColumns();
    clearRows();
  }

  void clearColumns()
  {
    // Remove all columns
    remove_all_columns();
  }

  void clearColumnsModel()
  {
    // Remove all columns
    columnsModel.clear();

    columnsModel.shrink_to_fit();
  }

  void clearRows()
  {
    if (listStore) {
      listStore->clear();
    }
  }

  void parseCSVHeader(const char* line, std::vector<std::string>& items)
  {
    StringTokenizerT<char> tokenizer(line);
    while(tokenizer.hasMoreToken()) {
      char token[256] = { 0 };
      tokenizer.getToken(token, sizeof(token));
      items.push_back(std::string(token)); 
    }
  }

  //line is a set of space, comma, or tab separated strings.
  void addLine(const char* line)
  {
    std::vector<std::string> items;
    StringTokenizerT<char> tokenizer(line);
    while(tokenizer.hasMoreToken()) {
      char token[128] = { 0 };
      tokenizer.getToken(token, sizeof(token));
      items.push_back(std::string(token));
    }
    addRow(items);
  }

  void readCSVFile(const std::string& filename)
  {
    std::ifstream ifs(filename);
    if (!ifs) {
      throw IException("File not found %s", filename.c_str());
    }
    std::string line;
    bool first_line = true;

    while (!ifs.eof()) {
      std::getline(ifs, line);
      //Erase ¥n if line ends with it.
      if (line[line.size()-1] == '\n') {
        line.erase(line.size()-1);
      }
      //Erase ¥r if line ends with it.
      if (line[line.size()-1] == '\r') {
        line.erase(line.size()-1);
      }
      if (first_line) {
        std::vector<std::string> column_names;
        parseCSVHeader(line.c_str(), column_names);

        createModel(column_names);

        first_line = false;
      } else {
        addLine(line.c_str());
      }
    }
  }
};

}

