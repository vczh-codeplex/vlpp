#ifndef VCZH_DIRECTX_MODELBUILDER
#define VCZH_DIRECTX_MODELBUILDER

#include "ModelEditorRenderer.h"

namespace modeleditor
{
	extern void				BuildCube(Model* model);
	extern void				BuildSphere(Model* model, int rows, int cols);
	extern void				BuildCylinder(Model* model, int rows, int cols);
	extern void				BuildCone(Model* model, int rows, int cols);
}

#endif