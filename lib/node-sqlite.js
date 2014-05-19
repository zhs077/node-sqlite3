var node_sqlite3 = require('../build/Release/node_sqlite3.node');
var sqlite3 = new node_sqlite3.Sqlite3Client();
var Connection = require('./connection');


//callback (err,conn)
exports.open = function(path,callback){
	sqlite3.open(path,function(err,conn_){
		if(err){
      	callback(err,null)
   	 }
	else{
		var conn = new Connection(conn_)
       	callback(null,conn);
	}
});
	
	
};

//同步方法
exports.openSync = function (path){
    var conn = sqlite3.openSync(path);
    if (conn == null){
        throw "cannot open sqlite3 please check path";
    }else{
        return new Connection(conn);
    }
}
