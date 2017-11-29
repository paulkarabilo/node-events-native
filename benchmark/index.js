var Benchmark = require('benchmark');
var NativeEvents = require('../index.js');
var NodeEvents = require('events');

var suite = new Benchmark.Suite;
var opts = {defer: true}
var counter = 0;
var nativeEvents = new NativeEvents();
var nodeEvents = new NodeEvents.EventEmitter();
suite
    .add('NativeEvents#Call', function (def) {
            counter++;
            nativeEvents.once("TEST"+counter, function () { def.resolve(); });
            nativeEvents.emit("TEST"+counter);
        }, opts)
    .add('NodeEvents#Call', function (def) {
            counter++;
            nodeEvents.once("TEST"+counter, function () { def.resolve(); });
            nodeEvents.emit("TEST"+counter);
        }, opts)
    .on('cycle',  (event) =>  console.log(String(event.target)))
    .on('complete', function () { 
        console.log('Fastest is ' + this.filter('fastest').map('name'));
        process.exit();
    }).run();