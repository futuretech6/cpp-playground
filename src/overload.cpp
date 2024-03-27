#include <iostream>

class Integer {
  private:
    int value;

  public:
    Integer(int val) : value(val) {}

    // 重载前置递增运算符 ++i
    Integer &operator++() {
        ++value;
        return *this;
    }

    // 重载后置递增运算符 i++
    Integer operator++(int) {
        Integer temp(value);
        ++value;
        return temp;
    }

    // 获取当前值
    int getValue() const { return value; }
};

int main() {
    Integer num(5);

    // 前置递增运算符 ++i
    ++num;
    std::cout << "After pre-increment: " << num.getValue() << std::endl;

    // 后置递增运算符 i++
    Integer num2 = num++;
    std::cout << "After post-increment: " << num2.getValue() << std::endl;
    std::cout << "Original value after post-increment: " << num.getValue() << std::endl;

    return 0;
}
