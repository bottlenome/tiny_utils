#include <iostream>
#include <chrono>

enum Function{
  A_,
  B_,
};

volatile int sum = 0;

class A
{
public:
  void operator()(){
      sum += 1;
  }
};

A a_;

class B
{
public:
  void operator()(){
    sum += 2;
  }
};

void a_func(){
  sum += 1;
}

void b_func(){
  sum += 2;
}

void exec_switch(Function f){
  switch(f){
  case A_:
    a_func();
    break;
  case B_:
    b_func();
    break;
  default:
    std::cout << "default" << std::endl;
  }
}

template<class Functor>
void exec_functor(Functor f){
  f();
}

template<class Functor>
void apply(Functor f){
  auto t1 = std::chrono::system_clock::now();
  for(int i = 0; i < 100000; i++){
    f();
  }
  auto t2 = std::chrono::system_clock::now();
  auto elasped = t2 - t1;
  std::cout << elasped.count() << std::endl;
}

int main(int argc, char const* argv[])
{
  Function tmp = A_;
  apply([&](){exec_switch(tmp);});
  apply([](){exec_functor(a_);});

  return 0;
}
