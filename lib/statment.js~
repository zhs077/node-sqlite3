
function Statment(statment){
	this.statment_ = statment;
}

module.exports = Statment;
Statment.prototype.first = function(){
	return this.statment_.moveFirst();
}

Statment.prototype.next = function(){
	return this.statment_.moveNext();
}
Statment.prototype.last = function(){
	return this.statment_.moveLast();
}

Statment.prototype.fieldCount = function(){
	return this.statment_.getFieldCount();
}
Statment.prototype.recordCount = function(){
	return this.statment_.getRecordCount();
}
Statment.prototype.fieldValue = function(arg){
	if (typeof(arg) === "number")
		return this.statment_.getFieldValueByIndex(arg);
	else if ( typeof (arg) === "string")
		return this.statment_.getFieldValueByName(arg);
	else
		return null;

}
Statment.prototype.fieldName = function(index){
	return this.statment_.getFieldName(index);
}


