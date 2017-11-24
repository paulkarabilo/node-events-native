var Events = require('../index.js');
var chai = require('chai');
var sinon = require('sinon')
var sinonChai = require('sinon-chai');
chai.should();
chai.use(sinonChai);

describe("Basic Adding Listeners", function () {
    it("registers simple listener", function (done) {
        var events = new Events();
        events.addListener("test", function (a, b) {
            a.should.equal(1);
            b.should.equal(2);
            done();
        });
        events.emit("test", 1, 2);
    });

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
});

