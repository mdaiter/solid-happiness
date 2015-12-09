var restify = require('restify');
var hashmap = require('./models/hashmap_words.js');

function sentenceRespond(req, res, next) {
  	hashmap.store(req.params.sentence);
  	res.send('Recorded: ' + req.params.sentence);
  	next();
}

function statsRespond(req, res, next) {
	res.contentType = 'json';
	res.send(hashmap.retrieve(req.query.N != undefined ? req.query.N : 5));
  	next();
}

var serverSentences = restify.createServer();

serverSentences.get('/:sentence', sentenceRespond);
serverSentences.head('/:sentence', sentenceRespond);

var serverQuery = restify.createServer();
serverQuery.use(restify.queryParser());

serverQuery.get('/stats', statsRespond);
serverQuery.head('/stats', statsRespond);

serverSentences.listen(5555, function() {
  	console.log('%s listening at %s', serverSentences.name, serverSentences.url);
});

serverQuery.listen(8080, function() {
  	console.log('%s listening at %s', serverQuery.name, serverQuery.url);
});
