var _canvas = new Object();
var _M;

function _draw_arrow(from, to, sstyle, sw){
	from  = prodmv(_M,from);
	to    = prodmv(_M,to);
	
	var delta = diff(to,from);
	var headlen = 10;
	var angle = Math.atan2(delta[1],delta[0]);
	
	_canvas.context.beginPath();
	_canvas.context.lineWidth = (sw)?sw:1;
	_canvas.context.strokeStyle = sstyle;
	_canvas.context.moveTo(from[0], from[1]);
	_canvas.context.lineTo(to[0], to[1]);
	_canvas.context.moveTo(to[0]-headlen*Math.cos(angle-Math.PI/6),to[1]-headlen*Math.sin(angle-Math.PI/6));
	_canvas.context.lineTo(to[0],to[1]);
	_canvas.context.moveTo(to[0]-headlen*Math.cos(angle+Math.PI/6),to[1]-headlen*Math.sin(angle+Math.PI/6));
	_canvas.context.lineTo(to[0],to[1]);
	_canvas.context.stroke();
}

function _draw_disc(p,sstyle) {
	p = prodmv(_M,p);
	
	_canvas.context.beginPath();
	_canvas.context.fillStyle= sstyle;
	_canvas.context.arc(p[0],p[1],_canvas.grid/2-1,0,2 *Math.PI,false);
	_canvas.context.fill();
}

function _draw_grid() {
	_canvas.context.beginPath();
	_canvas.context.strokeStyle = rgb(220,220,220); 
	for (var y=0; y <= _canvas.h; y+=_canvas.grid) {
		_canvas.context.moveTo(0,y);
		_canvas.context.lineTo(_canvas.w,y);
	}
	for (var x=0; x <= _canvas.w; x+=_canvas.grid) {
		_canvas.context.moveTo(x,0);
		_canvas.context.lineTo(x,_canvas.h);
	}
	_canvas.context.stroke();
}		

function _display_matrix() {
	var out=[[]];
	
	out[0]=[_canvas.grid, 0,          _canvas.grid/2];
	out[1]=[0,-_canvas.grid,_canvas.h-_canvas.grid/2];
	out[2]=[0,            0,                       1];
	
	return out;
}

function initialize() {
	_canvas.ref     = document.getElementById("canvas");
	_canvas.w       = parseInt(_canvas.ref.getAttribute("width"));
	_canvas.h       = parseInt(_canvas.ref.getAttribute("height"));
	_canvas.context = _canvas.ref.getContext("2d");
	_canvas.grid    = 10;

	_M = _display_matrix();

	_draw_grid(_canvas.w,_canvas.h,_canvas.grid);
	_draw_arrow(point2D(0,0),point2D(_canvas.w/_canvas.grid-1,0),rgb(0,0,255),2);
	_draw_arrow(point2D(0,0),point2D(0,_canvas.h/_canvas.grid-1),rgb(0,0,255),2);	
}

function deepCopy (arr) {
	var out = [];
	for (var i = 0, len = arr.length; i < len; i++) {
		var item = arr[i];
		var obj = {};
		for (var k in item) {
			obj[k] = item[k];
		}
		out[i]=obj;
	}
	return out;
}