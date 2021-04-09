#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <map>

class AutoObj {
 public:
  AutoObj(const std::function<void(void)>& objInit, const std::function<void(void)>& objDel) : _objDel(objDel) { objInit(); }
  AutoObj(const AutoObj& r) { _objDel = r._objDel; }
  ~AutoObj() { _objDel(); }

 protected:
  std::function<void(void)> _objDel;
};


AutoObj MakeAutoObj(const std::function<void(void)>& objInit, const std::function<void(void)>& objDel) {
  return AutoObj(objInit, objDel);
}

void beg1() {
    std::cout << "begin1" << std::endl;
}

void beg2() {
    std::cout << "begin2" << std::endl;
}

void del1() {
    std::cout << "del1" << std::endl;
}

void del2() {
    std::cout << "del2" << std::endl;
}

int main()
{
    auto param1 = MakeAutoObj([]() { beg1(); }, []() { del1(); });
    auto param2 = MakeAutoObj([]() { beg2(); }, []() { del2(); });

    std::cout << "main" << std::endl;

    std::string str_resume = "thisistestresume";

    std::string sub_str_resume = str_resume.substr(0, str_resume.size() - 6);
    uint64_t i_64 = 64;
    sub_str_resume += std::to_string(i_64);

    std::cout << sub_str_resume << std::endl;



    std::map<std::string, std::vector<int>> map_int;
    std::vector<int> vec{0, 1, 2, 3, 4, 5};
    map_int.insert(std::make_pair("test", vec));

    std::cout << map_int.size() << std::endl;
    std::cout << map_int.find("test")->second.size() << std::endl;

    std::vector<int>& vec_new = map_int.find("test")->second;
    vec_new.push_back(6);
    map_int.insert(std::make_pair("test", vec_new));

    std::cout << map_int.size() << std::endl;
    std::cout << map_int.find("test")->second.size() << std::endl;
}