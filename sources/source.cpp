// Copyright 2019 mmeshcher
#include "Json.hpp"
using namespace std;
int main() {
  string text;
  text =
      R"({ "lastname" : "Ivanov" , "firstname" : "Ivan" , "age" : 25,"islegal" : false,"marks" : [ 4 , 5 , 5 , 5 , 2 , 3 ],"address" : {  "city" : "Moscow" ,  "street" : "Vozdvijenka" } })";
  Json object = Json::parseFile(text);
  cout << endl;
  if (object.is_object()) cout << "Yes";

  return 0;
}