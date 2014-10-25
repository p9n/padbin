env = Environment(parse_flags = "-O3 -pedantic -Wall -Wvla -std=c++11 -pthread -lboost_filesystem -lboost_system")  # Initialize the environment
# env.Append(CPPFLAGS = "-O3 -pedantic -Wall -Wvla -std=c++11 -pthread".split())
# env.Append(LINKFLAGS = "-O3 -pedantic -Wall -Wvla -std=c++11 -pthread".split())

main_lib = env.StaticLibrary('padbin', ['bin_decode.cpp', 'monster_data.cpp', 'skill_data.cpp', 'box_data.cpp', 'pad_db.cpp'])
Default(main_lib)
Default(env.Program('main', ['main.cpp', main_lib]))
Default(env.Program('decode', ['decode.cpp', main_lib]))
Default(env.Program('unittest', ['pad_db_unittest.cpp', main_lib], LIBS = env['LIBS'] + ['gtest', 'gtest_main']))

env.AlwaysBuild(env.Alias('test', [], './unittest --gtest_color=yes'))
