/*
CollisionHandler
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/

/*
* most of these come from a version of
* https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
* and have been modified to be more fitting to this engine
*/

#include "../../include/Scene/CollisionHandler.h"
#include <math.h>
namespace AA
{
CollisionHandler* CollisionHandler::getInstance()
{
	static CollisionHandler* cHandler = new CollisionHandler();
	return cHandler;
}
CollisionHandler::CollisionHandler() {};
/**
 * Bounding Box Collision for 2 3D rectagular Boxes.
 * @param[in] a  An impassible collider blockage, A.
 * @param[in] b  An impassible collider blockage, B.
 * @return       True if they collide. False if they do not collide.
 */
bool CollisionHandler::AABB_vs_AABB_3D(const ColliderBox& a, const ColliderBox& b)
{
	float aMinX = a.loc.x - a.size.x / 2.f;
	float aMaxX = a.loc.x + a.size.x / 2.f;
	float aMinY = a.loc.y - a.size.y / 2.f;
	float aMaxY = a.loc.y + a.size.y / 2.f;
	float aMinZ = a.loc.z - a.size.z / 2.f;
	float aMaxZ = a.loc.z + a.size.z / 2.f;

	float bMinX = b.loc.x - b.size.x / 2.f;
	float bMaxX = b.loc.x + b.size.x / 2.f;
	float bMinY = b.loc.y - b.size.y / 2.f;
	float bMaxY = b.loc.y + b.size.y / 2.f;
	float bMinZ = b.loc.z - b.size.z / 2.f;
	float bMaxZ = b.loc.z + b.size.z / 2.f;

	return (aMinX <= bMaxX && aMaxX >= bMinX) && (aMinY <= bMaxY && aMaxY >= bMinY) && (aMinZ <= bMaxZ && aMaxZ >= bMinZ);
}

bool CollisionHandler::point_vs_AABB_3D(const glm::vec3& pt, const ColliderBox& b)
{
	float bMinX = b.loc.x - b.size.x / 2.f;
	float bMaxX = b.loc.x + b.size.x / 2.f;
	float bMinY = b.loc.y - b.size.y / 2.f;
	float bMaxY = b.loc.y + b.size.y / 2.f;
	float bMinZ = b.loc.z - b.size.z / 2.f;
	float bMaxZ = b.loc.z + b.size.z / 2.f;

	return (pt.x >= bMinX && pt.x <= bMaxX) && (pt.y >= bMinY && pt.y <= bMaxY) && (pt.z >= bMinZ && pt.z <= bMaxZ);
}

bool CollisionHandler::line_vs_AABB_3D(const glm::vec3& startpt, const glm::vec3& endpt, const ColliderBox& b)
{
	float bMinX = b.loc.x - b.size.x / 2.f;
	float bMaxX = b.loc.x + b.size.x / 2.f;
	float bMinY = b.loc.y - b.size.y / 2.f;
	float bMaxY = b.loc.y + b.size.y / 2.f;
	float bMinZ = b.loc.z - b.size.z / 2.f;
	float bMaxZ = b.loc.z + b.size.z / 2.f;

	glm::vec3 pt = startpt;
	for (int i = 1; i < 9; ++i) // check 9 points along the line
	{
		pt += (endpt / (float)i);
		if ((pt.x >= bMinX && pt.x <= bMaxX) && (pt.y >= bMinY && pt.y <= bMaxY) && (pt.z >= bMinZ && pt.z <= bMaxZ))
		{
			return true;
		}
	}

	return false;
}

bool CollisionHandler::sphere_vs_AABB_3D(const ColliderSphere& sphere, const ColliderBox& box)
{
	float bMinX = box.loc.x - box.size.x / 2.f;
	float bMaxX = box.loc.x + box.size.x / 2.f;
	float bMinY = box.loc.y - box.size.y / 2.f;
	float bMaxY = box.loc.y + box.size.y / 2.f;
	float bMinZ = box.loc.z - box.size.z / 2.f;
	float bMaxZ = box.loc.z + box.size.z / 2.f;

	// get box closest point to sphere center by clamping
	float x = fmax(bMinX, fmin(sphere.center.x, bMaxX));
	float y = fmax(bMinY, fmin(sphere.center.y, bMaxY));
	float z = fmax(bMinZ, fmin(sphere.center.z, bMaxZ));

	// this is the same as point_vs_Sphere_3D
	float distance = sqrt(
		(x - sphere.center.x) * (x - sphere.center.x) +
		(y - sphere.center.y) * (y - sphere.center.y) +
		(z - sphere.center.z) * (z - sphere.center.z)
	);

	return distance < sphere.radius;
}

bool CollisionHandler::sphere_vs_Sphere_3D(const ColliderSphere* sphere, const ColliderSphere* other)
{
	if (!sphere || !other)
	{
		throw("null exception");
	}
	float distance_pow2 =
		(sphere->center.x - other->center.x) * (sphere->center.x - other->center.x) +
		(sphere->center.y - other->center.y) * (sphere->center.y - other->center.y) +
		(sphere->center.z - other->center.z) * (sphere->center.z - other->center.z);
	return distance_pow2 < pow((sphere->radius + other->radius), 2);
}

bool CollisionHandler::point_vs_Sphere_3D(const glm::vec3& point, const ColliderSphere& sphere)
{
	float distance = sqrt(
		(point.x - sphere.center.x) * (point.x - sphere.center.x) +
		(point.y - sphere.center.y) * (point.y - sphere.center.y) +
		(point.z - sphere.center.z) * (point.z - sphere.center.z)
	);
	return distance < sphere.radius;
}

bool CollisionHandler::line_vs_Sphere_3D(const glm::vec3& startpoint, const glm::vec3& endpoint, const ColliderSphere& sphere)
{
	glm::vec3 pt = startpoint;
	for (int i = 1; i < 9; ++i) // check 9 points along the line
	{
		pt += (endpoint / (float)i);
		if (point_vs_Sphere_3D(pt, sphere))
		{
			return true;
		}
	}
	return false;
}
}  // end namespace AA