//-------------------------------------------------------
// Utility functions
//-------------------------------------------------------

// Color coding
function rgb(r, g, b){
	return "rgb("+r+","+g+","+b+")";
}

// Floats -> Integer coordinates
function pround(p) {
	var out=[];
	for(var i=0;i<p.length;++i)
		out[i]=Math.round(p[i]);
	return out;
}

// Draw an arrow given two points (p1->p2)
function draw_arrow_p(p1,p2,color) {
	color=(color)?color:rgb(0,0,0);
	_draw_arrow(pround(p1),pround(p2),color);
}

// Draw an arrow given a point and a vector (p1->p1+v)
function draw_arrow_v(p1,v,color) {
	color=(color)?color:rgb(0,0,0);
	var p2 = summpv(p1,v);	
	draw_arrow_p(p1,p2,color);
}

//-------------------------------------------------------
// Drawing functions
//-------------------------------------------------------

// Draw a single point 
function draw_point(p,color) {
	color=(color)?color:rgb(0,0,0);
	_draw_disc(p,color);
}

function draw_line(p1,p2,color){
	var x0 = p1[0];
	var y0 = p1[1];
	var x1 = p2[0];
	var y1 = p2[1];

	var dx = x1 - x0;
	var dy = y1 - y0;

	var inc_x = (dx >= 0) ? +1 : -1;
	var inc_y = (dy >= 0) ? +1 : -1;

	dx = Math.abs(dx);
	dy = Math.abs(dy);

	if (dx >= dy) {
		var d = 2*dy - dx
		var delta_A = 2*dy
		var delta_B = 2*dy - 2*dx

		var x = 0;
		var y = 0;
		for (i=0; i <= dx; i++) {
			_draw_disc(point2D(x+x0,y+y0),color);
			if (d > 0) {
				d += delta_B;
				x += inc_x;
				y += inc_y;
			}
			else {
				d += delta_A;
				x += inc_x;
			}
		}
	}
	else {
		var d = 2*dx - dy
		var delta_A = 2*dx
		var delta_B = 2*dx - 2*dy

		var x = 0;
		var y = 0;
		for (i=0; i<=dy; i++) {
			_draw_disc(point2D(x+x0,y+y0),color);
			if (d > 0) {
				d += delta_B;
				x += inc_x;
				y += inc_y;
			}
			else {
				d += delta_A;
				y += inc_y;
			}
		}
	}
}

function _draw_circle_helper(xc,yc,x,y,color) {
	_draw_disc(point2D(xc+x, yc+y), color);
    _draw_disc(point2D(xc-x, yc+y), color);
    _draw_disc(point2D(xc+x, yc-y), color);
    _draw_disc(point2D(xc-x, yc-y), color);
    _draw_disc(point2D(xc+y, yc+x), color);
    _draw_disc(point2D(xc-y, yc+x), color);
    _draw_disc(point2D(xc+y, yc-x), color);
    _draw_disc(point2D(xc-y, yc-x), color);
}

function draw_circle(c, r, color) {
	var x = 0;
	var y = r;
    var d = 3 - 2 * r;
	
	_draw_circle_helper(c[0], c[1], x, y, color);
	while (y >= x) {
		x++;
        if (d > 0) {
            y--; 
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        _draw_circle_helper(c[0], c[1], x, y, color);		
	}
}

