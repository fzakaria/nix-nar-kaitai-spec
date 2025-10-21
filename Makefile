.PHONY: generate clean

nix_nar.o: generated/nix_nar.h generated/nix_nar.cpp
	$(CXX) -std=c++11 -c generated/nix_nar.cpp -o nix_nar.o -lkaitai_struct_cpp_stl_runtime

nar-ls: nar_ls.cc generated/nix_nar.h nix_nar.o
	$(CXX) -std=c++17 nix_nar.o -o nar-ls nar_ls.cc -lkaitai_struct_cpp_stl_runtime

generate:
	@ksc NAR.ksy --target cpp_stl --cpp-standard 11 \
				 --outdir generated

clean:
	rm -rf generated
	rm -rf nar-ls nix_nar.o