// Draw scene's primitives
function draw_scene(sc) {
	for(var ss=0;ss<sc.length;++ss) {
		target = sc[ss];
		if (target.type=="point") {
			draw_point(target.p,target.color);
		}
		if (target.type=="line") {
			draw_line(target.p1, target.p2, target.color);
		}
		if (target.type=="circle") {
			draw_circle(target.c, target.r, target.color);
		}
	}
}
