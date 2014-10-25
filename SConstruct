env = Environment(parse_flags = "-O3 -pedantic -Wall -Wvla -Wold-style-cast -std=c++11 -pthread -lboost_filesystem -lboost_system")  # Initialize the environment

env.VariantDir(variant_dir = 'build', src_dir = 'src', duplicate = 0)

main_lib = env.StaticLibrary('out/padbin', ['build/bin_decode.cpp', 'build/monster_data.cpp', 'build/skill_data.cpp', 'build/box_data.cpp', 'build/pad_db.cpp'])
Default(main_lib)
Default(env.Program('out/main', ['build/main.cpp', main_lib]))
Default(env.Program('out/decode', ['build/decode.cpp', main_lib]))
Default(env.Program('out/unittest', ['build/pad_db_unittest.cpp', main_lib], LIBS = env['LIBS'] + ['gtest', 'gtest_main']))

env.AlwaysBuild(env.Alias('test', [], './out/unittest --gtest_color=yes'))
