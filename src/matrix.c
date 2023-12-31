#include "common.h"

const Matrix IDENTITY_MATRIX = {{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
}};
     
Matrix multiplymat4(const Matrix* m1, const Matrix* m2) {
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row = 0, row_offset = row * 4; row < 4; ++row, row_offset = row * 4)
		for (column = 0; column < 4; ++column)
			out.m[row_offset + column] =
				(m1->m[row_offset + 0] * m2->m[column + 0]) +
				(m1->m[row_offset + 1] * m2->m[column + 4]) +
				(m1->m[row_offset + 2] * m2->m[column + 8]) +
				(m1->m[row_offset + 3] * m2->m[column + 12]);

	return out;
}

void MrotateZ(const Matrix* m, float angle) {
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[0] = cosine;
	rotation.m[1] = -sine;
	rotation.m[4] = sine;
	rotation.m[5] = cosine;

	memcpy(m->m, multiplymat4(m, &rotation).m, sizeof(m->m));
}

void Mscale(const Matrix* m, float x, float y, float z) {
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	memcpy(m->m, multiplymat4(m, &scale).m, sizeof(m->m));
}

void Mtranslate(const Matrix* m, float x, float y, float z) {
	Matrix translation = IDENTITY_MATRIX;
	
	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, multiplymat4(m, &translation).m, sizeof(m->m));
}

Matrix orthogonal(float left, float right, float bottom, float top) {
	Matrix out = IDENTITY_MATRIX;
	out.m[0] = 2 / (right - left);
	out.m[5] = 2 / (top - bottom);
	out.m[10] = -1;
	out.m[12] = - (right + left) / (right - left);
	out.m[13] = - (top + bottom) / (top - bottom);

	return out;
}

