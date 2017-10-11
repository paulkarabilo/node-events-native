var Benchmark = require('benchmark');
var NativeEvents = require('../index.js');
var NodeEvents = require('events');

var suite = new Benchmark.Suite;
var opts = {defer: true}
var counter = 0;

suite
    .add('NativeEvents#Call', function (def) {
            var nativeEvents = new NativeEvents();
            nativeEvents.once("TEST", function () { def.resolve(); });
            nativeEvents.emit("TEST");
        }, opts)
    .add('NodeEvents#Call', function (def) {
            var nodeEvents = new NodeEvents.EventEmitter();
            nodeEvents.once("TEST", function () { def.resolve(); });
            nodeEvents.emit("TEST");
        }, opts)
    .on('cycle',  (event) =>  console.log(String(event.target)))
    .on('complete', function () { 
        console.log('Fastest is ' + this.filter('fastest').map('name'));
        process.exit();
    }).run();