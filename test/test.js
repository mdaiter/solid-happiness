var request = require('supertest');  
var fs = require('fs');
var wordListPath = require('word-list');
var should = require('chai').should();

describe('Routing', function() {
  var url_sentences = 'http://localhost:5555';
  var url_stats = 'http://localhost:8080';
  
  describe('Sentence insertion', function() {
  	var words;

	before( function () {
	  words = fs.readFileSync(wordListPath, 'utf8').split('\n');
	});

	it('should post a sigle word and receive a 200', function(done) {
	  for (var i = 0; i < 500; i++){
		request(url_sentences).get("/" + words[i]).expect(200).end (function (err, res) { if (err) throw err; } );
	  }
	  done();
	});

    it('should post a setence to the server and receive a 200', function(done) {
      for (var i = 0; i < 500; i++){
        var sentence_words = "";
        for (var j = i; j < i + 4; j++){
	      sentence_words += words[j] + " ";
	    }
	    sentence_words = encodeURIComponent(sentence_words.trim());
	    request(url_sentences).get("/" + sentence_words).expect(200).end (function (err, res) { if (err) throw err; } );
	  }
	  done();
	});
  });
  describe ('Stats retrieval', function() {
    it ('should be able to retrieve count accurately', function(done) {
	  var init;
	  request(url_stats).get("/stats").set('Accept', 'application/json').expect(200).expect('Content-Type', /json/).end( function(err, res) {
	  if (err) return done(err);
	  	res.body.should.have.property('count');
	  	init = res.body.count;
	  });
	  request(url_sentences).get("/test").expect(200).end( function (err, res) { if (err) throw err; } );
	  request(url_stats).get("/stats").set('Accept', 'application/json').expect(200).expect('Content-Type', /json/).end( function(err, res) {
	  	if (err) return done(err);
	  	res.body.should.have.property('count');
	  	res.body.count.should.equal(init++);
	  	done();
	  });
	});
  });
});
