
#ifndef CORE_GRAPHICS_INTERSECTION_H_
#define CORE_GRAPHICS_INTERSECTION_H_

namespace ml {

namespace intersection {

	// sort so that a<=b 
	template<class FloatType>
	void SORT(FloatType& a, FloatType& b) {
		if (a > b) {
			std::swap(a,b);
		}
	}

	// this edge to edge test is based on Franlin Antonio's gem:
    //"Faster Line Segment Intersection", in Graphics Gems III,
    //pp. 199-202
	template<class FloatType> 
	bool EDGE_EDGE_TEST(
		const point3d<FloatType>& V0, 
		const point3d<FloatType>& U0, 
		const point3d<FloatType>& U1,
		unsigned short i0, unsigned short i1,
		point2d<FloatType>& a,
		point2d<FloatType>& b, 
		point2d<FloatType>& c,
		FloatType& d,
		FloatType& e,
		FloatType& f) 
	{

		b.x = U0[i0] - U1[i0];
		b.y = U0[i1] - U1[i1];
		c.x = V0[i0] - U0[i0];
		c.y = V0[i1] - U0[i1];

		f = a.y*b.x - a.x*b.y;
		d = b.y*c.x - b.x*c.y;

		if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f)) 	{                                                  
			e=a.x*c.y-a.y*c.x;                                   
			if (f>0)	{                                                
				if(e>=0 && e<=f) {return true;}            
			} else {                                                
				if(e<=0 && e>=f) {return true;}                       
			}                                                
		}
		return false;
	}
	

	template<class FloatType> 
	bool EDGE_AGAINST_TRI_EDGES(
		const point3d<FloatType>& V0,
		const point3d<FloatType>& V1,
		const point3d<FloatType>& U0,
		const point3d<FloatType>& U1,
		const point3d<FloatType>& U2,
		unsigned short i0, unsigned short i1)
	{
		point2d<FloatType> a, b, c;
		FloatType e,d,f;          
		a.x = V1[i0]-V0[i0];                           
		a.y = V1[i1]-V0[i1];                           
		//test edge U0,U1 against V0,V1         
		if (EDGE_EDGE_TEST(V0,U0,U1,i0,i1,a,b,c,d,e,f)) return true;                
		//test edge U1,U2 against V0,V1        
		if (EDGE_EDGE_TEST(V0,U1,U2,i0,i1,a,b,c,d,e,f)) return true;                 
		//test edge U2,U1 against V0,V1    
		if (EDGE_EDGE_TEST(V0,U2,U0,i0,i1,a,b,c,d,e,f)) return true;

		return false;
	}


	template<class FloatType> 
	bool POINT_IN_TRI(
		const point3d<FloatType>& V0,
		const point3d<FloatType>& U0,
		const point3d<FloatType>& U1,
		const point3d<FloatType>& U2,
		unsigned short i0, unsigned short i1)          
	{                                          
		FloatType a,b,c,d0,d1,d2;                    
		//is T1 completely inside T2?    
		//check if V0 is inside tri(U0,U1,U2)
		a=U1[i1]-U0[i1];                         
		b=-(U1[i0]-U0[i0]);                      
		c=-a*U0[i0]-b*U0[i1];                    
		d0=a*V0[i0]+b*V0[i1]+c;                  

		a=U2[i1]-U1[i1];                         
		b=-(U2[i0]-U1[i0]);                      
		c=-a*U1[i0]-b*U1[i1];                    
		d1=a*V0[i0]+b*V0[i1]+c;                  

		a=U0[i1]-U2[i1];                         
		b=-(U0[i0]-U2[i0]);                      
		c=-a*U2[i0]-b*U2[i1];                    
		d2=a*V0[i0]+b*V0[i1]+c;                  
		if (d0*d1 > (FloatType)0.0) {                                        
			if (d0*d2 > (FloatType)0.0) return true;               
		} 
		return false;
	}


	template<class FloatType>
	bool coplanar_tri_tri(
		const point3d<FloatType>& N,
		const point3d<FloatType>& V0,
		const point3d<FloatType>& V1,
		const point3d<FloatType>& V2,
		const point3d<FloatType>& U0,
		const point3d<FloatType>& U1,
		const point3d<FloatType>& U2)
	{
		point3d<FloatType> A;
		short i0,i1;
		// first project onto an axis-aligned plane, that maximizes the area 
		// of the triangles, compute indices: i0,i1. 
		A[0] = math::abs(N[0]);
		A[1] = math::abs(N[1]);
		A[2] = math::abs(N[2]);
		if(A[0]>A[1])	{
			if(A[0]>A[2])	{
				i0=1;      // A[0] is greatest
				i1=2;
			} else	{
				i0=0;      // A[2] is greatest
				i1=1;
			}
		} 
		else   // A[0]<=A[1] 
		{
			if(A[2]>A[1]) {
				i0=0;      // A[2] is greatest 
				i1=1;
			} else {
				i0=0;      // A[1] is greatest 
				i1=2;
			}
		}

		// test all edges of triangle 1 against the edges of triangle 2 
		if (EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2,i0,i1))	return true;
		if (EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2,i0,i1))	return true;
		if (EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2,i0,i1))	return true;

		// finally, test if tri1 is totally contained in tri2 or vice versa 
		if (POINT_IN_TRI(V0,U0,U1,U2,i0,i1)) return true;
		if (POINT_IN_TRI(U0,V0,V1,V2,i0,i1)) return true;

		return false;
	}


	//returns true if coplanar
	template<class FloatType>
	bool NEWCOMPUTE_INTERVALS(
		const point3d<FloatType>& v0,
		const point3d<FloatType>& v1,
		const point3d<FloatType>& v2,
		const point3d<FloatType>& u0,
		const point3d<FloatType>& u1,
		const point3d<FloatType>& u2,
		const point3d<FloatType>& n1,
		FloatType& VV0,FloatType& VV1,FloatType& VV2, 
		FloatType& D0, FloatType& D1, FloatType& D2, 
		FloatType& D0D1, FloatType& D0D2,
		FloatType& A, FloatType& B, FloatType& C, FloatType& X0, FloatType& X1) 
	{
		if(D0D1>(FloatType)0.0)
		{
			// here we know that D0D2<=0.0
			// that is D0, D1 are on the same side, D2 on the other or on the plane
			A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;
		}	
		else if(D0D2>(FloatType)0.0)
		{ 
			// here we know that d0d1<=0.0
			A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;
		}
		else if(D1*D2>(FloatType)0.0 || D0!=(FloatType)0.0)
		{
			// here we know that d0d1<=0.0 or that D0!=0.0
			A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; 
		} 
		else if(D1!=(FloatType)0.0)
		{
			A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;
		}
		else if(D2!=(FloatType)0.0)
		{
			A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;
		}
		else
		{
			//triangles are co-planar
			return true;
		}
		return false;
	}

	template<class FloatType>
	bool intersectTriangleTriangle(
		const point3d<FloatType> &v0,
		const point3d<FloatType> &v1,
		const point3d<FloatType> &v2,
		const point3d<FloatType> &u0,
		const point3d<FloatType> &u1,
		const point3d<FloatType> &u2) 
	{
		const bool USE_EPSILON_TEST = true;
		const FloatType EPSILON = (FloatType)0.000001;

		//compute plane equation of triangle(V0,V1,V2)
		point3d<FloatType> e1 = v1 - v0;
		point3d<FloatType> e2 = v2 - v0;
		point3d<FloatType> n1 = e1 ^ e2;
		FloatType d1 = - n1 | v0;
		//plane equation 1: N1.X+d1=0 */

		//put U0,U1,U2 into plane equation 1 to compute signed distances to the plane
		FloatType du0 = (n1 | u0)+d1;
		FloatType du1 = (n1 | u1)+d1;
		FloatType du2 = (n1 | u2)+d1;

		//co-planarity robustness check
//#if USE_EPSILON_TEST==TRUE
		if (USE_EPSILON_TEST) {
			if(math::abs(du0)<EPSILON) du0=(FloatType)0.0;
			if(math::abs(du1)<EPSILON) du1=(FloatType)0.0;
			if(math::abs(du2)<EPSILON) du2=(FloatType)0.0;
		}
//#endif
		FloatType du0du1 = du0*du1;
		FloatType du0du2 = du0*du2;

		if (du0du1 > (FloatType)0.0 && du0du2 > (FloatType)0.0) //same sign on all of them + not equal 0 ? 
			return false;										//no intersection occurs

		// compute plane of triangle (U0,U1,U2) 
		e1 = u1 - u0;
		e2 = u2 - u0;
		point3d<FloatType> n2 = e1 ^ e2;
		FloatType d2 = -(n2 | u0);
		// plane equation 2: N2.X+d2=0 

		// put V0,V1,V2 into plane equation 2 
		FloatType dv0 = (n2 | v0)+d2;
		FloatType dv1 = (n2 | v1)+d2;
		FloatType dv2 = (n2 | v2)+d2;

//#if USE_EPSILON_TEST==TRUE
		if (USE_EPSILON_TEST) {
			if(math::abs(dv0)<EPSILON) dv0 = (FloatType)0.0;
			if(math::abs(dv1)<EPSILON) dv1 = (FloatType)0.0;
			if(math::abs(dv2)<EPSILON) dv2 = (FloatType)0.0;
		}
//#endif

		FloatType dv0dv1=dv0*dv1;
		FloatType dv0dv2=dv0*dv2;

		if(dv0dv1>(FloatType)0.0 && dv0dv2>(FloatType)0.0) //same sign on all of them + not equal 0 ?
			return false;                    // no intersection occurs

		// compute direction of intersection line
		point3d<FloatType> D = (n1^n2);

		// compute and index to the largest component of D
		FloatType max = (FloatType)math::abs(D[0]);
		unsigned int index=0;
		FloatType bb=(FloatType)math::abs(D[1]);
		FloatType cc=(FloatType)math::abs(D[2]);
		if(bb>max) max=bb,index=1;
		if(cc>max) max=cc,index=2;

		// this is the simplified projection onto L
		FloatType vp0=v0[index];
		FloatType vp1=v1[index];
		FloatType vp2=v2[index];

		FloatType up0=u0[index];
		FloatType up1=u1[index];
		FloatType up2=u2[index];

		// compute interval for triangle 1
		FloatType a,b,c,x0,x1;
		if (NEWCOMPUTE_INTERVALS(u0,u1,u2,v0,v1,v2,n1,vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1)) {
			return (coplanar_tri_tri(n1,v0,v1,v2,u0,u1,u2));	//triangles are a co-planar
		}

		// compute interval for triangle 2 
		FloatType d,e,f,y0,y1;
		if (NEWCOMPUTE_INTERVALS(u0,u1,u2,v0,v1,v2,n1,up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1)) {
			return (coplanar_tri_tri(n1,v0,v1,v2,u0,u1,u2));	//triangles are a co-planar
		}

		FloatType xx,yy,xxyy,tmp;
		xx=x0*x1;
		yy=y0*y1;
		xxyy=xx*yy;

		point2d<FloatType> isect1, isect2;

		tmp=a*xxyy;
		isect1[0]=tmp+b*x1*yy;
		isect1[1]=tmp+c*x0*yy;

		tmp=d*xxyy;
		isect2[0]=tmp+e*xx*y1;
		isect2[1]=tmp+f*xx*y0;

		SORT(isect1[0],isect1[1]);
		SORT(isect2[0],isect2[1]);

		if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;
		return true;

	}

}	//namespace intersection

}	//namespace ml

#endif
