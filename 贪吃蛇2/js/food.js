var colorlast = "";
function Food(gameSnake,flag,countFood){
    var self = this;
    //食物的位置
    //下面的do-while循环语句作用是先创建一个row和col，然后判断这个row和col是否在蛇的身上
    do{
		if(flag=="0"){
			this.row = parseInt(Math.random() * gameSnake.row);
			this.col = parseInt(Math.random() * gameSnake.col);
			colorlast = this.getColor();
		}else{
			var n = snakeHistory[countFood].split("|");
			this.row = n[0];
			this.col = n[1];
			colorlast = n[2]
		}
    }while((function(){
        //遍历蛇的row和col，然后和Food新随机出来的row和col来进行判断，是否重合
        for(var i = 0; i < gameSnake.snake.body.length; i++){
            if(gameSnake.snake.body[i].row == self.row || gameSnake.snake.body[i].col == self.col){
                return true;
            }
        }
        return false;
    })());
	if(flag=="0"){
		snakeHistory[snakeHistory.length] = this.row+"|"+this.col+"|"+colorlast;
	}
    console.log(this.row, this.col);
}


Food.prototype.render = function() {
	game.setHtml(this.row, this.col, "♥");
	document.getElementsByTagName("tr")[this.row].getElementsByTagName("td")[this.col].className = colorlast;
	//这个本来是要随机红绿蓝的
	/* 绿rgb(85, 255, 0); */
	/* 红rgb(255, 145, 0); */
	/* 蓝rgb(0, 251, 255) */
}

Food.prototype.getColor = function() {
	var x= parseInt(3*Math.random())+1;
	switch(x)
	{
		case 1: 
			colorlast= "xcolor1";
			break;
		case 2: 
			colorlast="xcolor2";
			break;
		case 3: 
			colorlast="xcolor3";
			break;
	}
	return colorlast;
}