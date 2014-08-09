add_executable (Library msConGra.cpp KeyMaker.cpp Library.cpp LibApp.cpp)
add_executable (KeyMaker msConGra.cpp KeyMaker_MakeKey.cpp MakeKey.cpp)
install (TARGETS Library KeyMaker DESTINATION bin)
install (FILES Application.sif Books.dat Members.dat DESTINATION bin)
