/*
 * hashmap_words.js
 * Author: Matthew Daiter
 *
 * This class is meant to store and retrive words sent by users and parsed by our JS server. It gives frequency of words and characters
 * */
var addon = require("../build/Release/addon.node");

module.exports = {
	attributes : {
		priority_map : new addon.PriorityMap()
	},

	store: function(string) {
		string.toLowerCase().split(" ").map(key => module.exports.attributes.priority_map.addToKey(key));
	},
	/*
	 * TODO: ask about formatting of JSON in display. Don't want to make assumpions about printing 'incorrect' N to screen
	 * */
	retrieve: function(N) {
		var JSON = {};
		JSON["count"] = module.exports.attributes.priority_map.count();
		JSON["top_" + N + "_words"] = module.exports.attributes.priority_map.topNWords(N);
		JSON["top_" + N + "_letters"] = module.exports.attributes.priority_map.topNChars(N);
		return JSON;
	}

};
