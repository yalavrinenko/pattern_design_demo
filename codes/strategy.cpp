//
// Created by yalavrinenko on 14.04.2021.
//
#include <iostream>
#include <vector>

namespace rts_d {
  template<typename key_t, typename value_t>
  class ReadStrategy {
  public:
    std::vector<std::pair<key_t, value_t>> read(std::string const &path) {
      return read_impl(path);
    }

    virtual ~ReadStrategy() = default;

  private:
    virtual std::vector<std::pair<key_t, value_t>> read_impl(std::string const &path) = 0;
  };

  template<typename key_t, typename value_t>
  class TxtReadStrategy : public ReadStrategy<key_t, value_t> {
  private:
    std::vector<std::pair<key_t, value_t>> read_impl(const std::string &path) override {
      /*Implement txt reading*/ return {};
    }
  };

  template<typename key_t, typename value_t>
  class JsonReadStrategy : public ReadStrategy<key_t, value_t> {
  private:
    std::vector<std::pair<key_t, value_t>> read_impl(const std::string &path) override {
      /*Implement txt reading*/ return {};
    }
  };

  template<typename key_t, typename value_t>
  class XlsReadStrategy : public ReadStrategy<key_t, value_t> {
  private:
    std::vector<std::pair<key_t, value_t>> read_impl(const std::string &path) override {
      /*Implement txt reading*/ return {};
    }
  };

  template<typename key_t, typename value_t>
  class DataHolder {
  public:
    explicit DataHolder(std::string const &path, ReadStrategy<key_t, value_t> *read_strategy);

    value_t find(key_t const &) const;

  private:
    std::vector<std::pair<key_t, value_t>> data_;
  };

  template<typename key_t, typename value_t>
  DataHolder<key_t, value_t>::DataHolder(const std::string &path, ReadStrategy<key_t, value_t> *read_strategy) {
    data_ = read_strategy->read(path);
  }
}

namespace cts_d {
  template<typename key_t, typename value_t>
  struct TxtReadStrategy {
    std::vector<std::pair<key_t, value_t>> read(std::string const &path) {
      /**Read txt file**/ return {};
    }
  };

  template<typename key_t, typename value_t>
  struct JsonReadStrategy {
    std::vector<std::pair<key_t, value_t>> read(std::string const &path) {
      /**Read txt file**/ return {};
    }
  };

  template<typename key_t, typename value_t>
  struct NativeSearchStrategy {
    value_t find(std::vector<std::pair<key_t, value_t>> const &data, key_t const &key) const {
      /**Implement native search**/ return {};
    }
  };

  template<typename key_t, typename value_t,
      template<typename, typename> typename read_strategy = TxtReadStrategy,
      template<typename, typename> typename search_strategy = NativeSearchStrategy>
  class DataHolder {
  public:
    explicit DataHolder(std::string const &path,
                        read_strategy<key_t, value_t> reader = {},
                        search_strategy<key_t, value_t> searcher = {}) : data_{reader.read(path)},
                                                                         search_s_{std::move(searcher)} {
    }

    value_t find(key_t const &key) const {
      return search_s_.find(data_, key);
    }

  private:
    std::vector<std::pair<key_t, value_t>> data_;
    search_strategy<key_t, value_t> search_s_;
  };
}

int main(int argc, char **argv) {
  {
    using namespace rts_d;
    auto reader = TxtReadStrategy<int, double>();
    DataHolder<int, double> txt_holder("data.txt", &reader);

    auto json_reader = TxtReadStrategy<int, double>();
    DataHolder<int, double> json_holder("data.txt", &json_reader);

    auto xls_reader = TxtReadStrategy<int, double>();
    DataHolder<int, double> xls_holder("data.txt", &xls_reader);
  }
  {
    using namespace cts_d;
    DataHolder<int, double> txt_holder("data.txt");
    DataHolder<int, double, JsonReadStrategy> json_holder("data.txt");
  }
  return 0;
}
