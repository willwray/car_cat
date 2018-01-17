project('car', 'cpp', default_options : 'cpp_std=c++1z')

test('test car array string literals',
  executable('test_car', 'test_car.cpp')
)

car_dep = declare_dependency(
  include_directories : include_directories('.')
)
