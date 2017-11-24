var Events = require('../index.js');
var chai = require('chai');
chai.should();

describe("Basics", function () {
    it ("constructor works", function () {
        var events = new Events();
        events.should.exist;
    });
});