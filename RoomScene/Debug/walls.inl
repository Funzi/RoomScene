const int walls_vertices_count = 24;
const float walls_vertices[walls_vertices_count * 8] =
{
	//       Position                 Normal             Tex Coord
	// Front face
	-1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,		 0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
	1.0f,-1.0f, 1.0f,		 0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	// Right face
	1.0f, 1.0f, 1.0f,		 -1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	1.0f,-1.0f, 1.0f,		 -1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 1.0f,-1.0f,		 -1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	// Back face
	1.0f, 1.0f,-1.0f,		 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	1.0f,-1.0f,-1.0f,		 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,		 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,		 0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	// Left face
	-1.0f, 1.0f,-1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-1.0f,-1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	// Top face
	-1.0f, 1.0f,-1.0f,		 0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,		 0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		 0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 1.0f,-1.0f,		 0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
	// Bottom face
	-1.0f,-1.0f, 1.0f,		 0.0f,1.0f, 0.0f,		0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,		 0.0f,1.0f, 0.0f,		0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,		 0.0f,1.0f, 0.0f,		1.0f, 0.0f,
	1.0f,-1.0f, 1.0f,		 0.0f,1.0f, 0.0f,		1.0f, 1.0f,
};

const int walls_indices_count = 30;
const unsigned int walls_indices[walls_indices_count] =
{
	0, 1, 2,		0, 2, 3,		// Front face
	4, 5, 6,		4, 6, 7,		// Right face
	8, 9, 10,		8, 10, 11,		// Back face
	12, 13, 14,		12, 14, 15,		// Left face
	16, 17, 18,		16, 18, 19,		// Top face
	//20, 21, 22,		20, 22, 23,		// Bottom face
};
