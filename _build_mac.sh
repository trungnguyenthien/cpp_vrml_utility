clean
cmake .
cmake --build .
# dist/g4_gamoslog_to_cubic --log data/gamos.log --wrl data/g4_01.wrl
dist/rand_cube_in_vrml -f "gamma.wrl" -n 1000 -s 1