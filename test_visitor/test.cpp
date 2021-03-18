#include <iostream>
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
  virtual ~Component() {}
  virtual void Accept(Visitor* visitor) const = 0;
};

class ConcreteComponentA : public Component {
 public:
  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }

  std::string ExculsiveMethodOfConreteComponentA() const {
    return "A";
  }
};

class ConcreteComponentB : public Component {
 public:
  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }

  std::string SpecialMethodOfConreteComponentB() const {
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
  std::array<const Component*, 2> components = {new ConcreteComponentA, new ConcreteComponentB};
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
