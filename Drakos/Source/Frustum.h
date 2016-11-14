#pragma once

#include "DXMath.h"

class Frustum
{
public:
	Frustum();
	~Frustum();

	void Initialize(float);
	void ConstructFrustum(DirectX::XMMATRIX, DirectX::XMMATRIX);
	bool CheckPoint(float x, float y, float z) const;
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius) const;
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius) const;
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) const;
	bool CheckRectangle2(float maxWidth, float maxHeight, float maxDepth, float minWidth, float minHeight, float minDepth) const;

private:
	Frustum(const Frustum&);
	float m_screenDepth;
	float m_planes[6][4];
};

