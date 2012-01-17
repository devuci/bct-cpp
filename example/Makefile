.PHONY: clean

example: example.cpp
	$(CXX) $(CXXFLAGS) -lbct -lgsl -lgslcblas -o $@ $<

clean:
	-rm example
