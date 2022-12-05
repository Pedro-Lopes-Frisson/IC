all:
	@(if [ ! -d build ]; then mkdir build; fi; cd build;\
	  if [ ! -d p3 ]; then mkdir p3; fi; cmake ..; make)

clean:
	rm -rf ./build
