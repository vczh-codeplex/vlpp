#include "SimScene.h"

namespace simulator
{
	namespace renderers
	{
		namespace directtray
		{
			bool Intersect(const Ray3& ray, Scene* scene, Geometry* refractingGeometry, Plane3& plane, Vector3& diffuseNormal, Material& material, Geometry*& g)
			{
				if(refractingGeometry)
				{
					if(refractingGeometry->Intersect(ray*refractingGeometry->localMatrixInverse, plane, diffuseNormal, material))
					{
						g=refractingGeometry;
						plane=TransformPlane(plane, g->localMatrix, g->localMatrixNormal);
						diffuseNormal*=g->localMatrixNormal;
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					bool found=false;
					double nearestScale=0;
					double scale=0;
					for(int i=0;i<scene->geometries.Count();i++)
					{
						Plane3 _plane;
						Vector3 _diffuseNormal;
						Material _material;
						Geometry* _g=scene->geometries[i];
						if(_g->Intersect(ray*_g->localMatrixInverse, _plane, _diffuseNormal, _material))
						{
							_plane=TransformPlane(_plane, _g->localMatrix, _g->localMatrixNormal);
							double scale=LengthSquare(_plane.position-ray.position);
							if(!found || scale<nearestScale)
							{
								found=true;
								nearestScale=scale;

								plane=_plane;
								diffuseNormal=_diffuseNormal*_g->localMatrixNormal;
								material=_material;
								g=_g;
							}
						}
					}
					return found;
				}
			}

			bool Intersect(const Ray3& ray, Scene* scene, Geometry* refractingGeometry)
			{
				Plane3 plane;
				Vector3 diffuseNormal;
				Material material;
				Geometry* g=0;
				return Intersect(ray, scene, refractingGeometry, plane, diffuseNormal, material, g);
			}

			Color GetLight(const Ray3& acceptingRay, const Plane3& acceptingPanel, const Vector3& acceptingDiffuseNormal, Geometry* acceptingGeometry, Scene* scene, bool allowShadow)
			{
				Color color(0, 0, 0);
				for(int l=0;l<scene->parallelLights.Count();l++)
				{
					ParallelLight& light=scene->parallelLights[l];
					double cosT=-Dot(acceptingPanel.normal, light.direction);
					double cosD=-Dot(acceptingDiffuseNormal, light.direction);
					if(cosD>0)
					{
						Ray3 ray(acceptingPanel.position+Scale(-light.direction, 0.0001), -light.direction);
						if(!allowShadow || !Intersect(ray, scene, 0))
						{
							double blinnSpecularScale=0;
							if(acceptingGeometry->material.enableBlinnSpecular)
							{
								Vector3 N=acceptingDiffuseNormal;
								Vector3 E=Normalize(-acceptingRay.direction, 1.0);
								Vector3 L=Normalize(-light.direction, 1.0);
								Vector3 H=Normalize(L+E, 1.0);
								double NH=Dot(N, H);
								double NE=Dot(N, E);
								double NL=Dot(N, L);
								double EH=Dot(E, H);
								if(NE>0.0001 && EH>0.0001)
								{
									double D=1;
									double G=1;
									double F=1;
									{
										double c3_2=acceptingGeometry->material.blinnC3*acceptingGeometry->material.blinnC3;
										double cos2a=NH*NH;
										D=c3_2/(cos2a*(c3_2-1)+1);
										D=D*D;
									}
									{
										double Gb=2*NH*NE/EH;
										double Gc=2*NH*NL/EH;
										double Gmin=Gb<Gc?Gb:Gc;
										G=Gmin<1?Gmin:1;
									}
									{
										Vector3 reflectionDirection;
										Vector3 refractionDirection;
										bool enableReflection=acceptingGeometry->material.enableReflection;
										bool enableRefraction=acceptingGeometry->material.enableRefraction;
										ReflectAndRefract(-E, N, acceptingGeometry->material.N, reflectionDirection, refractionDirection, F, enableReflection, enableRefraction);
									}
									blinnSpecularScale=D*G*F/NE;
								}
							}
							color+=Scale(light.light, cosD+blinnSpecularScale);
						}
					}
				}
				return color;
			}

			bool GetDiffuse(const Ray3& ray, Scene* scene, bool allowShadow, bool allowAO, Geometry* refractingGeometry, Plane3& plane, Material& material, Geometry*& g, Color& color)
			{
				Vector3 diffuseNormal;
				if(Intersect(ray, scene, refractingGeometry, plane, diffuseNormal, material, g))
				{
					plane.normal=Normalize(plane.normal, 1.0);
					diffuseNormal=Normalize(diffuseNormal, 1.0);
					double environmentScale=1;

					if(allowAO && material.enableAmbientOcclusion && !refractingGeometry)
					{
						const double dmin=0.0001;
						Vector3 aoX, aoY, aoZ;
						GetAxis(plane.normal, aoX, aoY, aoZ);
						int ao=material.aoLevel;
						int hit=0;
						int total=0;
						for(int h=0;h<ao;h++)
						{
							double th=0.99*(PI*h/(2*(ao-1)));
							double sth=sin(th);
							double cth=cos(th);
							int vao=(int)(2*ao*sth)+1;
							total+=vao;

							for(int v=0;v<vao;v++)
							{
								double tv=2*PI*v/vao;
								double stv=sin(tv);
								double ctv=cos(tv);

								double aoXs=ctv*sth;
								double aoYs=stv*sth;
								double aoZs=cth;
								Vector3 aod=Scale(aoX, aoXs)+Scale(aoY, aoYs)+Scale(aoZ, aoZs);
								Ray3 aor(plane.position+Scale(aod, dmin), aod);

								if(Intersect(aor, scene, 0))
								{
									hit++;
								}
							}
						}
						environmentScale=1-(double)hit/total;
					}
					color=(Scale(scene->environmentLight, environmentScale)+GetLight(ray, plane, diffuseNormal, g, scene, allowShadow))*material.color;
					return true;
				}
				else
				{
					return false;
				}
			}

			Color GetSkyColor(const Ray3& ray, Scene* scene)
			{
				if(scene->skyGeometry)
				{
					Plane3 plane;
					Vector3 diffuseNormal;
					Material material;
					if(scene->skyGeometry->Intersect(
						Ray3(Vector3(0, 0, 0), ray.direction*scene->skyGeometry->localMatrixInverse),
						plane,
						diffuseNormal,
						material))
					{
						return material.color;
					}
				}
				return Color(0, 0, 0);
			}

			Color GetDiffuseWithoutReflection(const Ray3& ray, Scene* scene, bool allowShadow, bool allowAO)
			{
				Plane3 plane;
				Material material;
				Geometry* g;
				Color diffuseColor(0, 0, 0);
				if(GetDiffuse(ray, scene, allowShadow, allowAO, 0, plane, material, g, diffuseColor))
				{
					return diffuseColor;
				}
				else
				{
					return GetSkyColor(ray, scene);
				}
			}

			Color GetDiffuseWithReflection(const Ray3& ray, Scene* scene, int maxReflection, Geometry* refractingGeometry, bool allowShadow, bool allowAO, bool allowBRDF)
			{
				Plane3 plane;
				Material material;
				Geometry* g;
				Color diffuseColor(0, 0, 0);
				if(GetDiffuse(ray, scene, allowShadow, allowAO, refractingGeometry, plane, material, g, diffuseColor))
				{
					if((maxReflection>0 && (material.enableReflection || material.enableRefraction))
						|| (maxReflection==0 && material.enableRefraction && refractingGeometry))
					{
						diffuseColor=Scale(diffuseColor, 1-material.reflection);
						double factor=0;
						Vector3 reflectionDirection;
						Vector3 refractionDirection;
						bool enableReflection=material.enableReflection && maxReflection>0;
						bool enableRefraction=material.enableRefraction;
						ReflectAndRefract(
							ray.direction,
							plane.normal,
							(refractingGeometry?1/material.N:material.N),
							reflectionDirection,
							refractionDirection,
							factor,
							enableReflection,
							enableRefraction);

						if(enableReflection)
						{
							Color reflectionColor(0, 0, 0);
							if(!material.enableBRDFReflection || !allowBRDF)
							{
								Ray3 newRay;
								newRay.direction=reflectionDirection;
								newRay.position=plane.position+Scale(newRay.direction, 0.0001);
								reflectionColor=GetDiffuseWithReflection(newRay, scene, maxReflection-1, refractingGeometry, allowShadow, allowAO, false);
							}
							else
							{
								const double dmin=0.0001;
								Vector3 aoX, aoY, aoZ;
								GetAxis(Normalize(plane.normal, 1.0), aoX, aoY, aoZ);
								int ao=material.brLevel;

								int total=0;
								Color reflectionColorSum(0, 0, 0);
								for(int h=0;h<ao;h++)
								{
									double th=ao>1?0.99*(material.brAngle*h/(ao-1)):0;
									double sth=sin(th);
									double cth=cos(th);
									int vao=(int)(2*ao*sth/sin(material.brAngle))+1;
									total+=vao;

									for(int v=0;v<vao;v++)
									{
										double tv=2*PI*v/vao;
										double stv=sin(tv);
										double ctv=cos(tv);

										double aoXs=ctv*sth;
										double aoYs=stv*sth;
										double aoZs=cth;
										Vector3 aod=Scale(aoX, aoXs)+Scale(aoY, aoYs)+Scale(aoZ, aoZs);
										
										Ray3 newRay;
										newRay.direction=Reflect(ray.direction, aod);
										newRay.position=plane.position+Scale(newRay.direction, 0.0001);
										double reflectionScale=pow(Dot(aoZ, Normalize(aod, 1.0)), material.brAlpha);
										reflectionColorSum+=Scale(GetDiffuseWithReflection(newRay, scene, maxReflection-1, refractingGeometry, allowShadow, allowAO, false), reflectionScale);
									}
								}
								reflectionColor=Scale(reflectionColorSum, 1.0/total);
							}
							diffuseColor+=material.color*Scale(reflectionColor, material.reflection*factor);
						}
						if(enableRefraction)
						{
							Ray3 newRay;
							newRay.direction=refractionDirection;
							newRay.position=plane.position+Scale(newRay.direction, 0.0001);
							diffuseColor+=material.color*Scale(GetDiffuseWithReflection(newRay, scene, maxReflection-1, (refractingGeometry?0:g), allowShadow, allowAO, allowBRDF), material.reflection*(1-factor));
						}
					}
					return diffuseColor;
				}
				else
				{
					return GetSkyColor(ray, scene);
				}
			}
		}

		DirectTrayRenderer::DirectTrayRenderer(int _level)
			:level(_level)
		{
		}

		void DirectTrayRenderer::Render(Observer* observer, Scene* scene, vint bufferWidth, vint bufferHeight, vint screenWidth, vint screenHeight, int startY, int endY, Color* colors)
		{
			colors+=startY*bufferWidth;
			switch(level)
			{
			case 0:
			case 1:
				{
					bool allowShadow=(level==1);
					for(int y=startY;y<=endY;y++)
					{
						for(int x=0;x<bufferWidth;x++)
						{
							Ray3 ray=observer->CreateRay(x, y, bufferWidth, bufferHeight, screenWidth, screenHeight);
							*colors++=directtray::GetDiffuseWithoutReflection(ray, scene, allowShadow, false);
						}
					}
				}
				break;
			case 2:
				{
					for(int y=startY;y<=endY;y++)
					{
						for(int x=0;x<bufferWidth;x++)
						{
							Ray3 ray=observer->CreateRay(x, y, bufferWidth, bufferHeight, screenWidth, screenHeight);
							Color color=directtray::GetDiffuseWithReflection(ray, scene, 4, 0, true, false, false);
							*colors++=color;
						}
					}
				}
				break;
			case 3:
				{
					for(int y=startY;y<=endY;y++)
					{
						for(int x=0;x<bufferWidth;x++)
						{
							Ray3 ray=observer->CreateRay(x, y, bufferWidth, bufferHeight, screenWidth, screenHeight);
							Color color=directtray::GetDiffuseWithReflection(ray, scene, 4, 0, true, true, true);
							*colors++=color;
						}
					}
				}
				break;
			}
		}
	}
}