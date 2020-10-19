const pty = require('./build/Release/pty');
const fs = require('fs');

const fd = pty.spawn();

const read_stream = fs.createReadStream(null, {fd});
const write_stream = fs.createWriteStream(null, {fd});

process.stdin.setRawMode(true);

read_stream.pipe(process.stdout);
process.stdin.pipe(write_stream);
