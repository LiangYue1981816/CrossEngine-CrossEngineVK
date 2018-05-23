/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include "CrossEngine.h"


namespace glm {

	typedef struct plane {
	public:
		plane(void)
		{
			vertex = glm::vec3(0.0f, 0.0f, 0.0f);
			normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		plane(const plane &_plane)
		{
			vertex = _plane.vertex;
			normal = _plane.normal;
		}

		plane(const vec3 &_vertex, const vec3 &_normal)
		{
			vertex = _vertex;
			normal = _normal;
		}

		plane& operator = (const plane &_plane)
		{
			vertex = _plane.vertex;
			normal = _plane.normal;
			return *this;
		}

		bool operator == (const plane &_plane)
		{
			if (vertex != _plane.vertex) return false;
			if (normal != _plane.normal) return false;
			return true;
		}

		bool operator != (const plane &_plane)
		{
			if (vertex == _plane.vertex) return false;
			if (normal == _plane.normal) return false;
			return true;
		}

		plane& operator * (const mat4 &_mat4)
		{
			vertex = vec3(_mat4 * vec4(vertex, 1.0f));
			normal = vec3(_mat4 * vec4(normal, 0.0f));
			return *this;
		}

		plane& operator *= (const mat4 &_mat4)
		{
			vertex = vec3(_mat4 * vec4(vertex, 1.0f));
			normal = vec3(_mat4 * vec4(normal, 0.0f));
			return *this;
		}

		float distance(const vec3 &_vertex)
		{
			return dot(_vertex - vertex, normal);
		}

		int side(const vec3 &_vertex)
		{
			float dis = distance(_vertex);

			if (dis < -0.001f) return -1;
			if (dis > 0.001f) return  1;

			return 0;
		}

		vec3 project(const vec3 &_vertex)
		{
			vec3 vv = _vertex - vertex;
			vec3 pp = dot(vv, normal) * normal;
			vec3 vp = vv - pp;
			return vertex + vp;
		}

	public:
		vec3 vertex;
		vec3 normal;
	} plane;

	typedef struct aabb {
	public:
		static aabb create(const vec3 *vertices, size_t count)
		{
			vec3 minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
			vec3 maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);

			for (size_t index = 0; index < count; index++) {
				if (minVertex.x > vertices[index].x) minVertex.x = vertices[index].x;
				if (minVertex.y > vertices[index].y) minVertex.y = vertices[index].y;
				if (minVertex.z > vertices[index].z) minVertex.z = vertices[index].z;

				if (maxVertex.x < vertices[index].x) maxVertex.x = vertices[index].x;
				if (maxVertex.y < vertices[index].y) maxVertex.y = vertices[index].y;
				if (maxVertex.z < vertices[index].z) maxVertex.z = vertices[index].z;
			}

			return aabb(minVertex, maxVertex);
		}

	public:
		aabb(void)
		{

		}

		aabb(const aabb &_aabb)
		{
			minVertex = _aabb.minVertex;
			maxVertex = _aabb.maxVertex;
			normalize();
		}

		aabb(const vec3 &_minVertex, const vec3 &_maxVertex)
		{
			minVertex = _minVertex;
			maxVertex = _maxVertex;
			normalize();
		}

		aabb& operator = (const aabb &_aabb)
		{
			minVertex = _aabb.minVertex;
			maxVertex = _aabb.maxVertex;
			normalize();

			return *this;
		}

		bool operator == (const aabb &_aabb)
		{
			if (minVertex != _aabb.minVertex) return false;
			if (maxVertex != _aabb.maxVertex) return false;
			return true;
		}

		bool operator != (const aabb &_aabb)
		{
			if (minVertex == _aabb.minVertex) return false;
			if (maxVertex == _aabb.maxVertex) return false;
			return true;
		}

		aabb& operator * (const mat4 &_mat4)
		{
			minVertex = vec3(_mat4 * vec4(minVertex, 1.0f));
			maxVertex = vec3(_mat4 * vec4(maxVertex, 1.0f));
			normalize();

			return *this;
		}

		aabb& operator *= (const mat4 &_mat4)
		{
			minVertex = vec3(_mat4 * vec4(minVertex, 1.0f));
			maxVertex = vec3(_mat4 * vec4(maxVertex, 1.0f));
			normalize();

			return *this;
		}

		void normalize(void)
		{
			float minx = minVertex.x;
			float miny = minVertex.y;
			float minz = minVertex.z;
			float maxx = maxVertex.x;
			float maxy = maxVertex.y;
			float maxz = maxVertex.z;

			minVertex.x = min(minx, maxx);
			minVertex.y = min(miny, maxy);
			minVertex.z = min(minz, maxz);

			maxVertex.x = max(minx, maxx);
			maxVertex.y = max(miny, maxy);
			maxVertex.z = max(minz, maxz);

			center.x = (minVertex.x + maxVertex.x) * 0.5f;
			center.y = (minVertex.y + maxVertex.y) * 0.5f;
			center.z = (minVertex.z + maxVertex.z) * 0.5f;
		}

		bool inside(const vec3 &_vertex)
		{
			return
				_vertex.x > minVertex.x && _vertex.x < maxVertex.x &&
				_vertex.y > minVertex.y && _vertex.y < maxVertex.y &&
				_vertex.z > minVertex.z && _vertex.z < maxVertex.z;
		}

		bool intersect(const aabb &_aabb)
		{
			if (minVertex.x > _aabb.maxVertex.x || maxVertex.x < _aabb.minVertex.x) return false;
			if (minVertex.y > _aabb.maxVertex.y || maxVertex.y < _aabb.minVertex.y) return false;
			if (minVertex.z > _aabb.maxVertex.z || maxVertex.z < _aabb.minVertex.z) return false;
			return true;
		}

	public:
		vec3 center;
		vec3 minVertex;
		vec3 maxVertex;
	} aabb;

	typedef struct camera {
	public:
		camera(void)
		{

		}

		void setViewport(float x, float y, float width, float height)
		{
			viewport.x = x;
			viewport.y = y;
			viewport.z = width;
			viewport.w = height;
		}

		void setPerspective(float fovy, float aspect, float zNear, float zFar)
		{
			float angle = radians(fovy);
			float d = 1.0f / tan(angle * 0.5f);

			mtxProjection = glm::perspective(angle, aspect, zNear, zFar);

			planes[0][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -d, -1.0f));
			planes[1][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, d, -1.0f));
			planes[2][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(d, 0.0f, -aspect));
			planes[3][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(-d, 0.0f, -aspect));
			planes[4][0] = plane(vec3(0.0f, 0.0f, -zNear), vec3(0.0f, 0.0f, -1.0f));
			planes[5][0] = plane(vec3(0.0f, 0.0f, -zFar), vec3(0.0f, 0.0f, 1.0f));
		}

		void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			mtxProjection = glm::ortho(left, right, bottom, top, zNear, zFar);

			planes[0][0] = plane(vec3(0.0f, top, 0.0f), vec3(0.0f, -1.0f, 0.0f));
			planes[1][0] = plane(vec3(0.0f, bottom, 0.0f), vec3(0.0f, 1.0f, 0.0f));
			planes[2][0] = plane(vec3(left, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));
			planes[3][0] = plane(vec3(right, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f));
			planes[4][0] = plane(vec3(0.0f, 0.0f, zNear), vec3(0.0f, 0.0f, -1.0f));
			planes[5][0] = plane(vec3(0.0f, 0.0f, zFar), vec3(0.0f, 0.0f, 1.0f));
		}

		void setLookat(const vec3 &_eye, const vec3 &_center, const vec3 &_up)
		{
			position = _eye;
			forward = normalize(_center - _eye);
			up = normalize(_up);

			mtxView = glm::lookAt(_eye, _center, _up);
			mtxViewInverse = glm::inverse(mtxView);
			mtxViewInverseTranspose = glm::transpose(mtxViewInverse);

			planes[0][1] = planes[0][0] * mtxViewInverse;
			planes[1][1] = planes[1][0] * mtxViewInverse;
			planes[2][1] = planes[2][0] * mtxViewInverse;
			planes[3][1] = planes[3][0] * mtxViewInverse;
			planes[4][1] = planes[4][0] * mtxViewInverse;
			planes[5][1] = planes[5][0] * mtxViewInverse;
		}

		vec3 worldToScreen(const vec3 &world)
		{
			return project(world, mtxView, mtxProjection, viewport);
		}

		vec3 screenToWorld(const vec3 &screen)
		{
			return unProject(screen, mtxView, mtxProjection, viewport);
		}

		bool visible(const vec3 &_vertex)
		{
			if (planes[0][1].side(_vertex) == -1) return false;
			if (planes[1][1].side(_vertex) == -1) return false;
			if (planes[2][1].side(_vertex) == -1) return false;
			if (planes[3][1].side(_vertex) == -1) return false;
			if (planes[4][1].side(_vertex) == -1) return false;
			if (planes[5][1].side(_vertex) == -1) return false;
			return true;
		}

		bool visible(const aabb &_aabb)
		{
			vec3 vertices[8] = {
				vec3(_aabb.minVertex.x, _aabb.minVertex.y, _aabb.minVertex.z),
				vec3(_aabb.minVertex.x, _aabb.minVertex.y, _aabb.maxVertex.z),
				vec3(_aabb.minVertex.x, _aabb.maxVertex.y, _aabb.minVertex.z),
				vec3(_aabb.minVertex.x, _aabb.maxVertex.y, _aabb.maxVertex.z),
				vec3(_aabb.maxVertex.x, _aabb.minVertex.y, _aabb.minVertex.z),
				vec3(_aabb.maxVertex.x, _aabb.minVertex.y, _aabb.maxVertex.z),
				vec3(_aabb.maxVertex.x, _aabb.maxVertex.y, _aabb.minVertex.z),
				vec3(_aabb.maxVertex.x, _aabb.maxVertex.y, _aabb.maxVertex.z),
			};

			for (int indexPlane = 0; indexPlane < 6; indexPlane++) {
				for (int indexVertex = 0; indexVertex < 8; indexVertex++) {
					if (planes[indexPlane][1].side(vertices[indexVertex]) == 1) {
						goto NEXT;
					}
				}

				return false;
			NEXT:
				continue;
			}

			return true;
		}

	public:
		vec4 viewport;

		vec3 position;
		vec3 forward;
		vec3 up;

		mat4 mtxProjection;
		mat4 mtxView;
		mat4 mtxViewInverse;
		mat4 mtxViewInverseTranspose;

		plane planes[6][2];
	} camera;

}
