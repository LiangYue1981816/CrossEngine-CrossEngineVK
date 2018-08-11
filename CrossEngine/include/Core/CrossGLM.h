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
		void zero(void)
		{
			vertex = glm::vec3(0.0f, 0.0f, 0.0f);
			normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		void set(const vec3 &_vertex, const vec3 &_normal)
		{
			vertex = _vertex;
			normal = glm::normalize(_normal);
		}

	public:
		plane(void)
		{
			zero();
		}

		plane(const vec3 &_vertex, const vec3 &_normal)
		{
			set(_vertex, _normal);
		}

		plane(const plane &_plane)
		{
			set(_plane.vertex, _plane.normal);
		}

		plane& operator = (const plane &_plane)
		{
			set(_plane.vertex, _plane.normal);
			return *this;
		}

		plane& operator * (const mat4 &_mat4)
		{
			set(vec3(_mat4 * vec4(vertex, 1.0f)), vec3(_mat4 * vec4(normal, 0.0f)));
			return *this;
		}

		plane& operator *= (const mat4 &_mat4)
		{
			set(vec3(_mat4 * vec4(vertex, 1.0f)), vec3(_mat4 * vec4(normal, 0.0f)));
			return *this;
		}

		bool operator == (const plane &_plane) const
		{
			if (vertex != _plane.vertex) return false;
			if (normal != _plane.normal) return false;

			return true;
		}

		bool operator != (const plane &_plane) const
		{
			if (vertex == _plane.vertex) return false;
			if (normal == _plane.normal) return false;

			return true;
		}

		float distance(const vec3 &_vertex) const
		{
			return dot(_vertex - vertex, normal);
		}

		int side(const vec3 &_vertex) const
		{
			float dis = distance(_vertex);

			if (dis < -0.001f) return -1;
			if (dis >  0.001f) return  1;

			return 0;
		}

		vec3 project(const vec3 &_vertex) const
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

			for (size_t indexVertex = 0; indexVertex < count; indexVertex++) {
				if (minVertex.x > vertices[indexVertex].x) minVertex.x = vertices[indexVertex].x;
				if (minVertex.y > vertices[indexVertex].y) minVertex.y = vertices[indexVertex].y;
				if (minVertex.z > vertices[indexVertex].z) minVertex.z = vertices[indexVertex].z;

				if (maxVertex.x < vertices[indexVertex].x) maxVertex.x = vertices[indexVertex].x;
				if (maxVertex.y < vertices[indexVertex].y) maxVertex.y = vertices[indexVertex].y;
				if (maxVertex.z < vertices[indexVertex].z) maxVertex.z = vertices[indexVertex].z;
			}

			return aabb(minVertex, maxVertex);
		}

	public:
		void zero(void)
		{
			minVertex = glm::vec3(0.0f, 0.0f, 0.0f);
			maxVertex = glm::vec3(0.0f, 0.0f, 0.0f);
			center = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		void set(const vec3 &_minVertex, const vec3 &_maxVertex)
		{
			minVertex = _minVertex;
			maxVertex = _maxVertex;
			center = glm::vec3(0.0f, 0.0f, 0.0f);
		}

	public:
		aabb(void)
		{
			zero();
		}

		aabb(const vec3 &_minVertex, const vec3 &_maxVertex)
		{
			set(_minVertex, _maxVertex);
			normalize();
		}

		aabb(const aabb &_aabb)
		{
			set(_aabb.minVertex, _aabb.maxVertex);
			normalize();
		}

		aabb& operator = (const aabb &_aabb)
		{
			set(_aabb.minVertex, _aabb.maxVertex);
			normalize();
			return *this;
		}

		aabb& operator * (const mat4 &_mat4)
		{
			set(vec3(_mat4 * vec4(minVertex, 1.0f)), vec3(_mat4 * vec4(maxVertex, 1.0f)));
			normalize();
			return *this;
		}

		aabb& operator *= (const mat4 &_mat4)
		{
			set(vec3(_mat4 * vec4(minVertex, 1.0f)), vec3(_mat4 * vec4(maxVertex, 1.0f)));
			normalize();
			return *this;
		}

		bool operator == (const aabb &_aabb) const
		{
			if (minVertex != _aabb.minVertex) return false;
			if (maxVertex != _aabb.maxVertex) return false;

			return true;
		}

		bool operator != (const aabb &_aabb) const
		{
			if (minVertex == _aabb.minVertex) return false;
			if (maxVertex == _aabb.maxVertex) return false;

			return true;
		}

		void normalize(void)
		{
			float minx = minVertex.x;
			float miny = minVertex.y;
			float minz = minVertex.z;
			float maxx = maxVertex.x;
			float maxy = maxVertex.y;
			float maxz = maxVertex.z;

			minVertex = glm::vec3(min(minx, maxx), min(miny, maxy), min(minz, maxz));
			maxVertex = glm::vec3(max(minx, maxx), max(miny, maxy), max(minz, maxz));
			center = (minVertex + maxVertex) * 0.5f;
		}

		bool valid(void) const
		{
			if (minVertex.x > maxVertex.x) return false;
			if (minVertex.y > maxVertex.y) return false;
			if (minVertex.z > maxVertex.z) return false;

			if (maxVertex.x - minVertex.x < 0.001f) return false;
			if (maxVertex.y - minVertex.y < 0.001f) return false;
			if (maxVertex.z - minVertex.z < 0.001f) return false;

			return true;
		}

		bool inside(const vec3 &_vertex) const
		{
			return
				_vertex.x > minVertex.x && _vertex.x < maxVertex.x &&
				_vertex.y > minVertex.y && _vertex.y < maxVertex.y &&
				_vertex.z > minVertex.z && _vertex.z < maxVertex.z;
		}

		bool intersect(const aabb &_aabb) const
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

	typedef struct sphere {
	public:
		static sphere create(const vec3 *vertices, size_t count)
		{
			vec3 minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
			vec3 maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);

			for (size_t indexVertex = 0; indexVertex < count; indexVertex++) {
				if (minVertex.x > vertices[indexVertex].x) minVertex.x = vertices[indexVertex].x;
				if (minVertex.y > vertices[indexVertex].y) minVertex.y = vertices[indexVertex].y;
				if (minVertex.z > vertices[indexVertex].z) minVertex.z = vertices[indexVertex].z;

				if (maxVertex.x < vertices[indexVertex].x) maxVertex.x = vertices[indexVertex].x;
				if (maxVertex.y < vertices[indexVertex].y) maxVertex.y = vertices[indexVertex].y;
				if (maxVertex.z < vertices[indexVertex].z) maxVertex.z = vertices[indexVertex].z;
			}

			return sphere(minVertex, maxVertex);
		}

	public:
		void zero(void)
		{
			center = glm::vec3(0.0f, 0.0f, 0.0f);
			radius = 0.0f;
			radius2 = 0.0f;
		}

		void set(const vec3 &_center, float _radius)
		{
			center = _center;
			radius = _radius;
			radius2 = radius * radius;
		}

		void set(const vec3 &_minVertex, const vec3 &_maxVertex)
		{
			center = (_maxVertex + _minVertex) / 2.0f;
			radius = length(_maxVertex - _minVertex) / 2.0f;
			radius2 = radius * radius;
		}

	public:
		sphere(void)
		{
			zero();
		}

		sphere(const vec3 &_center, float _radius)
		{
			set(_center, _radius);
		}

		sphere(const vec3 &_minVertex, const vec3 &_maxVertex)
		{
			set(_minVertex, _maxVertex);
		}

		sphere(const sphere &_sphere)
		{
			set(_sphere.center, _sphere.radius);
		}

		sphere& operator = (const sphere &_sphere)
		{
			set(_sphere.center, _sphere.radius);
			return *this;
		}

		sphere& operator * (const mat4 &_mat4)
		{
			center = vec3(_mat4 * vec4(center, 1.0));
			return *this;
		}

		sphere& operator *= (const mat4 &_mat4)
		{
			center = vec3(_mat4 * vec4(center, 1.0));
			return *this;
		}

		bool operator == (const sphere &_sphere) const
		{
			if (center != _sphere.center) return false;
			if (radius != _sphere.radius) return false;

			return true;
		}

		bool operator != (const sphere &_sphere) const
		{
			if (center == _sphere.center) return false;
			if (radius == _sphere.radius) return false;

			return true;
		}

		bool valid(void) const
		{
			return radius > 0.001f;
		}

		bool inside(const vec3 &_vertex) const
		{
			vec3 delta = center - _vertex;
			return delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < radius2;
		}

		bool intersect(const sphere &_sphere) const
		{
			vec3 delta = center - _sphere.center;
			return delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < radius2 + _sphere.radius2;
		}

	public:
		vec3 center;
		float radius;
		float radius2;
	} sphere;

	typedef struct camera {
	public:
		camera(void)
		{

		}

		void setScissor(float x, float y, float width, float height)
		{
			scissor.x = x;
			scissor.y = y;
			scissor.z = width;
			scissor.w = height;
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

			projectionMatrix = glm::perspective(angle, aspect, zNear, zFar);

			planes[0][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -d, -1.0f));
			planes[1][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, d, -1.0f));
			planes[2][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(d, 0.0f, -aspect));
			planes[3][0] = plane(vec3(0.0f, 0.0f, 0.0f), vec3(-d, 0.0f, -aspect));
			planes[4][0] = plane(vec3(0.0f, 0.0f, -zNear), vec3(0.0f, 0.0f, -1.0f));
			planes[5][0] = plane(vec3(0.0f, 0.0f, -zFar), vec3(0.0f, 0.0f, 1.0f));
		}

		void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

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

			viewMatrix = glm::lookAt(_eye, _center, _up);
			viewInverseMatrix = glm::inverse(viewMatrix);
			viewInverseTransposeMatrix = glm::transpose(viewInverseMatrix);

			for (int indexPlane = 0; indexPlane < 6; indexPlane++) {
				planes[indexPlane][1] = planes[indexPlane][0] * viewInverseMatrix;
			}
		}

		vec3 worldToScreen(const vec3 &world) const
		{
			return project(world, viewMatrix, projectionMatrix, viewport);
		}

		vec3 screenToWorld(const vec3 &screen) const
		{
			return unProject(screen, viewMatrix, projectionMatrix, viewport);
		}

		bool visible(const vec3 &_vertex) const
		{
			for (int indexPlane = 0; indexPlane < 6; indexPlane++) {
				if (planes[indexPlane][1].side(_vertex) == -1) {
					return false;
				}
			}

			return true;
		}

		bool visible(const sphere &_sphere) const
		{
			if (_sphere.valid() == false) {
				return false;
			}

			for (int indexPlane = 0; indexPlane < 6; indexPlane++) {
				if (planes[indexPlane][1].distance(_sphere.center) < -_sphere.radius) {
					return false;
				}
			}

			return true;
		}

		bool visible(const aabb &_aabb) const
		{
			if (_aabb.valid() == false) {
				return false;
			}

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
		vec4 scissor;
		vec4 viewport;

		vec3 position;
		vec3 forward;
		vec3 up;

		mat4 projectionMatrix;
		mat4 viewMatrix;
		mat4 viewInverseMatrix;
		mat4 viewInverseTransposeMatrix;

	private:
		plane planes[6][2];
	} camera;

}
