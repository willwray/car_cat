project('car_cat', 'cpp',
  default_options : 'cpp_std=c++1z')

subprojects = ['car', 'cat']

foreach s : subprojects
    dep = s + '_dep'
    set_variable(dep, subproject(s).get_variable(dep))
endforeach

test('car_cat example',
  executable('var_cat', 'var_cat_example.cpp',
    dependencies : [car_dep, cat_dep],
  )
)
