var sqlite3 = require('../');
var ret;
ret = sqlite3.open("test.db",function(err,conn){
	if(err){
		console.log('err'+err);
	}
	else{
		console.log(conn);
		conn.query('select * from stu where name=?','郑华山',function(err,statment){
			if(err){console.log('err'+err); return;}
			console.log('statment'+statment);
			field_count = statment.fieldCount();
			console.log('field_count:'+ field_count);
			console.log('record_count:'+ statment.recordCount());
			
			var bret = statment.first();
			console.log(bret);
			console.log(statment.fieldName());
			while(bret){
				
				for(var i =0 ; i < field_count; ++i){
			console.log('value:'+ statment.fieldValue(0));
			console.log('value:'+ statment.fieldValue(1));
			console.log('value:'+ statment.fieldValue('ID'));
			console.log('value:'+ statment.fieldValue('name'));
			//console.log('value:'+ object2);
			//var object = statment.getFieldValueByName('ID');
			//console.log('value:'+ object);
				}
				bret = statment.next();
			
			}
			conn.close();
			
		});
		
		
	}
});





