#ifndef __JSON_H__
#define __JSON_H__

#include <vector>
#include <map>
#include <string>
#include <any>
#include <utility>


enum Type {
    JSTRING,
    JOBJECT,
    JARRAY,
    JBOOLEAN,
    JNUMBER,
    JNULL,
    JUNKNOWN
};

enum TokenType {
    UNKNOWN,
    STRING,
    NUMBER,
    CROUSH_OPEN,
    CROUSH_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COMMA, COLON,
    BOOLEAN,
    NUL
};

struct Token {
    std::string m_value;
    TokenType m_type;
    Token(std::string value = "", TokenType type = UNKNOWN)
     : m_value(value), m_type(type) {}
};

class Json {
public:
    Json();
    Json(const std::string& s);

    static Json parse(const std::string& str);
    static Json parseFile(const std::string& str);

    bool is_array() const;

    bool is_object() const;

    std::any& operator[](const std::string& s);
    std::any& operator[](int i);

private:

    static bool IsWhitespace(const char c);
    static int NextWhitespace(const std::string& source, int i);
    static int SkipWhitespaces(const std::string& source, int i);

    static std::vector<Token> Tokenize(std::string source);

    static Json JsonParse(std::vector<Token> v, int i, int& r);

    void set_type(Type tp);
    void add_property(std::string new_key, Json v);
    void add_element(Json v);
    void set_string(std::string s);

    std::any ConvertType(const std::string& str, Type type);

protected:
    std::string key;
    Type type;

private:
    std::any value;
    std::vector<std::pair<std::string, std::any>> prop;
    std::vector<std::any> arr;
};

#endif // __JSON_H__
