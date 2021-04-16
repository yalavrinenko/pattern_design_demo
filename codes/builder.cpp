//
// Created by yalavrinenko on 14.04.2021.
//

#include <iostream>
#include <memory>

enum class FuelType{
  methane
};

enum class OxidizerType{
  oxygen
};

enum class PayloadType{
  stelites
};

class manual_rocket_builder;

class rocket{
public:
  rocket() = default; //new

  rocket(size_t stage_count, FuelType ft, OxidizerType ot, PayloadType pt, bool is_reusable, double target_height){}
private:
  size_t stage_count_;
  FuelType ft_;
  OxidizerType ot_;
  PayloadType pt_;
  bool is_reusable_;
  double target_height_;
  friend manual_rocket_builder;
};

class rocket_builder{
public:
  std::unique_ptr<rocket> get_rocket() const {
    return std::make_unique<rocket>(
        stage_count(),  fuel_type(), oxydizer_type(),
        payload_type(), is_reusable(), target_height()
        );
  }
  virtual ~rocket_builder() = default;
private:
  virtual size_t stage_count() const = 0;
  virtual FuelType fuel_type() const = 0;
  virtual OxidizerType oxydizer_type() const = 0;
  virtual PayloadType payload_type() const = 0;
  virtual bool is_reusable() const = 0;
  virtual double target_height() const = 0;
};

class methane_rocket: public rocket_builder{
private:
  size_t stage_count() const override { return 2; }

  FuelType fuel_type() const override { return FuelType::methane; }

  OxidizerType oxydizer_type() const override { return OxidizerType::oxygen; }

  PayloadType payload_type() const override { return PayloadType::stelites; }

  bool is_reusable() const override { return false; }

  double target_height() const override { return 128.05; }
};

class manual_rocket_builder {
public:
  manual_rocket_builder() { reset(); }
  void reset() { rocket_ = std::make_unique<rocket>(); }
  std::unique_ptr<rocket> get_rocket() { return std::move(rocket_); }

  manual_rocket_builder& set_stage_count(size_t stages)  {
    rocket_->stage_count_ = stages; return *this;
  }

  manual_rocket_builder& set_fuel_type(FuelType ft) {
    rocket_->ft_ = ft; return *this;
  }

  manual_rocket_builder& set_oxydizer_type(OxidizerType ot) {
    rocket_->ot_ = ot; return *this;
  }

  manual_rocket_builder& set_payload_type(PayloadType pt) {
    rocket_->pt_ = pt; return *this;
  }

  manual_rocket_builder& make_reusable() { rocket_->is_reusable_ = true; return *this; }

  manual_rocket_builder& set_target_height(double h) {
    rocket_->target_height_ = h; return *this;
  }
private:
  std::unique_ptr<rocket> rocket_ = nullptr;
};

int main(int argc, char** argv){
  auto predefined_rocket = methane_rocket{}.get_rocket();

  auto rocket = manual_rocket_builder()
      .set_fuel_type(FuelType::methane)
      .set_oxydizer_type(OxidizerType::oxygen)
      .set_stage_count(2)
      .set_target_height(128.05)
      .make_reusable()
      .set_payload_type(PayloadType::stelites)
      .get_rocket();
  return 0;
}