#include <iostream>
#include <vector>
#include <array>

class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
 public:
  virtual void VisitConcreteComponentA(const ConcreteComponentA* element) const = 0;
  virtual void VisitConcreteComponentB(const ConcreteComponentB* element) const = 0;
};

class Component {
 public:
  Component() : vec_{} {}
  Component(std::vector<int> vec) : vec_(std::move(vec)) {}
  virtual ~Component() {}
  virtual void Accept(Visitor* visitor) const = 0;

 //private:
  std::vector<int> vec_;
};

class ConcreteComponentA : public Component {
 public:
  ConcreteComponentA() : Component() {}
  ConcreteComponentA(std::vector<int> vec) : Component(vec) {}
  ConcreteComponentA(std::vector<int>&& vec) : Component(std::move(vec)) {}

  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }

  std::string ExculsiveMethodOfConreteComponentA() const {
    for (auto i : vec_) {
      std::cout << i << "\n";
    }
    return "A";
  }
};

class ConcreteComponentB : public Component {
 public:
  ConcreteComponentB() : Component() {}
  ConcreteComponentB(std::vector<int>&& vec) : Component(std::move(vec)) {}

  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }

  std::string SpecialMethodOfConreteComponentB() const {
    for (auto i : vec_) {
      std::cout << i << "\n";
    }
    return "B";
  }
};

class ConcreteVisitor1 : public Visitor {
 public:
  void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
    std::cout << element->ExculsiveMethodOfConreteComponentA() << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
    std::cout << element->SpecialMethodOfConreteComponentB() << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
 public:
  void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
    std::cout << element->ExculsiveMethodOfConreteComponentA() << " + ConcreteVisitor2\n";
  }
  void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
    std::cout << element->SpecialMethodOfConreteComponentB() << "+ ConcreteVisitor2\n";
  }
};

void ClientCode(std::array<const Component*, 2> components, Visitor* visitor) {
  for (const Component* comp : components) {
    comp->Accept(visitor);
  }
}

int main() {
  std::vector<int> vec1{1, 2, 3, 4, 5};
  std::vector<int> vec2{6, 7, 8, 9, 10};
  std::array<const Component*, 2> components = {new ConcreteComponentA{vec1}, new ConcreteComponentB{std::move(vec2)}};
  ConcreteVisitor1* visitor1 = new ConcreteVisitor1;
  ClientCode(components, visitor1);
  ConcreteVisitor2* visitor2 = new ConcreteVisitor2;
  ClientCode(components, visitor2);

  for (const Component* comp : components) {
    delete comp;
  }

  delete visitor1;
  delete visitor2;

  return 0;
}
