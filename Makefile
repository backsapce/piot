all:dht22
	node index.js

dht22:
	cd sensors/dht22 && node-gyp configure && node-gyp build

run: 
	node index.js
