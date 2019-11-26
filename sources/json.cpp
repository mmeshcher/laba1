// Copyright 2019 <mmeshcher>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <any>
#include <utility>
#include <algorithm>
#include <json.h>

Json::Json()
{
    this->type = JUNKNOWN;
}
Json::Json(const std::string& s)
{
    Json::parse(s);
}

std::any Json::ConvertType(const std::string& str, Type tp)
{
    if (tp == JSTRING)
        return std::any(str);
    if (tp == JBOOLEAN)
        return std::any(str == "true" ? true : false);
    if (tp == JNUMBER)
        return std::any(atoi(str.c_str()));
    return std::any(str);
}

void Json::set_type(Type tp) {
    type = tp;
}

void Json::add_property(std::string new_key, Json v) {
    if (v.type != JARRAY && v.type != JOBJECT)
    {
        prop.push_back(make_pair(new_key, ConvertType(v.key, v.type)));
    } else {
        prop.push_back(make_pair(new_key, std::any(v)));
    }
}

void Json::add_element(Json v) {
    if (v.type != JARRAY && v.type != JOBJECT)
    {
        arr.push_back(ConvertType(v.key, v.type));
    } else {
        arr.push_back(std::any(v));
    }
}

void Json::set_string(std::string s) {
    if (type == JSTRING)
        value = std::any(s);
    if (type == JNUMBER)
        value = std::any(atoi(s.c_str()));
    if (type == JBOOLEAN)
        value = std::any(s == "true" ? true : false);
    key = s;
}

std::any& Json::operator[](int i) {
    if (type == JARRAY) {
        return arr[i];
    }
    if (type == JOBJECT) {
        return prop[i].second;
    }
    throw std::logic_error("Index out of range");
}
std::any& Json::operator[](const std::string& s) {
    auto found = std::find_if(prop.begin(), prop.end(), [s](const auto &a) {
        return a.first == s;
    });
    if (found == prop.end())
    {
        throw std::logic_error("Invalid property name");
    }

    return (*found).second;
}

bool Json::is_array() const
{
    return type == JARRAY;
}

bool Json::is_object() const
{
    return type == JOBJECT;
}

bool Json::IsWhitespace(const char c) {
    return isspace(c);
}
int Json::NextWhitespace(const std::string& source, int i) {
    while (i < static_cast<int>(source.length())) {
        if (source[i] == '"') {
            i++;
            while (i < static_cast<int>(source.length())
            && (source[i] != '"' || source[i - 1] == '\\')) i++;
        }
        if (source[i] == '\'') {
            i++;
            while (i < static_cast<int>(source.length())
            && (source[i] != '\'' || source[i - 1] == '\\')) i++;
        }
        if (IsWhitespace(source[i])) return i;
        i++;
    }
    return static_cast<int>(source.length());
}
int Json::SkipWhitespaces(const std::string& source, int i) {
    while (i < static_cast<int>(source.length())) {
        if (!IsWhitespace(source[i])) return i;
        i++;
    }
    return -1;
}

std::vector<Token> Json::Tokenize(std::string source) {
    source += " ";
    std::vector<Token> tokens;
    int index = SkipWhitespaces(source, 0);
    while (index >= 0) {
        int next = NextWhitespace(source, index);
        std::string str = source.substr(index, next - index);

        size_t k = 0;
        while (k < str.length()) {
            if (str[k] == '"') {
                size_t tmp_k = k + 1;
                while (tmp_k < str.length() && (str[tmp_k] != '"'
                || str[tmp_k - 1] == '\\')) tmp_k++;
                tokens.push_back(
                    Token(str.substr(k + 1, tmp_k - k - 1), STRING));
                k = tmp_k + 1;
                continue;
            }
            if (str[k] == '\'') {
                size_t tmp_k = k + 1;
                while (tmp_k < str.length() && (str[tmp_k] != '\''
                || str[tmp_k - 1] == '\\')) tmp_k++;
                tokens.push_back(
                    Token(str.substr(k + 1, tmp_k - k - 1), STRING));
                k = tmp_k + 1;
                continue;
            }
            if (str[k] == ',') {
                tokens.push_back(Token(",", COMMA));
                k++;
                continue;
            }
            if (str[k] == 't' && k + 3 < str.length()
                && str.substr(k, 4) == "true") {
                tokens.push_back(Token("true", BOOLEAN));
                k += 4;
                continue;
            }
            if (str[k] == 'f' && k + 4 < str.length()
                && str.substr(k, 5) == "false") {
                tokens.push_back(Token("false", BOOLEAN));
                k += 5;
                continue;
            }
            if (str[k] == 'n' && k + 3 < str.length()
                && str.substr(k, 4) == "null") {
                tokens.push_back(Token("null", NUL));
                k += 4;
                continue;
            }
            if (str[k] == '}') {
                tokens.push_back(Token("}", CROUSH_CLOSE));
                k++;
                continue;
            }
            if (str[k] == '{') {
                tokens.push_back(Token("{", CROUSH_OPEN));
                k++;
                continue;
            }
            if (str[k] == ']') {
                tokens.push_back(Token("]", BRACKET_CLOSE));
                k++;
                continue;
            }
            if (str[k] == '[') {
                tokens.push_back(Token("[", BRACKET_OPEN));
                k++;
                continue;
            }
            if (str[k] == ':') {
                tokens.push_back(Token(":", COLON));
                k++;
                continue;
            }
            if (str[k] == '-' || (str[k] <= '9' && str[k] >= '0')) {
                size_t tmp_k = k;
                if (str[tmp_k] == '-') tmp_k++;
                while (tmp_k < str.size() && ((str[tmp_k] <= '9'
                && str[tmp_k] >= '0') || str[tmp_k] == '.')) tmp_k++;
                tokens.push_back(Token(str.substr(k, tmp_k - k), NUMBER));
                k = tmp_k;
                continue;
            }
            tokens.push_back(Token(str.substr(k), UNKNOWN));
            k = str.length();
        }

        index = SkipWhitespaces(source, next);
    }
    return tokens;
}

Json Json::JsonParse(std::vector<Token> v, int i, int& r) {
    Json current;
    try
    {
        if (v[i].m_type == CROUSH_OPEN) {
            current.set_type(JOBJECT);
            int k = i + 1;
            while (v[k].m_type != CROUSH_CLOSE) {
                std::string key = v[k].m_value;
                k += 2; // k+1 should be ':'
                int j = k;
                Json vv = JsonParse(v, k, j);
                current.add_property(key, vv);
                k = j;
                if (v[k].m_type == COMMA) k++;
            }
            r = k + 1;
            return current;
        }
        if (v[i].m_type == BRACKET_OPEN) {
            current.set_type(JARRAY);
            int k = i + 1;
            while (v[k].m_type != BRACKET_CLOSE) {
                int j = k;
                Json vv = JsonParse(v, k, j);
                current.add_element(vv);
                k = j;
                if (v[k].m_type == COMMA) k++;
            }
            r = k + 1;
            return current;
        }
        if (v[i].m_type == NUMBER) {
            current.set_type(JNUMBER);
            current.set_string(v[i].m_value);
            r = i + 1;
            return current;
        }
        if (v[i].m_type == STRING) {
            current.set_type(JSTRING);
            current.set_string(v[i].m_value);
            r = i + 1;
            return current;
        }
        if (v[i].m_type == BOOLEAN) {
            current.set_type(JBOOLEAN);
            current.set_string(v[i].m_value);
            r = i + 1;
            return current;
        }
        if (v[i].m_type == NUL) {
            current.set_type(JNULL);
            current.set_string("null");
            r = i + 1;
            return current;
        }
    }
    catch (std::exception e)
    {
        std::cout << "Exception";
    }
    return current;
}

Json Json::parse(const std::string& str) {
    int k;
    return JsonParse(Tokenize(str), 0, k);
}

Json Json::parseFile(const std::string& filename) {
    std::ifstream in(filename.c_str());

    std::string str = "";
    std::string tmp;

    while (getline(in, tmp)) str += tmp;
    in.close();
    return Json::parse(str);
}
