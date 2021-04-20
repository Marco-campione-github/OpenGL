// 2D Indentity matrix transform
function m_identity2D() {
	var out = [[]];
	out[0]=[1,0,0];
	out[1]=[0,1,0];
	out[2]=[0,0,1];
	return out;
}

// 2D Traslation matrix transform
function m_translation2D(dx,dy) {
	var out = [[]];
	out[0]=[1, 0, dx];
	out[1]=[0, 1, dy];
	out[2]=[0, 0,  1];
	return out;
}

// 2D Rotation (around the origin) matrix transform
// Theta is in degree
function m_rotation2D(theta) {
	var out = [[]];
	theta  = (Math.PI*theta)/180;
	out[0] = [Math.cos(theta), -Math.sin(theta), 0];
	out[1] = [Math.sin(theta),  Math.cos(theta), 0];
	out[2] = [              0,                0, 1];
	return out;
}


