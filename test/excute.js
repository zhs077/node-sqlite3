var sqlite3 = require('../');

var ret;
ret = sqlite3.open("test.db",function(err,conn){
	if(err){
		console.log('err'+err);
	}
	else{
		console.log(conn);
		
		
		//conn.begin(function(err){
		//	if(err)return;
		conn.executesync("create table stu(id text,name text)");
			
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){console.log(err)});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
			conn.execute("insert into stu(id,name) values(?,?)",1,'郑华山',function(err){});
		


	}
});




