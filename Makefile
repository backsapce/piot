all:dht22 epd wzs
	node index.js

dht22:
	cd sensors/dht22 && node-gyp configure && node-gyp build

epd: 
	cd sensors/epd && node-gyp configure && node-gyp build

wzs:
	cd sensors/dart-wz-s && node-gyp configure && node-gyp build	

run: 
	node index.js

run-back:
	nohup node index.js &
