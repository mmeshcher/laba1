// Copyright 2019 mmeshcher

#ifndef INCLUDE_JSON_HPP_
#define INCLUDE_JSON_HPP_

#include <any>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

using std::any;
using std::any_cast;
using std::cin;
using std::cout;
using std::map;
using std::string;
using std::vector;

class Json {
 private:
  map<string, any> json_map;
  vector<any> json_arr;

  bool is_json_array = false;
  bool is_json_object = false;

  void create_vector(const string& s);
  void create_map(const string& s);

 public:
  explicit Json(const string& s);

  bool is_array() const;

  bool is_object() const;

  std::any& operator[](const string& key);

  std::any& operator[](int index);

  static Json parse(const string& s);

  static Json parseFile(const string& path_to_file);

  static unsigned int skip_spaces(unsigned int i, const string& s);

  static string read_word(unsigned int& i, const string& s);

  static unsigned int find_end(unsigned int i, const string& s);

  static string cut_num(unsigned int i, const string& s);
};

#endif  // INCLUDE_JSON_HPP_
