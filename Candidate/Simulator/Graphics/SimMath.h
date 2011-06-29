#ifndef VCZHSIM_SIMMATH
#define VCZHSIM_SIMMATH

#include <math.h>
#include "..\..\..\Library\Basic.h"

namespace simulator
{
	
/*********************************************************************************************************
Vector3
*********************************************************************************************************/

	struct Vector3
	{
		double x;
		double y;
		double z;

		Vector3(){}
		Vector3(double _x, double _y, double _z) :x(_x) ,y(_y), z(_z){}
	};
	
	__forceinline Vector3 operator-(const Vector3& a)
	{
		return Vector3(-a.x, -a.y, -a.z);
	}

	/*----------------------------------------------------------------------------------*/
	
	__forceinline Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x+b.x, a.y+b.y, a.z+b.z);
	}
	
	__forceinline Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x-b.x, a.y-b.y, a.z-b.z);
	}
	
	__forceinline Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x*b.x, a.y*b.y, a.z*b.z);
	}
	
	__forceinline Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x/b.x, a.y/b.y, a.z/b.z);
	}

	/*----------------------------------------------------------------------------------*/
	
	__forceinline Vector3& operator+=(Vector3& a, const Vector3& b)
	{
		a.x+=b.x;
		a.y+=b.y;
		a.z+=b.z;
		return a;
	}
	
	__forceinline Vector3& operator-=(Vector3& a, const Vector3& b)
	{
		a.x-=b.x;
		a.y-=b.y;
		a.z-=b.z;
		return a;
	}
	
	__forceinline Vector3& operator*=(Vector3& a, const Vector3& b)
	{
		a.x*=b.x;
		a.y*=b.y;
		a.z*=b.z;
		return a;
	}
	
	__forceinline Vector3& operator/=(Vector3& a, const Vector3& b)
	{
		a.x/=b.x;
		a.y/=b.y;
		a.z/=b.z;
		return a;
	}

	/*----------------------------------------------------------------------------------*/
	
	__forceinline double Dot(const Vector3& a, const Vector3& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	
	__forceinline Vector3 Scale(const Vector3& v, double s)
	{
		return Vector3(v.x*s, v.y*s, v.z*s);
	}
	
	__forceinline Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
			);
	}

	__forceinline double LengthSquare(const Vector3& v)
	{
		return v.x*v.x + v.y*v.y + v.z*v.z;
	}

	__forceinline double Length(const Vector3& v)
	{
		return sqrt(LengthSquare(v));
	}

	__forceinline Vector3 Normalize(const Vector3& v, double length)
	{
		return Scale(v, length/Length(v));
	}
	
/*********************************************************************************************************
Vectorf3
*********************************************************************************************************/

	struct Vectorf3
	{
		float x;
		float y;
		float z;

		Vector3 D()
		{
			return Vector3(x, y, z);
		}

		void D(const Vector3& v)
		{
			x=(float)v.x;
			y=(float)v.y;
			z=(float)v.z;
		}

		bool operator==(const Vectorf3& v)
		{
			return x==v.x && y==v.y && z==v.z;
		}

		bool operator!=(const Vectorf3& v)
		{
			return !(*this==v);
		}
	};

/*********************************************************************************************************
Ray3/Plane3
*********************************************************************************************************/

	struct Ray3
	{
		Vector3 position;
		Vector3 direction;

		Ray3(){}
		Ray3(const Vector3& _p, const Vector3& _d)
			:position(_p), direction(_d){}
	};

	struct Plane3
	{
		Vector3 position;
		Vector3 normal;

		Plane3(){}
		Plane3(const Vector3& _p, const Vector3& _n)
			:position(_p), normal(_n){}
	};

	__forceinline Vector3 Intersect(const Plane3& plane, const Ray3& ray)
	{
		double scale=-Dot(plane.normal, ray.position-plane.position)/Dot(plane.normal, ray.direction);
		return ray.position+Scale(ray.direction, scale);
	}
	
/*********************************************************************************************************
Matrix
*********************************************************************************************************/

	struct Matrix4
	{
		double v[4][4];
	};

	/*----------------------------------------------------------------------------------*/

	__forceinline Matrix4 operator*(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 m;
		m.v[0][0] = a.v[0][0]*b.v[0][0] + a.v[0][1]*b.v[1][0] + a.v[0][2]*b.v[2][0];
		m.v[0][1] = a.v[0][0]*b.v[0][1] + a.v[0][1]*b.v[1][1] + a.v[0][2]*b.v[2][1];
		m.v[0][2] = a.v[0][0]*b.v[0][2] + a.v[0][1]*b.v[1][2] + a.v[0][2]*b.v[2][2];
		m.v[0][3] = 0;
		
		m.v[1][0] = a.v[1][0]*b.v[0][0] + a.v[1][1]*b.v[1][0] + a.v[1][2]*b.v[2][0];
		m.v[1][1] = a.v[1][0]*b.v[0][1] + a.v[1][1]*b.v[1][1] + a.v[1][2]*b.v[2][1];
		m.v[1][2] = a.v[1][0]*b.v[0][2] + a.v[1][1]*b.v[1][2] + a.v[1][2]*b.v[2][2];
		m.v[1][3] = 0;

		m.v[2][0] = a.v[2][0]*b.v[0][0] + a.v[2][1]*b.v[1][0] + a.v[2][2]*b.v[2][0];
		m.v[2][1] = a.v[2][0]*b.v[0][1] + a.v[2][1]*b.v[1][1] + a.v[2][2]*b.v[2][1];
		m.v[2][2] = a.v[2][0]*b.v[0][2] + a.v[2][1]*b.v[1][2] + a.v[2][2]*b.v[2][2];
		m.v[2][3] = 0;

		m.v[3][0] = a.v[3][0]*b.v[0][0] + a.v[3][1]*b.v[1][0] + a.v[3][2]*b.v[2][0] + b.v[3][0];
		m.v[3][1] = a.v[3][0]*b.v[0][1] + a.v[3][1]*b.v[1][1] + a.v[3][2]*b.v[2][1] + b.v[3][1];
		m.v[3][2] = a.v[3][0]*b.v[0][2] + a.v[3][1]*b.v[1][2] + a.v[3][2]*b.v[2][2] + b.v[3][2];
		m.v[3][3] = 1;
		return m;
	}

	/*----------------------------------------------------------------------------------*/

	__forceinline Vector3 operator*(const Vector3& v, const Matrix4& m)
	{
		double rx=v.x*m.v[0][0] + v.y*m.v[1][0] + v.z*m.v[2][0] + m.v[3][0];
		double ry=v.x*m.v[0][1] + v.y*m.v[1][1] + v.z*m.v[2][1] + m.v[3][1];
		double rz=v.x*m.v[0][2] + v.y*m.v[1][2] + v.z*m.v[2][2] + m.v[3][2];
		return Vector3(rx, ry, rz);
	}

	/*----------------------------------------------------------------------------------*/

	__forceinline Ray3 operator*(const Ray3& r, const Matrix4& m)
	{
		Vector3 p=r.position*m;
		Vector3 q=(r.position+r.direction)*m;
		return Ray3(p, q-p);
	}

	__forceinline Plane3 TransformPlane(const Plane3& r, const Matrix4& m, const Matrix4& n)
	{
		return Plane3(r.position*m, r.normal*n);
	}

	/*----------------------------------------------------------------------------------*/

	__forceinline Matrix4& operator*=(Matrix4& a, const Matrix4& b)
	{
		a=a*b;
		return a;
	}

	__forceinline Vector3& operator*=(Vector3& v, const Matrix4& m)
	{
		v=v*m;
		return v;
	}

/*********************************************************************************************************
¸¨Öúº¯Êý
*********************************************************************************************************/

	__forceinline bool SolveX2(double a, double b, double c, double& x1, double& x2)
	{
		double d=b*b-4*a*c;
		if(d>=0)
		{
			d=sqrt(d);
			x1=(-b-d)/(2*a);
			x2=(-b+d)/(2*a);
			return true;
		}
		else
		{
			return false;
		}
	}

	__forceinline bool MinPositive(double x1, double x2, double& x)
	{
		if(x1>0)
			if(x2>0) x=x1<x2?x1:x2; else x=x1;
		else
			if(x2>0) x=x2; else return false;
		return true;
	}

	__forceinline Vector3 Reflect(const Vector3& i, const Vector3& n)
	{
		Vector3 m=Normalize(Cross(Cross(n, i), n), 1);
		double l=Length(i);
		double s=Dot(i, m)/l;
		double c=Dot(i, n)/l;
		return Scale(n, -c)+Scale(m, s);
	}

	__forceinline void ReflectAndRefract(const Vector3& i, const Vector3& n, double refractionN, Vector3& reflect, Vector3& refract, double& factor, bool& enableReflection, bool& enableRefraction)
	{
		if(enableReflection || enableRefraction)
		{
			Vector3 m=Normalize(Cross(Cross(n, i), n), 1);
			double l=Length(i);
			double s=Dot(i, m)/l;
			double c=Dot(i, n)/l;
			double sr=s/refractionN;
			double cr=(c<0?-1:1)*sqrt(1-sr*sr);
			if(sr>=1)
			{
				enableRefraction=false;
			}
			if(enableReflection && enableRefraction)
			{
				refract=Scale(n, cr)+Scale(m, sr);
				reflect=Scale(n, -c)+Scale(m, s);

				double f=asin(abs(s));
				double t=asin(abs(sr));
				double s1=sin(f-t);
				double s2=sin(f+t);
				double t1=tan(f-t);
				double t2=tan(f+t);
				factor=((s1*s1)/(s2*s2)+(t1*t1)/(t2*t2))/2;
				if(factor<0)
				{
					factor=0;
					enableReflection=false;
				}
				else if(factor>1)
				{
					factor=1;
					enableRefraction=false;
				}
			}
			else if(enableReflection)
			{
				reflect=Scale(n, -c)+Scale(m, s);
				factor=1;
			}
			else
			{
				refract=Scale(n, cr)+Scale(m, sr);
				factor=0;
			}
		}
	}

	__forceinline void GetAxis(const Vector3& i, Vector3& x, Vector3& y, Vector3& z)
	{
		z=i;
		{
			Vector3 _1(0, -z.z, z.y);
			Vector3 _2(z.z, 0, -z.x);
			Vector3 _3(-z.y, z.x, 0);
			double _l1=LengthSquare(_1);
			double _l2=LengthSquare(_2);
			double _l3=LengthSquare(_3);

			double _l=_l1;
			x=_1;
			if(_l2>_l)
			{
				_l=_l2;
				x=_2;
			}
			if(_l3>_l)
			{
				_l=_l3;
				x=_3;
			}
			x=Normalize(x, 1.0);
		}
		y=Cross(z, x);
	}

	__forceinline Matrix4 MatrixUnit()
	{
		Matrix4 m;

		m.v[0][0]=1;
		m.v[0][1]=0;
		m.v[0][2]=0;
		m.v[0][3]=0;

		m.v[1][0]=0;
		m.v[1][1]=1;
		m.v[1][2]=0;
		m.v[1][3]=0;

		m.v[2][0]=0;
		m.v[2][1]=0;
		m.v[2][2]=1;
		m.v[2][3]=0;

		m.v[3][0]=0;
		m.v[3][1]=0;
		m.v[3][2]=0;
		m.v[3][3]=1;

		return m;
	}

	__forceinline Matrix4 MatrixOffset(double x, double y, double z)
	{
		Matrix4 m=MatrixUnit();
		m.v[3][0]=x;
		m.v[3][1]=y;
		m.v[3][2]=z;
		return m;
	}

	__forceinline Matrix4 MatrixScale(double x, double y, double z)
	{
		Matrix4 m=MatrixUnit();
		m.v[0][0]=x;
		m.v[1][1]=y;
		m.v[2][2]=z;
		return m;
	}

	__forceinline Matrix4 MatrixRotateXY(double t)
	{
		Matrix4 m=MatrixUnit();
		double s=sin(t);
		double c=cos(t);
		m.v[0][0]=c;
		m.v[0][1]=s;
		m.v[1][0]=-s;
		m.v[1][1]=c;
		return m;
	}

	__forceinline Matrix4 MatrixRotateYZ(double t)
	{
		Matrix4 m=MatrixUnit();
		double s=sin(t);
		double c=cos(t);
		m.v[1][1]=c;
		m.v[1][2]=s;
		m.v[2][1]=-s;
		m.v[2][2]=c;
		return m;
	}

	__forceinline Matrix4 MatrixRotateZX(double t)
	{
		Matrix4 m=MatrixUnit();
		double s=sin(t);
		double c=cos(t);
		m.v[2][2]=c;
		m.v[2][0]=s;
		m.v[0][2]=-s;
		m.v[0][0]=c;
		return m;
	}

	__forceinline Matrix4 MatrixAxis(const Vector3& x, const Vector3& y, const Vector3& z)
	{
		Matrix4 m=MatrixUnit();

		m.v[0][0]=x.x;
		m.v[0][1]=x.y;
		m.v[0][2]=x.z;

		m.v[1][0]=y.x;
		m.v[1][1]=y.y;
		m.v[1][2]=y.z;

		m.v[2][0]=z.x;
		m.v[2][1]=z.y;
		m.v[2][2]=z.z;

		return m;
	}

	__forceinline Matrix4 MatrixInverse(const Matrix4& m)
	{
		Matrix4 r;

		double fDetInv = 1 / (m.v[0][0] * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]) - 
		m.v[0][1] * (m.v[1][0] * m.v[2][2] - m.v[1][2] * m.v[2][0]) +
		m.v[0][2] * (m.v[1][0] * m.v[2][1] - m.v[1][1] * m.v[2][0]));

		r.v[0][0] = fDetInv * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]);
		r.v[0][1] = -fDetInv * (m.v[0][1] * m.v[2][2] - m.v[0][2] * m.v[2][1]);
		r.v[0][2] = fDetInv * (m.v[0][1] * m.v[1][2] - m.v[0][2] * m.v[1][1]);
		r.v[0][3] = 0;

		r.v[1][0] = -fDetInv * (m.v[1][0] * m.v[2][2] - m.v[1][2] * m.v[2][0]);
		r.v[1][1] = fDetInv * (m.v[0][0] * m.v[2][2] - m.v[0][2] * m.v[2][0]);
		r.v[1][2] = -fDetInv * (m.v[0][0] * m.v[1][2] - m.v[0][2] * m.v[1][0]);
		r.v[1][3] = 0;

		r.v[2][0] = fDetInv * (m.v[1][0] * m.v[2][1] - m.v[1][1] * m.v[2][0]);
		r.v[2][1] = -fDetInv * (m.v[0][0] * m.v[2][1] - m.v[0][1] * m.v[2][0]);
		r.v[2][2] = fDetInv * (m.v[0][0] * m.v[1][1] - m.v[0][1] * m.v[1][0]);
		r.v[2][3] = 0;

		r.v[3][0] = -(m.v[3][0] * r.v[0][0] + m.v[3][1] * r.v[1][0] + m.v[3][2] * r.v[2][0]);
		r.v[3][1] = -(m.v[3][0] * r.v[0][1] + m.v[3][1] * r.v[1][1] + m.v[3][2] * r.v[2][1]);
		r.v[3][2] = -(m.v[3][0] * r.v[0][2] + m.v[3][1] * r.v[1][2] + m.v[3][2] * r.v[2][2]);
		r.v[3][3] = 1;

		return r;
	}

	__forceinline Matrix4 MatrixNormal(const Matrix4& m)
	{
		Matrix4 r;
		r.v[0][0]=m.v[0][0];
		r.v[0][1]=m.v[1][0];
		r.v[0][2]=m.v[2][0];
		r.v[0][3]=0;

		r.v[1][0]=m.v[0][1];
		r.v[1][1]=m.v[1][1];
		r.v[1][2]=m.v[2][1];
		r.v[1][3]=0;

		r.v[2][0]=m.v[0][2];
		r.v[2][1]=m.v[1][2];
		r.v[2][2]=m.v[2][2];
		r.v[2][3]=0;

		r.v[3][0]=0;
		r.v[3][1]=0;
		r.v[3][2]=0;
		r.v[3][3]=1;

		r=MatrixInverse(r);
		return r;
	}
}

#endif