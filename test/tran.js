var sqlite3 = require('../');
var ret;
ret = sqlite3.open("test.db",function(err,conn){
	if(err){
		console.log('err'+err);
	}
	else{
			
			console.log(conn.beginTran()); 
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
			conn.executesync("insert into stu(id,name) values(?,?)",1,'郑华山');
		
			console.log(conn.commitTran()); //异步不好支持事务
			conn.close();
			console.log(conn.isopen());
	
	}
});
