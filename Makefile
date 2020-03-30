HTSLIB_DIR=src/htslib
.PHONY = install $(HTSLIB_DIR) clean

PKG_LIBS = -lhts -L$(HTSLIB_DIR) -Wl,-rpath=$(HTSLIB_DIR)
PKG_CPPFLAGS = -I$(HTSLIB_DIR) -Wall -O2

install: $(HTSLIB_DIR)/libhts.so bin/alpha_satellite_counter

$(HTSLIB_DIR):
	git submodule add https://github.com/samtools/htslib.git $@

$(HTSLIB_DIR)/libhts.so: $(HTSLIB_DIR)
	cd $< && make
	
bin/alpha_satellite_counter: $(HTSLIB_DIR)/libhts.so src/main.cpp
	mkdir -p bin
	g++ src/main.cpp $(PKG_LIBS) $(PKG_CPPFLAGS) -o $@

clean:
	rm bin/alpha_satellite_counter

