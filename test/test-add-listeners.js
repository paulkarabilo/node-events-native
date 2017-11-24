var Events = require('../index.js');
var chai = require('chai');
var sinon = require('sinon')
var sinonChai = require('sinon-chai');
chai.should();
chai.use(sinonChai);

describe("Basic Adding Listeners", function () {
    it("should trigger newListener once added", function() {
        var newListener = sinon.spy()
        var listener = function() { return; };
        var events = new Events();
        events.on("newListener", newListener);
        events.addListener("test", listener)
        newListener.should.have.been.calledWith("test", listener);
    });

    it("should trigger newListener once added/async", function(done) {
        var listener = function() { return; };
        var events = new Events();
        events.on("newListener", function(event, fn) {
            if (event === 'newListener') return;
            fn.should.equal(listener);
            event.should.equal("test");
            done();
        });
        events.addListener("test", listener)
    });

    it("should trigger simple listener", function () {
        var listener = sinon.spy();
        var events = new Events();
        events.addListener("test", listener)
        events.emit("test", "a", "b");
        listener.should.have.been.calledWith("a", "b");
    });

    it("should trigger simple listener/asyn", function (done) {
        var listener = function(a, b) {
            a.should.equal("a");
            b.should.equal("b");
            done();
        };
        var events = new Events();
        events.addListener("test", listener)
        events.emit("test", "a", "b");
    });

    it("should call newListener before appending it to own list of listeners", function () {
        var listen1 = function () {};
        var listen2 = function () {};
        var events = new Events();
      
        events.once('newListener', function() {
          console.log("OUT");
          events.listeners('hello').should.deep.equal([]);
          events.once('newListener', function() {
            console.log("IN");
            events.listeners('hello').should.deep.equal([]);
            console.log("IN AFTER");
          });
          console.log("OUT AFTER");
          events.on('hello', listen2);
          console.log("OUT AFTER after");
        });
        console.log("BEFORE");
        events.on('hello', listen1);
        console.log('BEFORE AFTER')
        // The order of listeners on an event is not always the order in which the
        // listeners were added.
        events.listeners('hello').should.deep.equal([listen2, listen1]);
    });
});

