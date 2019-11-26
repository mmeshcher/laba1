//Copyright 2019 <mmeshcher>
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    std::ifstream in(" C : / Git / lab - 01 - parser / file.json ");
    json obj = json::parse(in);
    in.close();
    auto arr1 = obj[0];
    auto arr2 = obj[1];
    auto arr3 = obj[2];
    std::ofstream o("file2.json");
    o << "[\n";
    for (int i = 0; i < 3; i++) {
        o << "    " << "{ \"ticker\" : " << arr1[i] << ", \"id\": "
            "" << arr2[i] << ", \"description\": " << arr3[i] << "}";
        if (i != 2) o << ",";
        o << "\n";
    }
    o << "]\n";
    o.close();
    std::ifstream in1("file2.json");
    return 1;
}
