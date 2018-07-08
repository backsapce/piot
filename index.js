const mqtt = require('mqtt')
const config = require('./config')

const serverAddr = config.server;
const dht22 = require('./sensors').dht22

let client  = mqtt.connect(serverAddr)
 
client.on('connect', function () {
    console.log('connected')
    begin()
})
client.on('error',()=>{
    clearInterval(task)
    setTimeout(()=>{
        console.log('try to reconnect')
        client  = mqtt.connect(serverAddr)
    },1000)
})

let task
function begin(){
    if(task) return 
    task = setInterval(()=>{
        let data =  dht22.data()
        console.log('dht22 data',data)
        client.publish('/iot/sensors/dht22',Buffer.from(JSON.stringify(data)))
    },1000)
}