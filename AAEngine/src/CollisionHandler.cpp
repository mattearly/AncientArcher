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
#include "../include/CollisionHandler.h"
namespace AA
{
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

/**
 * Bouding Box Collision for a point v.s. a 3D rectangular Box.
 * @return        True if they collide. False if they do not collide
 */
bool CollisionHandler::point_vs_AABB_3D(const float& px, const float& py, const float& pz, const ColliderBox& b)
{
	float bMinX = b.loc.x - b.size.x / 2.f;
	float bMaxX = b.loc.x + b.size.x / 2.f;
	float bMinY = b.loc.y - b.size.y / 2.f;
	float bMaxY = b.loc.y + b.size.y / 2.f;
	float bMinZ = b.loc.z - b.size.z / 2.f;
	float bMaxZ = b.loc.z + b.size.z / 2.f;

	return (px >= bMinX && px <= bMaxX) && (py >= bMinY && py <= bMaxY) && (pz >= bMinZ && pz <= bMaxZ);
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

bool CollisionHandler::vector_vs_AABB_3D(const glm::vec3& startpt, const glm::vec3& endpt, const ColliderBox& b)
{
	float bMinX = b.loc.x - b.size.x / 2.f;
	float bMaxX = b.loc.x + b.size.x / 2.f;
	float bMinY = b.loc.y - b.size.y / 2.f;
	float bMaxY = b.loc.y + b.size.y / 2.f;
	float bMinZ = b.loc.z - b.size.z / 2.f;
	float bMaxZ = b.loc.z + b.size.z / 2.f;

	glm::vec3 pt = startpt;
	for (int i = 1; i < 3; ++i)
	{
		pt += (endpt / (float)i);
		if (
			(pt.x >= bMinX && pt.x <= bMaxX) &&
			(pt.y >= bMinY && pt.y <= bMaxY) &&
			(pt.z >= bMinZ && pt.z <= bMaxZ)
			)
		{
			return true;
		}
	}

	return false;
}
}  // end namespace AA
