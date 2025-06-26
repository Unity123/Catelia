#pragma once

struct Vector2 {
	float x;
	float y;

	bool operator==(Point2D other);
	bool operator!=(Point2D other);
	bool operator+(Point2D other);
	bool operator-(Point2D other);
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};