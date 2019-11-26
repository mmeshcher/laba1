// Copyright 2019 mmeshcher
#include "json.h"

int main() {
    std::string text;
    text =
        R"({ "lastname" : "Ivanov" , "firstname" : "Ivan" , "age" : 25,"islegal" : false,"marks" : [ 4 , 5 , 5 , 5 , 2 , 3 ],"address" : {  "city" : "Moscow" ,  "street" : "Vozdvijenka" } })";
    Json object = Json::parseFile(text);
    std::cout << endl;
    if (object.is_object())
        std::cout << "Yes";

    return 0;
}
