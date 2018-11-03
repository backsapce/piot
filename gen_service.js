const path = require('path')
const fs = require('fs')

const cur_dir = __dirname;

const service_desc = 'rpi service'

const entry_file = path.resolve(cur_dir,'./entry_file.sh')

const system_service_path = '/etc/systemd/system'

let service_file = `[Unit]\nDescription=${service_desc}\nAfter=network.target\n\n[Service]\nType=simple\nRestart=always\nExecStart=${entry_file}\n\n[Install]\nWantedBy=multi-user.target`

fs.writeFileSync(path.join(system_service_path,'./rpi.service'),service_file)