var Events = require('../index.js');
var chai = require('chai');
chai.should();

describe("Basics", function () {
    it ("constructor works", function () {
        var events = new Events();
        events.should.exist;
    });

    it("registers simple listener", function (done) {
        var events = new Events();
        events.addListener("test", function (a, b) {
            a.should.equal(1);
            b.should.equal(2);
            done();
        });
        events.emit("test", 1, 2);
    });
});