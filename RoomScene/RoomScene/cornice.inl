const int cornice_vertices_count = 16 ;
const float cornice_vertices[cornice_vertices_count * 8] =
{
	//       Position                 Normal             Tex Coord
	// Front face
	-1.0f, -0.96f, 1.0f,		 0.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	-1.0f,-1.0f, 0.96f,		 0.0f, 1.0f, 1.0f,		0.0f, 0.0f,
	1.0f,-1.0f, 0.96f,		 0.0f, 1.0f, 1.0f,		1.0f, 0.0f,
	1.0f, -0.96f, 1.0f,		 0.0f, 1.0f, 1.0f,		1.0f, 1.0f,
	// Right face
	1.0f, -0.96f, 1.0f,		 1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	0.96f,-1.0f, 1.0f,		 1.0f, 1.0f, 0.0f,		0.0f, 0.0f,
	0.96f,-1.0f,-1.0f,		 1.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	1.0f, -0.96f,-1.0f,		 1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	// Back face
	1.0f,-0.96f,-1.0f,		 0.0f, 0.0f,-1.0f,		0.0f, 1.0f,
	1.0f,-1.0f,-0.96f,		 0.0f, 0.0f,-1.0f,		0.0f, 0.0f,
	-1.0f,-1.0f,-0.96f,		 0.0f, 0.0f,-1.0f,		1.0f, 0.0f,
	-1.0f, -0.96f,-1.0f,		 0.0f, 0.0f,-1.0f,		1.0f, 1.0f,
	//// Left face
	-1.0f, -0.96f,-1.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.96f,-1.0f,-1.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.96f,-1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-1.0f, -0.96f, 1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	
};

const int cornice_indices_count = 24;
const unsigned int cornice_indices[cornice_indices_count] =
{
	0, 1, 2,		0, 2, 3,		// Front face
	4, 5, 6,		4, 6, 7,		// Right face
	8, 9, 10,		8, 10, 11,		// Back face
	12, 13, 14,		12, 14, 15,		// Left face
	
};
