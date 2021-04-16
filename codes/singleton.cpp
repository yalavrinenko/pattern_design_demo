//
// Created by yalavrinenko on 14.04.2021.
//
#include <iostream>
#include <memory>

struct keyboard_impl{
  void press(char c) {}
};

class Keyboard{
public:
  Keyboard(Keyboard const&) = delete;
  Keyboard(Keyboard &&) = delete;
  Keyboard& operator=(Keyboard const&) = delete;
  Keyboard& operator=(Keyboard &&) = delete;

  static Keyboard& Instance() {
    static Keyboard kb;
    return kb;
  }

  void press_key(char c) {
    raw_kb_->press(c);
  }
private:
  Keyboard() = default;

  keyboard_impl* raw_kb_;
};

class SharedKeyboard{
public:
  SharedKeyboard(Keyboard const&) = delete;
  SharedKeyboard(Keyboard &&) = delete;
  SharedKeyboard& operator=(Keyboard const&) = delete;
  SharedKeyboard& operator=(Keyboard &&) = delete;

  static std::shared_ptr<SharedKeyboard>& Instance();

  void press_key(char c) {
    raw_kb_->press(c);
  }

protected:
  SharedKeyboard() = default;
private:
  keyboard_impl* raw_kb_;
};

std::shared_ptr<SharedKeyboard> &SharedKeyboard::Instance() {
  struct tmp_class : public  SharedKeyboard{
    tmp_class() = default;
  };

  static std::shared_ptr<SharedKeyboard> kb = std::make_shared<tmp_class>();
  return kb;
}

int main(int argc, char** argv){
  auto keyboard = SharedKeyboard::Instance();
  keyboard->press_key('c');
  auto key_copy = keyboard;
  key_copy->press_key('b');
  return 0;
}

