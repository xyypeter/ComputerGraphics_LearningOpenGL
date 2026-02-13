#pragma once
#include"object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh : public Object {
public:
	Mesh(Geometry* geometry, Material* material);
	~Mesh();

public:
	Geometry* mGeometry{ nullptr };//¼¸ºÎ
	Material* mMaterial{ nullptr };//²ÄÖÊ
};
