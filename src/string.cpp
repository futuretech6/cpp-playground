#include <cstring>
#include <istream>
#include <ostream>

using namespace std;

class MyString {

    // Prototype for stream insertion
    friend ostream &operator<<(ostream &os, const MyString &obj);

    // Prototype for stream extraction
    friend istream &operator>>(istream &is, MyString &obj) {
        char *buff = new char[1000];
        memset(&buff[0], 0, sizeof(buff));
        is >> buff;
        obj = MyString{buff};
        delete[] buff;
        return is;
    }

    // Prototype for '+'
    // operator overloading
    friend MyString operator+(const MyString &lhs, const MyString &rhs);
    char *str;

  public:
    // No arguments constructor
    MyString();

    // pop_back() function
    void pop_bk();

    // push_back() function
    void push_bk(char a);

    // To get the length
    int get_length();

    // Function to copy the string
    // of length len from position pos
    void copy(char s[], int len, int pos);

    // Swap strings function
    void swp(MyString &rhs);

    // Constructor with 1 arguments
    MyString(char *val);

    // Copy Constructor
    MyString(const MyString &source);

    // Move Constructor
    MyString(MyString &&source);

    // Overloading the assignment
    // operator
    MyString &operator=(const MyString &rhs);

    // Destructor
    ~MyString() { delete[] str; }
};

// Overloading the assignment operator
MyString &MyString::operator=(const MyString &rhs) {
    if (this == &rhs)
        return *this;
    delete[] str;
    str = new char[strlen(rhs.str) + 1];
    strcpy(str, rhs.str);
    return *this;
}

// Overloading the plus operator
MyString operator+(const MyString &lhs, const MyString &rhs) {
    int length = strlen(lhs.str) + strlen(rhs.str);

    char *buff = new char[length + 1];

    // Copy the strings to buff[]
    strcpy(buff, lhs.str);
    strcat(buff, rhs.str);
    buff[length] = '\0';

    // String temp
    MyString temp{buff};

    // delete the buff[]
    delete[] buff;

    // Return the concatenated string
    return temp;
}

// Overloading the stream
// insertion operator
ostream &operator<<(ostream &os, const MyString &obj) {
    os << obj.str;
    return os;
}

// Function for swapping string
void MyString::swp(MyString &rhs) {
    MyString temp{rhs};
    rhs   = *this;
    *this = temp;
}

// Function to copy the string
void MyString::copy(char s[], int len, int pos) {
    for (int i = 0; i < len; i++) {
        s[i] = str[pos + i];
    }
    s[len] = '\0';
}

// Function to implement push_bk
void MyString::push_bk(char a) {
    // Find length of string
    int length = strlen(str);

    char *buff = new char[length + 2];

    // Copy character from str
    // to buff[]
    for (int i = 0; i < length; i++) {
        buff[i] = str[i];
    }
    buff[length]     = a;
    buff[length + 1] = '\0';

    // Assign the new string with
    // char a to string str
    *this = MyString{buff};

    // Delete the temp buff[]
    delete[] buff;
}

// Function to implement pop_bk
void MyString::pop_bk() {
    int length = strlen(str);
    char *buff = new char[length];

    // Copy character from str
    // to buff[]
    for (int i = 0; i < length - 1; i++)
        buff[i] = str[i];
    buff[length - 1] = '\0';

    // Assign the new string with
    // char a to string str
    *this = MyString{buff};

    // delete the buff[]
    delete[] buff;
}

// Function to implement get_length
int MyString::get_length() {
    return strlen(str);
}

// Function to illustrate Constructor
// with no arguments
MyString::MyString() : str{nullptr} {
    str    = new char[1];
    str[0] = '\0';
}

// Function to illustrate Constructor
// with one arguments
MyString::MyString(char *val) {
    if (val == nullptr) {
        str    = new char[1];
        str[0] = '\0';
    }

    else {

        str = new char[strlen(val) + 1];

        // Copy character of val[]
        // using strcpy
        strcpy(str, val);
        str[strlen(val)] = '\0';
    }
}

// Function to illustrate
// Copy Constructor
MyString::MyString(const MyString &source) {
    str = new char[strlen(source.str) + 1];
    strcpy(str, source.str);
}

// Function to illustrate
// Move Constructor
MyString::MyString(MyString &&source) {
    str        = source.str;
    source.str = nullptr;
}

#include <iostream>

int main() {
    using namespace std;
    // 测试构造函数和赋值运算符重载
    MyString str1("Hello");
    MyString str2 = "World";
    MyString str3;
    str3 = str1;

    cout << "str1: " << str1 << endl;
    cout << "str2: " << str2 << endl;
    cout << "str3: " << str3 << endl;

    // 测试输入输出流重载
    cout << "Enter a string: ";
    cin >> str3;
    cout << "You entered: " << str3 << endl;

    // 测试字符串连接运算符重载
    MyString str4 = str1 + str2;
    cout << "str4: " << str4 << endl;

    // 测试其他功能
    cout << "Length of str1: " << str1.get_length() << endl;
    str1.pop_bk();
    cout << "After popping the last character, str1: " << str1 << endl;
    str1.push_bk('!');
    cout << "After pushing '!', str1: " << str1 << endl;

    return 0;
}
