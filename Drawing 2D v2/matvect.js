function assert(condition, message) {
	if (!condition) {
		throw(message);
	}
}

// Generic n-dimensional vector
function vector(n) {
	var out = new Array(n);
	for(var i=0;i<n;++i)
		out[i]=0;
	return out;
}

// Generic RxC matrix
function matrix(Rows,Cols) {
	var out = new Array(Rows);
	for(var i=0;i<Rows;++i) {
		out[i]=new Array(Cols);
		for(var j=0;j<Cols;++j) out[i][j]=0;
	}
	return out;
}

// 2-Dimensional Point (Cartesian->Homogenoues Coordinates)
function point2D(x,y) {
	var out = vector(3);
	out[0]=(x)?x:0;
	out[1]=(y)?y:0;
	out[2]=1;
	return out;
}

// 2-Dimensional Vector (Cartesian->Homogenoues Coordinates)
function vect2D(a,b) {
	out = vector(3);
	out[0]=(a)?a:0;
	out[1]=(b)?b:0;
	return out;
}

// 3x3 Matrix
function matrix3x3() {
	return matrix(3,3);
}

// Product Matrix x Vector
function prodmv(m,pv) {
	assert(m[0].length==pv.length,"prodmv(m,pv): m[0].length!=pv.length");
	
	var out = vector(m.length);	
	for(var i=0;i<m.length;++i) {
		out[i]=0;
		for(var j=0;j<pv.length;++j) {
			out[i]+=m[i][j]*pv[j];
		}
	}	
	return out;
}

// Product Matrix x Matrix
function prodmm(m1,m2) {
	assert(m1[0].length==m2.length,"prodmv(m,pv): m1[0].length!=m2.length");

	var out = matrix(m1.length,m2[0].length);
	for(var i=0;i<m1.length;++i) {
		for(var j=0;j<m2[0].length;++j) {
			out[i][j]=0;
			for(var k=0;k<m2.length;++k) {
				out[i][j]+=m1[i][k]*m2[k][j];
			}
		}
	}
	return out;
}

// Vector-Vector or Point-Point
function diff(pv1,pv2) {
	assert(pv1.length==pv2.length,"diff(pv1,pv2): pv1.length!=pv2.length");
	assert((pv1[pv1.length-1]>=pv2[pv2.length-1]),"diff(pv1,pv2): pv1 and pv2 are of different types");
	var out = vector(pv1.length);
	for(var i=0;i<pv1.length;++i)
		out[i]=pv1[i]-pv2[i]
	return out;
}

// Vector*Scalar
function prodvs(pv,scalar) {
	var out = vector(pv.length);
	for(var i=0;i<pv.length;++i)
		out[i]=pv[i]*scalar;
	return out;
}

// Vector+Vector
function summvv(v1,v2) {
	assert(v1.length==v2.length,"summvv(v1,v2): v1.length!=v2.length");
	var out = vector(v1.length);
	for(var i=0;i<v1.length;++i)
		out[i]=v1[i]+v2[i];
	return out;
}

// Point+Vector
function summpv(p,v) {
	assert(p.length==v.length,"summpv(p,v): p.length!=v.length");
	assert(p[p.length-1]==1,"summpv(p,v): p is not a point");
	assert(v[v.length-1]==0,"summpv(p,v): v is not a vector");
	
	var out = vector(v.length);
	for(var i=0;i<v.length;++i)
		out[i]=v[i]+p[i];
	return out;
}
