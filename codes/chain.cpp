//
// Created by yalavrinenko on 15.04.2021.
//
#include <iostream>
#include <string>
#include <memory>
#include <vector>

class istudent{
public:
  std::string say_name() const { return say_name_impl(); }
  double get_score() const { return get_score_impl(); }
  virtual ~istudent() = default;
private:
  virtual std::string say_name_impl() const = 0;
  virtual double get_score_impl() const = 0;
};

template<typename req_t>
class handler{
public:
  void handle_student(req_t& rq) { handle_impl(rq); }
  void set_next(std::shared_ptr<handler<req_t>> next) { set_next_impl(std::move(next));}
  virtual ~handler() = default;
private:
  virtual void handle_impl(req_t& student) = 0;
  virtual void set_next_impl(std::shared_ptr<handler<req_t>> next) = 0;
};

class common_institute: public handler<istudent> {
protected:
  void handle_impl(istudent &student) override {
    if (auto lock = next_.lock()){
      lock->handle_student(student);
    } else {
      throw std::runtime_error("End of chain");
    }
  }

private:
  void set_next_impl(std::shared_ptr<handler<istudent>> next) final {
    if (!next_.expired()){
      next->set_next(next_.lock());
    }
    next_ = next;
  }

  std::weak_ptr<handler<istudent>> next_;
};

class mipt : public common_institute {
  void handle_impl(istudent &student) override {
    if (student.get_score() > 8.0 && !student.say_name().empty())
      std::cout << "Mipt catch student << " << student.say_name() << "!" << std::endl;
    else
      common_institute::handle_impl(student);
  }
};

class jiht: public common_institute{
  void handle_impl(istudent &student) override {
    if (student.get_score() > 5.0 && student.say_name().find("Vladimirovich") != std::string::npos)
      std::cout << "jiht catch student << " << student.say_name() << "!" << std::endl;
    else
      common_institute::handle_impl(student);
  }
};

class sharaga : public common_institute{
  void handle_impl(istudent &student) override {
    if (!student.say_name().empty())
      std::cout << "Sharaga catch student << " << student.say_name() << "!" << std::endl;
    else
      common_institute::handle_impl(student);
  }
};

class Chain{
public:
  void add_institute(std::shared_ptr<handler<istudent>> ins) {
    ins->set_next(head_);
    processors_.emplace_back(ins);
    head_ = std::move(ins);
  }
  void handle(istudent& student) { head_->handle_student(student); }
private:
  std::vector<std::shared_ptr<handler<istudent>>> processors_;
  std::shared_ptr<handler<istudent>> head_ = {nullptr};
};

class GoogStudent: public istudent{
private:
  std::string say_name_impl() const override {
    return "Ivanov Ivan";
  }

  double get_score_impl() const override {
    return 9.0;
  }
};

class MediumStudent: public istudent{
private:
  std::string say_name_impl() const override {
    return "Ivanov Stepan";
  }

  double get_score_impl() const override {
    return 7.0;
  }
};

class VladimirovichStudent: public istudent{
private:
  std::string say_name_impl() const override {
    return "Stepan Vladimirovich";
  }

  double get_score_impl() const override {
    return 7.0;
  }
};

class BadStudent: public istudent{
private:
  std::string say_name_impl() const override {
    return "Say my name!";
  }

  double get_score_impl() const override {
    return 2.0;
  }
};

int main(int argc, char** argv){
  Chain institutes;
  institutes.add_institute(std::make_unique<sharaga>());
  institutes.add_institute(std::make_unique<jiht>());
  institutes.add_institute(std::make_unique<mipt>());

  std::vector<istudent*> students{
    new GoogStudent(), new MediumStudent(), new VladimirovichStudent(), new BadStudent()
  };
  for (auto s : students)
    institutes.handle(*s);

  for (auto &s : students)
    delete s;
  return 0;
}
