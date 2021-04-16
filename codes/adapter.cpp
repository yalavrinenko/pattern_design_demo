//
// Created by yalavrinenko on 15.04.2021.
//
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <vector>
#include <functional>

class ifuction{
public:
  double evaluate(double x) const { return evaluate_impl(x); }
  virtual ~ifuction() = default;
private:
  virtual double evaluate_impl(double x) const = 0;
};

class poly2: public ifuction{
public:
  poly2(double a, double b, double c): a{a}, b{b}, c{c} {}
private:
  double evaluate_impl(double x) const override {
    return a * x * x + b * x + c;
  }
  double a, b, c;
};

class small_sin: public ifuction {
public:
  small_sin() = default;
private:
  double evaluate_impl(double x) const override {
    return (x < 0.1) ? x : std::sin(x);
  }
};

template<typename T>
struct function_tag {
  static constexpr int tag = T::tag;
};
template<>
struct function_tag<poly2> {
  static constexpr int tag = 1;
};
template<>
struct function_tag<small_sin> {
  static constexpr int tag = 2;
};

template<typename T>
class virtual_entry{
public:
  void read(T &t) { read_impl(t); }
  virtual ~virtual_entry() = default;
private:
  virtual void read_impl(T&) = 0;
};

template <typename ... T>
class virtual_reader: public virtual_entry<T>... {
public:
  template<typename TArg>
  void read(TArg &&arg) { virtual_entry<typename std::decay<TArg>::type>::read(std::forward<TArg>(arg)); }
};

class ireader: public virtual_reader<double, int> {
};

template<typename T>
class stream_reader: public ireader{
public:
  explicit stream_reader(T &s) : stream_{s} {}
private:
  void read_impl(double &t) override { stream_ >> t; }
  void read_impl(int &t) override { stream_ >> t;  }
  T &stream_;
};

class function_factory{
public:
  using create_function = std::function<std::unique_ptr<ifuction>(ireader&)>;

  template<class T>
  void register_creator(create_function func) {
    callbacks_[function_tag<T>::tag] = std::move(func);
  }
  std::unique_ptr<ifuction> create_object(ireader&& in, int tag) {
    return callbacks_.at(tag)(in);
  }

  static function_factory& Instance(){
    static function_factory factory;
    return factory;
  }
  function_factory(function_factory const &) = delete;
  function_factory(function_factory &&) = delete;
private:
  function_factory() = default;

  std::unordered_map<int, create_function> callbacks_;
};

using function_set = std::vector<std::unique_ptr<ifuction>>;
template<typename IStream>
function_set read_functions(IStream &input){
  function_factory::Instance().register_creator<poly2>([](ireader& in){
    double a, b, c; in.read(a); in.read(b); in.read(c); return std::make_unique<poly2>(a, b, c);
  });
  function_factory::Instance().register_creator<small_sin>([](ireader& in){
    return std::make_unique<small_sin>();
  });

  function_set fs;
  int tag;
  while (input >> tag){
    fs.emplace_back(function_factory::Instance().create_object(stream_reader<IStream>(input), tag));
  }
  return fs;
}

int main(int argc, char** argv){
  auto fset = read_functions(std::cin);

  for (auto &f : fset)
    std::cout << f->evaluate(1.0) << std::endl;
  return 0;
}
