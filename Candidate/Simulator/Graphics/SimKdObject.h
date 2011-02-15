#ifndef VCZHSIM_SIMKDOBJECT
#define VCZHSIM_SIMKDOBJECT

#include "SimScene.h"
#include "SimKdTree.h"
#include "..\..\..\Library\Collections\OperationCopyFrom.h"

namespace simulator
{
	namespace geometries
	{
#define KD_CALLBACK ((Base*)this)

		template<typename T, typename Base>
		class KdObject : public Geometry
		{
		protected:
			struct KdNode
			{
				Array<int>						objects;
			};

			friend class collections::ReadonlyListEnumerator<Ptr<KdNode>>;

			struct KdContainer
			{
				KdNode*							nodes;
				int								usedNodeCount;
				KdTreeManager<KdNode>			manager;
				KdTree<KdNode>*					root;
				Vector3							min;
				Vector3							max;

				KdContainer(int count)
					:manager(count)
					,root(0)
					,nodes(new KdNode[count])
					,usedNodeCount(0)
				{
				}

				~KdContainer()
				{
					delete[] nodes;
				}
			};

#define BUILD_SUB_NODE(DIM, NEXTDIM, D, MIN_P, MAX_P)\
		case KdTree<KdNode>::DIM:\
			{\
				if(max.D-min.D<=blockSize)\
				{\
					return BuildKdTreeInternal(KdTree<KdNode>::NEXTDIM, indices, min, max, blockSize, blockMaxObjects);\
				}\
				else\
				{\
					List<int> smallIndices;\
					List<int> largeIndices;\
					float plane=(float)(min.D+max.D)/2;\
					for(int i=0;i<indices.Count();i++)\
					{\
						T& t=KD_CALLBACK->CallbackGetObject(indices[i]);\
						Vectorf3* points=KD_CALLBACK->CallbackGetObjectPoints(t);\
						bool addToSmall=false;\
						bool addToLarge=false;\
						for(int j=KD_CALLBACK->CallbackGetObjectPointCount(t)-1;j>=0;j--)\
						{\
							if(points[j].D<=plane)addToSmall=true;\
							if(points[j].D>=plane)addToLarge=true;\
						}\
						if(addToSmall)smallIndices.Add(indices[i]);\
						if(addToLarge)largeIndices.Add(indices[i]);\
					}\
					KdTree<KdNode>* node=kd->manager.Alloc();\
					node->dim=dim;\
					node->plane=plane;\
					node->left=BuildKdTreeInternal(KdTree<KdNode>::NEXTDIM, smallIndices, min, MAX_P, blockSize, blockMaxObjects);\
					node->right=BuildKdTreeInternal(KdTree<KdNode>::NEXTDIM, largeIndices, MIN_P, max, blockSize, blockMaxObjects);\
					return node;\
				}\
			}\
			break;

			KdTree<KdNode>* BuildKdTreeInternal(typename KdTree<KdNode>::Dim dim, List<int>& indices, const Vector3& min, const Vector3& max, double blockSize, int blockMaxObjects)
			{
				if(indices.Count()<=blockMaxObjects || ((max.x-min.x<=blockSize) && (max.y-min.y<=blockSize) && (max.z-min.z<=blockSize)))
				{
					KdTree<KdNode>* node=kd->manager.Alloc();
					node->dim=KdTree<KdNode>::Leaf;
					node->value=&kd->nodes[++kd->usedNodeCount];
					node->value->objects.Resize(indices.Count());
					CopyFrom(node->value->objects.Wrap(), indices.Wrap());
					return node;
				}
				else
				{
					switch(dim)
					{
					BUILD_SUB_NODE(X, Y, x, Vector3(plane, min.y, min.z), Vector3(plane, max.y, max.z))
					BUILD_SUB_NODE(Y, Z, y, Vector3(min.x, plane, min.z), Vector3(max.x, plane, max.z))
					BUILD_SUB_NODE(Z, X, z, Vector3(min.x, min.y, plane), Vector3(max.x, max.y, plane))
					}
					return 0;
				}
			}

#define TRAVERSE_SUB_NODE(DIM, D)\
		case KdTree<KdNode>::DIM:\
			{\
				if(front.D<=node->plane && back.D<=node->plane)\
				{\
					KdIntersect(d, scale, node->left, front, back, ray, plane, diffuseNormal, planeMaterial);\
				}\
				else if(front.D>=node->plane && back.D>=node->plane)\
				{\
					KdIntersect(d, scale, node->right, front, back, ray, plane, diffuseNormal, planeMaterial);\
				}\
				else\
				{\
					double midScale=(node->plane-ray.position.D)/ray.direction.D;\
					Vector3 mid=ray.position+Scale(ray.direction, midScale);\
					KdTree<KdNode>* frontNode=0;\
					KdTree<KdNode>* backNode=0;\
					if(front.D<node->plane)\
					{\
						frontNode=node->left;\
						backNode=node->right;\
					}\
					else\
					{\
						frontNode=node->right;\
						backNode=node->left;\
					}\
					if(midScale>0)\
					{\
						KdIntersect(d, scale, frontNode, front, mid, ray, plane, diffuseNormal, planeMaterial);\
					}\
					KdIntersect(d, scale, backNode, mid, back, ray, plane, diffuseNormal, planeMaterial);\
				}\
			}\
			break;

			void KdIntersect(const Vector3& d, double& scale, KdTree<KdNode>* node, const Vector3& front, const Vector3& back, const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
			{
				if(scale<0)
				{
					switch(node->dim)
					{
					case KdTree<KdNode>::Leaf:
						{
							for(int i=0;i<node->value->objects.Count();i++)
							{
								T& t=KD_CALLBACK->CallbackGetObject(node->value->objects[i]);
								KD_CALLBACK->CallbackContentIntersect(d, scale, t, ray, plane, diffuseNormal, planeMaterial);
							}
						}
						break;
					TRAVERSE_SUB_NODE(X, x)
					TRAVERSE_SUB_NODE(Y, y)
					TRAVERSE_SUB_NODE(Z, z)
					}
				}
			}
			
			void UpdateBox(Vectorf3& min, Vectorf3& max, const Vectorf3& p)
			{
				min.x=(p.x<min.x?p.x:min.x);
				min.y=(p.y<min.y?p.y:min.y);
				min.z=(p.z<min.z?p.z:min.z);
			
				max.x=(p.x>max.x?p.x:max.x);
				max.y=(p.y>max.y?p.y:max.y);
				max.z=(p.z>max.z?p.z:max.z);
			}

			void UpdateBoxScale(double& frontScale, bool& frontEnabled, double& backScale, bool& backEnabled, double scale, const Vector3& min, const Vector3& max, const Ray3& ray)
			{
				const double dmin=0.00001;
				Vector3 p=ray.position+Scale(ray.direction, scale);
				if(
					min.x-dmin<=p.x && p.x<=max.x+dmin && 
					min.y-dmin<=p.y && p.y<=max.y+dmin && 
					min.z-dmin<=p.z && p.z<=max.z+dmin)
				{
					if(!frontEnabled || scale<frontScale)
					{
						frontEnabled=true;
						frontScale=scale;
					}
					if(!backEnabled || scale>backScale)
					{
						backEnabled=true;
						backScale=scale;
					}
				}
			}
		public:
			KdContainer*						kd;

			KdObject(Shape shape)
				:Geometry(shape)
				,kd(0)
			{
			}

			~KdObject()
			{
				DeleteKdTree();
			}

			bool IntersectInternal(const Ray3& ray, Plane3& plane, Vector3& diffuseNormal, Material& planeMaterial)
			{
				Vector3 d=Normalize(ray.direction, 1.0);
				double scale=-1;
				if(kd)
				{
					const double dmin=0.00001;

					double frontScale=-1;
					bool frontEnabled=false;
					double backScale=-1;
					bool backEnabled=false;
					if(d.x<-dmin || d.x>dmin)
					{
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->min.x-ray.position.x)/ray.direction.x, kd->min, kd->max, ray);
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->max.x-ray.position.x)/ray.direction.x, kd->min, kd->max, ray);
					}
					if(d.y<-dmin || d.y>dmin)
					{
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->min.y-ray.position.y)/ray.direction.y, kd->min, kd->max, ray);
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->max.y-ray.position.y)/ray.direction.y, kd->min, kd->max, ray);
					}
					if(d.z<-dmin || d.z>dmin)
					{
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->min.z-ray.position.z)/ray.direction.z, kd->min, kd->max, ray);
						UpdateBoxScale(frontScale, frontEnabled, backScale, backEnabled, (kd->max.z-ray.position.z)/ray.direction.z, kd->min, kd->max, ray);
					}
					if(frontEnabled && backEnabled)
					{
						Vector3 front=ray.position+Scale(ray.direction, frontScale);
						Vector3 back=ray.position+Scale(ray.direction, backScale);
						KdIntersect(d, scale, kd->root, front, back, ray, plane, diffuseNormal, planeMaterial);
					}
				}
				else
				{
					for(int i=KD_CALLBACK->CallbackGetObjectCount()-1;i>=0;i--)
					{
						T& t=KD_CALLBACK->CallbackGetObject(i);
						KD_CALLBACK->CallbackContentIntersect(d, scale, t, ray, plane, diffuseNormal, planeMaterial);
					}
				}
				return scale>0;
			}

			void BuildKdTree(double blockSize, int blockMaxObjects)
			{
				DeleteKdTree();
				if(KD_CALLBACK->CallbackGetObjectCount()>0)
				{
					Vectorf3 min, max;
					KD_CALLBACK->CallbackFillBox(min, max);

					int xs=(int)ceil((max.x-min.x)/blockSize);
					int ys=(int)ceil((max.y-min.y)/blockSize);
					int zs=(int)ceil((max.z-min.z)/blockSize);
					kd=new KdContainer(12*xs*ys*zs);
					kd->min.x=min.x-0.001;
					kd->min.y=min.y-0.001;
					kd->min.z=min.z-0.001;
					kd->max.x=max.x+0.001;
					kd->max.y=max.y+0.001;
					kd->max.z=max.z+0.001;

					List<int> indices;
					for(int i=KD_CALLBACK->CallbackGetObjectCount()-1;i>=0;i--)
					{
						indices.Add(i);
					}
					kd->root=BuildKdTreeInternal(KdTree<KdNode>::X, indices, kd->min, kd->max, blockSize, blockMaxObjects);
				}
			}

			void DeleteKdTree()
			{
				if(kd)delete kd;
				kd=0;
			}
		};
	}
}

#undef CALLBACK
#undef BUILD_SUB_NODE

#endif