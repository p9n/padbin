env = Environment()  # Initialize the environment
env.Append(CPPFLAGS = "-O3 -pedantic -Wall -Wvla -std=c++11".split())

env.Program('main', ['main.cpp', 'bin_decode.cpp', 'monster_data.cpp', 'skill_data.cpp', 'box_data.cpp', 'pad_db.cpp'])
env.Program('decode', ['decode.cpp', 'bin_decode.cpp'])
