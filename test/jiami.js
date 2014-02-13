var sqlite3 = require('../');
var ret;
ret = sqlite3.open("test.db",function(err,conn){
	if(err){
		console.log('err'+err);
	}
	else{
			//加密一个数据库
			//conn.changekey('zhs');
			//访问一个加密到数据库
			//conn.enterkey('zhs');
			//删除密码
			conn.enterkey('zhs');
			conn.delkey();
			conn.query("select * from stu where id=1",function(err,state){
	if(err) console.log(err);
})
			
		
	
	}
});
