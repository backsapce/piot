const mqtt = require('mqtt');
const config = require('./config');

const serverAddr = config.server;
const dht22 = require('./sensors').dht22;
const epd = require('./sensors').epd;
const wzs = require('./sensors').wzs;
const mem = require('./lib/mem');
let client  = mqtt.connect(serverAddr,{
    connectTimeout:5*1000
})
 
client.on('connect', function () {
    console.log('connected')
})
client.on('reconnect', function () {
    console.log('reconnected')
})
client.on('offline', function () {
    console.log('go offline')
})
client.on('error',(err)=>{
    setTimeout(()=>{
        console.log('try to reconnect',err)
        client  = mqtt.connect(serverAddr)
    },3000)
})
begin()
function begin(){
    setInterval(()=>{
        let c,h,gas = ''
        try{
            let data =  dht22.data()
            c = data[0].toFixed(2);
            h = data[1].toFixed(2);
        }catch(e){
            console.error(e)
        }
        try{
            gas = wzs.data() || '';
        }catch(e){
            console.error(e)
        }
        console.log('dht22 data: ',c,' h: ',h)
        console.log('wzs data: ',gas);
        let memUsage = mem();
        epd.paint(c,h,gas.toString());
        client.publish('/iot/sensors/dht22',Buffer.from(JSON.stringify({
            c,
            h,
            gas,
            memUsage
        })))
    },1000)
}