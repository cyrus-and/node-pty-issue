.PHONY: configure build clean

configure:
	node-gyp configure

build:
	node-gyp build

clean:
	$(RM) -r build/
